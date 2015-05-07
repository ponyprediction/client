#include "job.hpp"

#include <QXmlStreamReader>
#include <QDebug>
#include "util.hpp"

Job::Job()
{

}

Job::Job(const int & id,
         const QString & problemsXml,
         const QString & brainXml,
         const int & brainCount,
         const int & interval) :
    id(id),
    problems(),
    bestBrain(),
    brains(),
    brainCount(brainCount),
    interval(interval),
    ratiosToSaveCount(100),
    lastNratios(QVector<float>(100, 0.0f)),
    averageRatio(0.0f),
    lastAverageRatio(0.0f),
    mutexLastNratios(),
    mutexAverageRatio(),
    mutexBestBrain(),
    mutationFrequency(0.0f),
    mutationIntensity(0.0f)
{
    loadProblems(problemsXml);
    loadBrains(brainXml);
}

Job::~Job()
{

}

void Job::start()
{
    for(int i = 0 ; i < brainCount ; i++)
        brains[i]->start(&problems);
}

void Job::loadProblems(const QString & problemsXml)
{
    problems.clear();
    QXmlStreamReader xmlReader(problemsXml);
    while (!xmlReader.atEnd())
    {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if(token == QXmlStreamReader::StartElement)
        {
            if(xmlReader.name() == "problem")
            {
                problems.push_back(new Problem(xmlReader));
            }
        }
    }
}

void Job::loadBrains(const QString & brainXml)
{
    brains.clear();
    for(int i = 0 ; i < brainCount ; i++)
    {
        QXmlStreamReader xmlReader(brainXml);
        brains.push_back(new Brain(this, i+1, xmlReader));
    }
}

void Job::evaluate(Brain * brain)
{
    addRatio(brain->getRatio());
    mutexAverageRatio.lock();
    float averagetmp = averageRatio;
    mutexAverageRatio.unlock();
    Util::addLog("Brain #" + QString::number(brain->getId())
                 + " : " + QString::number(brain->getRatio(), 'f', 6)
                 + " : " + QString::number(averagetmp, 'f', 6)
                 + " : " + QString::number(mutationFrequency, 'f', 6)
                 + " : " + QString::number(mutationIntensity, 'f', 6));
    if(brain->getRatio() > averagetmp)
    {
        copyToBestBrain(brain);
    }
    else
    {
        copyFromBestBrain(brain);
    }
    brain->mutate(mutationFrequency,mutationIntensity);
}

void Job::addRatio(const float & ratio)
{
    mutexLastNratios.lock();
    lastNratios.push_back(ratio);
    while(lastNratios.size() > ratiosToSaveCount)
        lastNratios.pop_front();
    mutexLastNratios.unlock();
    updateAverageRatio();
}

void Job::updateAverageRatio()
{
    mutexLastNratios.lock();
    mutexAverageRatio.lock();
    lastAverageRatio = averageRatio;
    averageRatio = 0.0f;
    for(int i = 0 ; i<lastNratios.size() ; i++)
    {
        averageRatio += lastNratios[i];
    }
    averageRatio /= (float)lastNratios.size();
    float ratioResolution = 0.00001f;
    float mutationFrequencyMax = 1.0f;
    float mutationFrequencyMin = 0.0001f;
    float mutationFrequencyUp =     0.0001f;
    float mutationFrequencyDown =   0.0002f;
    float mutationIntensityMax = 1.0f;
    float mutationIntensityMin = 0.0001f;
    float mutationIntensityUp =     0.0001f;
    float mutationIntensityDown =   0.0002f;
    if(lastAverageRatio > averageRatio - ratioResolution)
    {
        if(0 > Util::getRandomFloat(-100,100))
            mutationFrequency  += mutationFrequencyUp;
        else
            mutationIntensity  += mutationIntensityUp;
    }
    else
    {
        if(0 > Util::getRandomFloat(-100,100))
            mutationFrequency  -= mutationFrequencyDown;
        else
            mutationIntensity  -= mutationIntensityDown;
    }
    if(mutationFrequency > mutationFrequencyMax)
        mutationFrequency = mutationFrequencyMax;
    if(mutationFrequency < mutationFrequencyMin)
        mutationFrequency = mutationFrequencyMin;
    if(mutationIntensity > mutationIntensityMax)
        mutationIntensity = mutationIntensityMax;
    if(mutationIntensity < mutationIntensityMin)
        mutationIntensity = mutationIntensityMin;
    mutexLastNratios.unlock();
    mutexAverageRatio.unlock();
}




void Job::copyToBestBrain(Brain * brain)
{
    mutexBestBrain.lock();
    bestBrain.id = 0;
    bestBrain.inputCount = brain->inputCount;
    bestBrain.outputCount = brain->outputCount;
    bestBrain.weights = brain->weights;
    bestBrain.outputs = QVector<float>(brain->outputCount, 0.0f);
    bestBrain.neurons = QVector<Neuron>();
    bestBrain.inputs = QVector<float>(brain->inputCount, 0.0f);
    bestBrain.neuronBlueprints = brain->neuronBlueprints;
    bestBrain.result = -1;
    bestBrain.score = brain->score;
    bestBrain.attempts = brain->attempts;
    bestBrain.ratio = brain->ratio;
    mutexBestBrain.unlock();
}

void Job::copyFromBestBrain(Brain * brain)
{
    mutexBestBrain.lock();
    brain->inputCount = bestBrain.inputCount;
    brain->outputCount = bestBrain.outputCount;
    brain->weights = bestBrain.weights;
    brain->outputs = QVector<float>(bestBrain.outputCount, 0.0f);
    brain->neurons = QVector<Neuron>();
    brain->inputs = QVector<float>(bestBrain.inputCount, 0.0f);
    brain->neuronBlueprints = bestBrain.neuronBlueprints;
    brain->result =-1;
    brain->score = 0;
    brain->attempts = 0;
    brain->ratio = 0;
    mutexBestBrain.unlock();
    brain->initNeurons();
}
