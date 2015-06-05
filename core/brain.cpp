#include "brain.hpp"
#include <QXmlStreamReader>
#include <QStringList>
#include <QDebug>
#include "core/util.hpp"
#include "core/job.hpp"

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
    json()
{

}

Brain::Brain(Job * job, const int & id, const QJsonObject & json) :
    Brain()
{
    this->id = id;
    this->job = job;
    load(json);
    initNeurons();
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
    while(go)
    {
        //
        compute(problems->at(currentProblemId)->getInputs());
        prepareResult();
        learn(problems->at(currentProblemId)->getWantedOutput());
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
        this->inputs[i] = 0.0f;
    for(int i = 0 ; i < inputs.size() && i < this->inputs.size() ; i++)
        this->inputs[i] = inputs[i];
    for(int i = 0 ; i < neurons.size() ; i++)
        neurons[i].compute();
    for(int i = 0 ; i < outputCount ; i++)
    {
        int i2 = (neurons.size() - outputs.size()) + i;
        outputs[i] = neurons[i2].getOutput();
    }
}

void Brain::prepareResult()
{
    result = -1;
    float ratioBest = -1.0f;
    for(int i = 0 ; i < outputs.size() ; i++)
    {
        if(outputs[i] > ratioBest)
        {
            ratioBest = outputs[i];
            result = i;
        }
    }
    result -= 1; // result € [1 - 20]
}

void Brain::learn(const int & wantedResult)
{
    attempts++;
    if(result == wantedResult)
        score += 1.0f;
    ratio = score / (float)attempts;
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
    QString str;
    bool ok = true;
    //
    if(ok)
    {
        for(int i = 0 ; i < weights.size() ; i++)
        {
            if(i)
            {
                str += ";";
            }
            str += QString::number(weights[i], 'f', 6);
        }
        json["weights"] = str;
        QJsonDocument document;
        document.setObject(json);
        str = document.toJson();
    }
    //
    return str;
}


void Brain::load(const QJsonObject & json)
{
    //
    this->json = json;
    // neuronCount
    neuronCount = json["neuronCount"].toInt();
    neuronBlueprints.clear();
    neurons = QVector<Neuron>(neuronCount, Neuron());
    // inputCount
    inputCount = json["inputCount"].toInt();
    inputs = QVector<float>(inputCount, 0.0f);
    // weightCount
    weightCount = json["weightCount"].toInt();
    weights = QVector<float>(weightCount, 0.0f);
    // weights
    QString weightsStr = json["weights"].toString();
    QStringList weightsStrList = weightsStr.split(';');
    if(weightsStrList.size() == weightCount)
    {
        weights = QVector<float>(weightCount, 0.0f);
        for(int i = 0 ; i < weightCount ; i++)
        {
            weights[i] = weightsStrList[i].toFloat();
        }
    }
    else
    {
        qDebug() << "Problem loading weights : "
                    "the count ain't right";
        qDebug() << weightsStrList.size() << weightCount;
    }
    // outputCount
    outputCount = json["outputCount"].toInt();
    outputs = QVector<float>(outputCount, 0.0f);
    // neurons
    QJsonArray neurarr = json["neurons"].toArray();
    for(int i = 0 ; i < neurarr.size() ; i++)
    {
        neuronBlueprints.push_back(NeuronBlueprint(neurarr[i].toObject()));
    }
}

void Brain::initNeurons()
{
    //
    neurons.clear();
    for(int i = 0 ; i < neuronCount ; i++)
    {
        neurons.push_back(Neuron());
    }
    //
    for(int i = 0 ; i < neurons.size() ; i++)
    {
        NeuronBlueprint blueprint = neuronBlueprints[i];
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
