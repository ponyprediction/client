#include "brain.hpp"
#include "job.hpp"
#include "core/util.hpp"
#include <QStringList>
#include <QXmlStreamReader>

Brain::Brain() :
    QThread(),
    neuronCount(-1),
    inputCount(-1),
    weightCount(-1),
    outputCount(-1),
    jobId(-1),
    neurons(),
    neuronBlueprints(),
    inputs(),
    weights(),
    outputs(),
    go(false),
    currentProblemId(0),
    id(-1),
    attempts(0.0f),
    score(0.0f),
    ratio(0.0f),
    balance(-7000),
    json()

{

}

Brain::Brain(Job * job,
             const int & id,
             const QJsonObject & json,
             const int & seed,
             const Brain::Mode & mode) :
    Brain()
{
    this->id = id;
    this->job = job;
    this->seed = seed;
    this->mode = mode;
    load(json);
    initNeurons();
    for(int i = 0 ; i< weightCount; i++)
    {
        weights[i] = Util::getRandomFloat(-1,1);
    }
}

Brain::~Brain()
{

}

void Brain::start(QVector<Problem *> * problems)
{
    this->problems = problems;
    QThread::start();
}

void Brain::run()
{
    go = true;
    currentProblemId = 0;
    //
    qsrand(seed);

    while(go)
    {
        compute(problems->at(currentProblemId)->getInputs());
        prepareResults();
        learn();
        //
        currentProblemId++;
        currentProblemId%=problems->size();
        if(!currentProblemId)
        {
            job->evaluate(this);
        }
    }
}


void Brain::compute(const QVector<float> & inputs)
{
    for(int i = 0 ; i < this->inputs.size() ; i++)
    {
        this->inputs[i] = 0.0f;
    }
    for(int i = 0 ; i < inputs.size() && i < this->inputs.size() ; i++)
    {
        this->inputs[i] = inputs[i];
    }
    for(int i = 0 ; i < neurons.size() ; i++)
    {
        neurons[i].compute();
    }
    for(int i = 0 ; i < outputCount ; i++)
    {
        int i2 = (neurons.size() - outputs.size()) + i;
        outputs[i] = neurons[i2].getOutput();
    }

}


void Brain::prepareResults()
{
    QVector<float> sortedRatios;
    QVector<float> sortedIds;
    QVector<float> ratios = this->outputs;
    int size = ratios.size();
    float lastRatioBest = 1.0f;
    for(int i = 0 ; i < size ; i++)
    {
        float ratioBest = -1.0f;
        int idBest = 0;
        for(int j = 0 ; j < ratios.size() ; j++)
        {
            if(ratios[j] > ratioBest)
            {
                ratioBest = ratios[j];
                idBest = j;
            }
        }
        lastRatioBest = ratioBest;
        sortedRatios << ratios.at(idBest);
        sortedIds << idBest+1;
        ratios[idBest] = -1.0f;
    }
    results = sortedIds;
}


void Brain::learn()
{
    switch(mode)
    {
    case SINGLE_WIN:
    {
        learnSingleWin(problems->at(currentProblemId)->getWantedOutput());
        break;
    }
    case SINGLE_SHOW:
    {
        learnSingleShow(problems->at(currentProblemId)->getWantedOutputs(),
                        problems->at(currentProblemId)->getCount(),
                        problems->at(currentProblemId)->getWinnings(),
                        problems->at(currentProblemId)->getTargets());
        break;
    }
    default:
    {
        Util::writeError("invalide mode in brain");
    }
    }
}


void Brain::learnSingleWin(const int & wantedResult)
{
    attempts++;
    if(results[0] == wantedResult)
    {
        score += 1.0f;
    }
    ratio = score / (float)attempts;
}


void Brain::learnSingleShow(const QVector<int> & wantedResults,
                            const int & count,
                            const QMap<int, float> & winnings,
                            const QVector<float> &targets)
{
    attempts++;

    if(count > 7)
    {
        if(results[0] == wantedResults[0]
                || results[0] == wantedResults[1]
                || results[0] == wantedResults[2])
        {
            score += 1.0f;
            for(int i = 0; i < 3 ; i ++)
            {
                if(results[0] == wantedResults[i] && winnings.contains(wantedResults[i]))
                {
                    balance += winnings[wantedResults[i]];
                }
            }
        }
    }
    else
    {
        if(results[0] == wantedResults[0]
                || results[0] == wantedResults[1])
        {
            score += 1.0f;
            for(int i = 0; i < 2 ; i ++)
            {
                if(results[0] == wantedResults[i] && winnings.contains(wantedResults[i]))
                {
                    balance += winnings[wantedResults[i]];
                }
            }
        }
    }
    ratio = score / (float)attempts;
    balance--;

    int target = 0;
    for(int i = 0 ; i < connections.size(); i++)
    {
        if(target/20 == 1)
            target = 0;
        float tsigmaK = 0;
        float n = 0.2;
        tsigmaK = connections[i]->next->output*(1-connections[i]->next->output);
        tsigmaK *= (connections[i]->next->output-targets[target]);

        for(int j = 0 ; j < connections[i]->next->weights.size(); j++)
        {
            float delta = 0;
            delta = -n*tsigmaK*connections[i]->previous->output;
            connections[i]->next->tsigmaK = tsigmaK;
            *connections[i]->next->weights[j] = *connections[i]->next->weights[j]+delta;
        }
        tsigmaK = 0;
        tsigmaK = connections[i]->previous->output*(1-connections[i]->previous->output);
        float sum = 0;
        for(int j = 0 ; j < connections.size(); j++)
        {
            if(connections[i]->previous == connections[j]->previous)
            {
                sum += connections[j]->next->tsigmaK**connections[j]->weights;
            }
        }
        tsigmaK*=sum;
        for(int j = 0 ; j < connections[i]->previous->weights.size(); j++)
        {
            float delta = -n*tsigmaK*connections[i]->previous->output;
            *connections[i]->previous->weights[j] = *connections[i]->next->weights[j]+delta;
        }
        target++;
    }


    for(int i = 0 ; i < weightCount ; i++)
    {
        if(weights[i] > 1.0f)
            weights[i] = 1.0f;
        if(weights[i] < -1.0f)
            weights[i] = -1.0f;
    }

    //
}


void Brain::mutate(float mutationFrequency, float mutationIntensity)
{
    for(int i = 0 ; i < weights.size() ; i++)
    {
        if(Util::getRandomFloat(0.0f, 1.0f) < mutationFrequency)
        {
            if(Util::getRandomFloat(-1.0f, 1.0f) > 0)
                weights[i] += mutationIntensity;
            else
                weights[i] -= mutationIntensity;
            if(weights[i] > 1.0f)
                weights[i] = 1.0f;
            if(weights[i] < -1.0f)
                weights[i] = -1.0f;
        }
    }
}


QString Brain::getJson()
{
    // Init
    QString jsonStr = "";
    bool ok = true;
    // Weights
    if(ok)
    {
        QString str = "";
        for(int i = 0 ; i < weights.size() ; i++)
        {
            if(i)
            {
                str += ";";
            }
            str += QString::number(weights[i], 'f', 6);
        }
        json["weights"] = str;
    }
    // Ratio
    if(ok)
    {
        json["ratio"] = ratio;
    }
    //
    if(ok)
    {
        QJsonDocument document;
        document.setObject(json);
        jsonStr = document.toJson();
    }
    //
    return jsonStr;
}


QString Brain::getPrediction(const QVector<float> & inputs)
{
    compute(inputs);
    prepareResults();
    return QString::number(results[0]);
}


void Brain::stop()
{
    go = false;
}


void Brain::load(const QJsonObject & json)
{
    bool ok = true;
    this->json = json;
    QString weightsStr;
    QStringList weightsStrList;
    // neuronCount
    if(ok)
    {
        neuronCount = json["neuronCount"].toInt();
        neuronBlueprints.clear();
        neurons = QVector<Neuron>(neuronCount, Neuron());
    }
    // inputCount
    if(ok)
    {
        inputCount = json["inputCount"].toInt();
        inputs = QVector<float>(inputCount, 0.0f);
    }
    // weightCount
    if(ok)
    {
        weightCount = json["weightCount"].toInt();
        weights = QVector<float>(weightCount, 0.0f);

        weightsStr = json["weights"].toString();
        weightsStrList = weightsStr.split(';');
        if(weightsStrList.size() != weightCount)
        {
            ok = false;
            Util::writeError("problem loading weights, the count ain't right : "
                             + QString::number(weightsStrList.size())
                             + " vs " + QString::number(weightCount));
        }
    }
    // weights
    if(ok)
    {
        weights = QVector<float>(weightCount, 0.0f);
        for(int i = 0 ; i < weightCount ; i++)
        {
            weights[i] = weightsStrList[i].toFloat();
        }

    }
    // outputCount
    if(ok)
    {
        outputCount = json["outputCount"].toInt();
        outputs = QVector<float>(outputCount, 0.0f);
    }
    // neurons
    if(ok)
    {
        QJsonArray neurarr = json["neurons"].toArray();
        for(int i = 0 ; i < neurarr.size() ; i++)
        {
            neuronBlueprints.push_back(NeuronBlueprint(neurarr[i].toObject()));
        }
    }
    // ratio
    if(ok)
    {
        ratio = json["ratio"].toDouble();
    }
}

void Brain::initNeurons()
{
    //
    neurons.clear();
    connections.clear();
    for(int i = 0 ; i < neuronCount ; i++)
    {
        neurons.push_back(Neuron());
    }
    //
    for(int i = 0 ; i < neurons.size() ; i++)
    {
        NeuronBlueprint blueprint = neuronBlueprints[i];
        if(blueprint.neuronalInputIds.size() == 0)
            neurons[i].type = "H";
        else
            neurons[i].type = "O";
        for(int j = 0 ;
            j < blueprint.externalInputIds.size() ;
            j++)
        {
            float * a = &inputs[blueprint.externalInputIds[j]];
            neurons[i].addExternalInput(a);
        }
        for(int j = 0 ;
            j < blueprint.neuronalInputIds.size() ;
            j++)
        {
            float * a =
                    neurons[blueprint.neuronalInputIds[j]].getOutputAdress();
            neurons[i].addNeuronalInput(a);
            Connection * aa = new Connection();
            aa->next = &neurons[i];
            aa->previous = &neurons[blueprint.neuronalInputIds[j]];
            aa->weights = &weights[blueprint.weightIds[j]];
            connections.push_back(aa);
        }
        for(int j = 0 ;
            j < blueprint.weightIds.size() ;
            j++)
        {
            float * a = &weights[blueprint.weightIds[j]];
            neurons[i].addWeight(a);
        }
    }
}
