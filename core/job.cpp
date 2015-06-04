#include "job.hpp"
#include <QXmlStreamReader>
#include <QDebug>
#include <QString>
#include "util.hpp"


int inputCount = 80;


Job::Job() : QObject()
{

}

Job::Job(const int & id,
         const QString & problemsJson,
         const QString & brainJson,
         const int & brainCount,
         const int & interval) :
    QObject(),
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
    mutationFrequency(Util::getLineFromConf("mutationFrequency").toFloat()),
    mutationFrequencyUp(Util::getLineFromConf("mutationFrequencyUp").toFloat()),
    mutationFrequencyDown(Util::getLineFromConf("mutationFrequencyDown").toFloat()),
    mutationFrequencyMax(Util::getLineFromConf("mutationFrequencyMax").toFloat()),
    mutationFrequencyMin(Util::getLineFromConf("mutationFrequencyMin").toFloat()),
    mutationFrequencyAuto(Util::getLineFromConf("mutationFrequencyAuto").toInt()),
    mutationIntensity(Util::getLineFromConf("mutationIntensity").toFloat()),
    mutationIntensityUp(Util::getLineFromConf("mutationIntensityUp").toFloat()),
    mutationIntensityDown(Util::getLineFromConf("mutationIntensityDown").toFloat()),
    mutationIntensityMax(Util::getLineFromConf("mutationIntensityMax").toFloat()),
    mutationIntensityMin(Util::getLineFromConf("mutationIntensityMin").toFloat()),
    mutationIntensityAuto(Util::getLineFromConf("mutationIntensityAuto").toInt())
{
    loadProblems(problemsJson);
    loadBrains(brainJson);
    setMutationFrequency(mutationFrequency);
    setMutationIntensity(mutationIntensity);
}

Job::~Job()
{

}


void Job::start()
{
    for(int i = 0 ; i < brains.size() ; i++)
        brains[i]->start(&problems);
}


void Job::loadProblems(const QString & problemsJson)
{
    problems.clear();
    QJsonDocument json = QJsonDocument::fromJson(problemsJson.toUtf8());
    QJsonArray problemsJsonArray = json.object()["problems"].toArray();



    for(int i = 0 ; i < problemsJsonArray.size() ; i++)
    {
        problems.push_back(new Problem(
                               problemsJsonArray[i].toObject(), inputCount));
    }
}

void Job::loadBrains(const QString & brainJson)
{
    brains.clear();
    /*for(int i = 0 ; i < brainCount ; i++)
    {
        QXmlStreamReader xmlReader(brainJson);
        brains.push_back(new Brain(this, i+1, xmlReader));
    }*/
}

void Job::evaluate(Brain * brain)
{
    addRatio(brain->getRatio());
    mutexAverageRatio.lock();
    float averagetmp = averageRatio;
    mutexAverageRatio.unlock();
    if(brain->getRatio() > averagetmp)
    {
        Util::addLog("Brain #" + QString::number(brain->getId())
                     + " : " + QString::number(brain->getRatio(), 'f', 6)
                     + " : " + QString::number(averagetmp, 'f', 6)
                     + " : " + QString::number(mutationFrequency, 'f', 6)
                     + " : " + QString::number(mutationIntensity, 'f', 6));
        copyToBestBrain(brain);
    }
    copyFromBestBrain(brain);
    brain->mutate(mutationFrequency,mutationIntensity);
}
/******************************************************************************/
void Job::setMutationFrequency(float v)
{
    mutationFrequency = v;
    if(mutationFrequency < mutationFrequencyMin)
    {
        mutationFrequency = mutationFrequencyMin;
    }
    if(mutationFrequency > mutationFrequencyMax)
    {
        mutationFrequency = mutationFrequencyMax;
    }
}

void Job::setMutationFrequencyMin(float v)
{
    mutationFrequencyMin = v;
    if(mutationFrequencyMin > mutationFrequencyMax)
    {
        mutationFrequencyMin = mutationFrequencyMax;
    }
}

void Job::setMutationFrequencyMax(float v)
{
    mutationFrequencyMax = v;
    if(mutationFrequencyMax < mutationFrequencyMin)
    {
        mutationFrequencyMax = mutationFrequencyMin;
    }
}
/******************************************************************************/
void Job::setMutationIntensity(float v)
{
    mutationIntensity = v;
    if(mutationIntensity < mutationIntensityMin)
    {
        mutationIntensity = mutationIntensityMin;
    }
    if(mutationIntensity > mutationIntensityMax)
    {
        mutationIntensity = mutationIntensityMax;
    }
}

void Job::setMutationIntensityMin(float v)
{
    mutationIntensityMin = v;
    if(mutationIntensityMin > mutationIntensityMax)
    {
        mutationIntensityMin = mutationIntensityMax;
    }
}

void Job::setMutationIntensityMax(float v)
{
    mutationIntensityMax = v;
    if(mutationIntensityMax < mutationIntensityMin)
    {
        mutationIntensityMax = mutationIntensityMin;
    }
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
    float borne = 1000.0f;
    if(lastAverageRatio > averageRatio - ratioResolution)
    {
        if(mutationFrequencyAuto && mutationIntensityAuto)
        {
            if(0 > Util::getRandomFloat(-borne,borne))
                upMutationFrequency();
            else
                upMutationIntenstity();
        }
        else if(mutationFrequencyAuto)
        {
            upMutationFrequency();
        }
        else if(mutationIntensityAuto)
        {
            upMutationIntenstity();
        }
    }
    else
    {
        if(mutationFrequencyAuto && mutationIntensityAuto)
        {
            if(0 > Util::getRandomFloat(-borne,borne))
                downMutationFrequency();
            else
                downMutationIntenstity();
        }
        else if(mutationFrequencyAuto)
        {
            downMutationFrequency();
        }
        else if(mutationIntensityAuto)
        {
            downMutationIntenstity();
        }
    }
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


void Job::upMutationFrequency()
{
    mutationFrequency  += mutationFrequencyUp;
    if(mutationFrequency > mutationFrequencyMax)
        mutationFrequency = mutationFrequencyMax;
}


void Job::downMutationFrequency()
{
    mutationFrequency  -= mutationFrequencyDown;
    if(mutationFrequency < mutationFrequencyMin)
        mutationFrequency = mutationFrequencyMin;
}

void Job::upMutationIntenstity()
{
    mutationIntensity  += mutationIntensityUp;
    if(mutationIntensity > mutationIntensityMax)
        mutationIntensity = mutationIntensityMax;
}


void Job::downMutationIntenstity()
{
    mutationIntensity  -= mutationIntensityDown;
    if(mutationIntensity < mutationIntensityMin)
        mutationIntensity = mutationIntensityMin;
}
