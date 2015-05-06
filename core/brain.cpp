#include "brain.hpp"
#include <QXmlStreamReader>
#include <QStringList>
#include <QDebug>
#include "core/util.hpp"

Brain::Brain() :
    neuronCount(-1),
    inputCount(-1),
    weightCount(-1),
    outputCount(-1),
    jobId(-1),
    neurons(),
    neuronBlueprints(),
    inputs(),
    weights(),
    outputs()
{

}

Brain::Brain(QXmlStreamReader & xmlReader) :
    Brain()
{
    load(xmlReader);
    initNeurons();
}

Brain::~Brain()
{

}

void Brain::start(QVector<Problem *> * problems)
{

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
            float * a = neurons[blueprint.neuronalInputIds[j]].getOutputAdress();
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
