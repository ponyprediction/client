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
    ratio(0.0f)
{

}

Brain::Brain(Job * job, const int & id, QXmlStreamReader & xmlReader) :
    Brain()
{
    this->id = id;
    this->job = job;
    load(xmlReader);
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

void Brain::load(QXmlStreamReader & xmlReader)
{
    while (!xmlReader.atEnd()
           && !(xmlReader.name() == "brain"
                && xmlReader.tokenType() == QXmlStreamReader::EndElement))
    {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if(token == QXmlStreamReader::StartElement)
        {
            if(xmlReader.name() == "neuronCount")
            {
                neuronCount = xmlReader.readElementText().toInt();
                neuronBlueprints.clear();
                neurons = QVector<Neuron>(neuronCount, Neuron());
            }
            if(xmlReader.name() == "inputCount")
            {
                inputCount = xmlReader.readElementText().toInt();
                inputs = QVector<float>(inputCount, 0.0f);
            }
            if(xmlReader.name() == "weightCount")
            {
                weightCount = xmlReader.readElementText().toInt();
                weights = QVector<float>(weightCount, 0.0f);
            }
            if(xmlReader.name() == "weights")
            {
                QString weightsStr = xmlReader.readElementText();
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
                    Util::addLog("Problem loading weights : "
                                 "the count ain't right");
                }
            }
            if(xmlReader.name() == "outputCount")
            {
                outputCount = xmlReader.readElementText().toInt();
                outputs = QVector<float>(outputCount, 0.0f);
            }
            if(xmlReader.name() == "neuron")
            {
                neuronBlueprints.push_back(NeuronBlueprint(xmlReader));
            }
            if(xmlReader.name() == "jobId")
            {
                jobId = xmlReader.readElementText().toInt();
            }
        }
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



















