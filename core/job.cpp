#include "job.hpp"
#include "brain.hpp"
#include "util.hpp"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QDir>


int inputCount = 80;


Job::Job() : QObject()
{

}

Job::Job(const QString & id,
         const QString & trainingSetJson,
         const QString & brainJson,
         const int & brainCount,
         bool & ok,
         const Brain::Mode & mode) :
    QObject(),
    id(id),
    problems(),
    bestBrain(),
    bestBrainEver(),
    brains(),
    brainCount(brainCount),
    ratiosToSaveCount(100),
    lastNratios(QVector<float>(100, 0.0f)),
    averageRatio(0.0f),
    averageBalance(0),
    lastAverageBalance(0),
    lastAverageRatio(0.0f),
    mutexLastNratios(),
    mutexAverageRatio(),
    mutexBestBrain(),
    mutexBestBrainEver(),
    mutationFrequency(Util::getLineFromConf("mutationFrequency").toFloat()),
    mutationFrequencyAuto(Util::getLineFromConf("mutationFrequencyAuto").toInt()),
    mutationFrequencyUp(Util::getLineFromConf("mutationFrequencyUp").toFloat()),
    mutationFrequencyDown(Util::getLineFromConf("mutationFrequencyDown").toFloat()),
    mutationFrequencyMax(Util::getLineFromConf("mutationFrequencyMax").toFloat()),
    mutationFrequencyMin(Util::getLineFromConf("mutationFrequencyMin").toFloat()),
    mutationIntensity(Util::getLineFromConf("mutationIntensity").toFloat()),
    mutationIntensityAuto(Util::getLineFromConf("mutationIntensityAuto").toInt()),
    mutationIntensityUp(Util::getLineFromConf("mutationIntensityUp").toFloat()),
    mutationIntensityDown(Util::getLineFromConf("mutationIntensityDown").toFloat()),
    mutationIntensityMax(Util::getLineFromConf("mutationIntensityMax").toFloat()),
    mutationIntensityMin(Util::getLineFromConf("mutationIntensityMin").toFloat()),
    limitDeviation(Util::getLineFromConf("limitDeviation").toFloat()),
    mode(mode),
    bestBalanceEver(-10000),
    //bestRatioEver(0),
    session(QDateTime::currentDateTime())
{
    if(ok)
    {
        loadProblems(trainingSetJson, ok);
    }
    if(ok)
    {
        loadBrains(brainJson, ok);
    }
    saveDirectory = Util::getLineFromConf("pathToBrains")
            + "/"
            + QString::number(session.date().year())
            + "_"
            + QString::number(session.date().month())
            + "_"
            + QString::number(session.date().day())
            + "-"
            + QString::number(session.time().hour())
            + "_"
            + QString::number(session.time().minute())
            + "_"
            + QString::number(session.time().second());
    QDir directory;
    directory.mkdir(saveDirectory);
}

Job::~Job()
{

}


void Job::start()
{
    for(int i = 0 ; i < brains.size() ; i++)
    {
        brains[i]->start(&problems);
    }
}

void Job::stop()
{
    for(int i = 0 ; i < brains.size() ; i++)
    {
        brains[i]->stop();
    }
    for(int i = 0 ; i < brains.size() ; i++)
    {
        while (!brains[i]->isFinished())
        {
            QThread::msleep(10);
        }
        delete brains[i];
    }
}


void Job::loadProblems(const QString & trainingSetJson, bool & ok)
{
    //
    QJsonDocument json;
    QJsonArray problemsJsonArray;
    problems.clear();
    int minPonyCount = 1;
    int maxPonyCount = 20;
    //
    if(ok)
    {
        minPonyCount = Util::getLineFromConf("minPonyCount", &ok).toInt();
        maxPonyCount = Util::getLineFromConf("maxPonyCount", &ok).toInt();
    }
    if(ok)
    {
        json = QJsonDocument::fromJson(trainingSetJson.toUtf8());
        if(json.isEmpty())
        {
            ok = false;
            Util::writeError("can't load training set");
        }
    }
    if(ok)
    {
        problemsJsonArray = json.object()["problems"].toArray();
    }
    if(ok)
    {

    }
    if(ok)
    {
        for(int i = 0 ; i < problemsJsonArray.size() ; i++)
        {
            problems.push_back(new Problem(
                                   problemsJsonArray[i].toObject(), inputCount));
        }
        for(int i = 0 ; i < problems.size() ; i++)
        {
            if(!(minPonyCount <= problems.at(i)->getCount()
                 && problems.at(i)->getCount() <= maxPonyCount))
            {
                problems.removeAt(i);
                i--;
            }
        }
        Util::write("Working on "
                    + QString::number(problems.size())
                    + " problems after filtering");
    }
}


void Job::loadBrains(const QString & brainJson, bool & ok)
{
    QJsonDocument jsonDoc;
    // Check Json validity
    if(ok)
    {
        jsonDoc = QJsonDocument::fromJson(brainJson.toUtf8());
        if(jsonDoc.isEmpty())
        {
            ok = false;
            Util::writeError("the Json for the brain is unvalid");
        }
    }
    // Add brains
    if(ok)
    {
        brains.clear();
        for(int i = 0 ; i < brainCount ; i++)
        {
            brains.push_back(new Brain(this, i+1, jsonDoc.object(),
                                       qrand(), mode));
        }
        //
        copyToBestBrain(brains[0]);
        copyToBestBrainEver(brains[0]);
        mutexBestBrain.lock();
        bestBrain.json = brains[0]->json;
        mutexBestBrain.unlock();
        mutexBestBrainEver.lock();
        bestBrainEver.json = brains[0]->json;
        mutexBestBrainEver.unlock();
    }
}


void Job::evaluate(Brain * brain)
{
    addBalance(brain->getBalance());
    addRatio(brain->getRatio());
    mutexAverageRatio.lock();
    float averageBalanceTmp = averageBalance;
    mutexAverageRatio.unlock();
    if(brain->getBalance() > averageBalanceTmp)
    {
        copyToBestBrain(brain);
    }
    if(brain->getBalance() > bestBalanceEver)
    {
        copyToBestBrainEver(brain);
        bestBalanceEver = brain->getBalance();
        QString fileName = saveDirectory + "/" +
                QString::number(brain->getBalance(),'f',2) + " | " +
                QString::number(brain->getRatio(),'f',2) + ".brain";
        saveBestBrain(fileName);
    }
    /*float averagetmp = averageRatio;
    if(brain->getRatio() > averagetmp)
    {
        copyToBestBrain(brain);
    }
    if(brain->getRatio() > bestRatioEver)
    {
        bestRatioEver = brain->getRatio();
        QDir dir(Util::getLineFromConf("pathToBrains"));
        QString fileName = Util::getLineFromConf("pathToBrains") + "/" + QString::number(dir.count()) +"_" + QString::number(brain->getRatio()) + ".brain";
        saveBestBrain(fileName);
    }*/
    copyFromBestBrain(brain);
    brain->mutate(mutationFrequency,mutationIntensity);
}


void Job::saveBestBrain(const QString & fileName)
{
    //
    bool ok = true;
    QFile file(fileName);
    //
    if(ok && !file.open(QFile::WriteOnly))
    {
        ok = false;
        Util::writeError("Error : can't open file "
                         + QFileInfo(file).absoluteFilePath());
    }
    //
    if(ok)
    {
        mutexBestBrain.lock();
        file.write(bestBrain.getJson().toUtf8());
        mutexBestBrain.unlock();
    }
}


QString Job::getPrediction(QString problemsJson, QString brainJson, bool & ok, Brain::Mode mode)
{
    QJsonDocument json;
    QJsonArray problemsJsonArray;
    QVector<Problem*> problems;
    QString prediction;
    // Brain
    Brain brain(nullptr, 0,
                QJsonDocument::fromJson(brainJson.toUtf8()).object(), qrand(), mode);
    // Problems
    if(ok)
    {
        json = QJsonDocument::fromJson(problemsJson.toUtf8());
        if(json.isEmpty())
        {
            ok = false;
            Util::writeError("can't load problems");
        }
    }
    if(ok)
    {
        problemsJsonArray = json.object()["problems"].toArray();
    }
    if(ok)
    {
        problems.clear();
        for(int i = 0 ; i < problemsJsonArray.size() ; i++)
        {
            problems.push_back(new Problem(problemsJsonArray[i].toObject(),
                                           inputCount));
        }
    }
    // Compute
    if(ok)
    {
        prediction = "{ ";
        for(int i = 0 ; i < problems.size() ; i++)
        {
            if(i)
            {
                prediction += " ; ";
            }
            prediction += (brain.getPrediction(problems[i]->getInputs()));
        }
        prediction += " }";
    }
    return prediction;
}


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

float Job::getBestRatio()
{
    mutexBestBrain.lock();
    float ratio = bestBrain.getRatio();
    mutexBestBrain.unlock();
    return ratio;
}

float Job::getBestBalance()
{
    mutexBestBrain.lock();
    float balance = bestBrain.getBalance();
    mutexBestBrain.unlock();
    return balance;
}

float Job::getBestBalanceEver()
{
    mutexBestBrainEver.lock();
    float balance = bestBrainEver.getBalance();
    mutexBestBrainEver.unlock();
    return balance;
}

float Job::getAverageBalance()
{
    mutexAverageRatio.lock();
    float balance = averageBalance;
    mutexAverageRatio.unlock();
    return balance;
}


float Job::getAverageRatio()
{
    mutexAverageRatio.lock();
    float ratio = averageRatio;
    mutexAverageRatio.unlock();
    return ratio;
}


QString Job::getBestBrain()
{
    mutexBestBrain.lock();
    QString str = bestBrain.getJson();
    mutexBestBrain.unlock();
    return str;
}

void Job::setBrain(const QString & brainJson)
{
    bool ok = true;
    // Stop
    stop();
    //
    if(ok)
    {
        loadBrains(brainJson, ok);
    }
    //
    if(ok)
    {
        start();
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

void Job::addBalance(const float &balance)
{
    mutexLastNratios.lock();
    lastNbalances.push_back(balance);
    while(lastNbalances.size() > ratiosToSaveCount)
        lastNbalances.pop_front();
    mutexLastNratios.unlock();
    updateAverageRatio();
}


void Job::updateAverageRatio()
{
    mutexLastNratios.lock();
    mutexAverageRatio.lock();
    lastAverageRatio = averageRatio;
    lastAverageBalance = averageBalance;
    averageRatio = 0.0f;
    averageBalance = 0.0f;
    for(int i = 0 ; i<lastNratios.size() ; i++)
    {
        averageRatio += lastNratios[i];
    }
    for(int i = 0 ; i<lastNbalances.size() ; i++)
    {
        averageBalance += lastNbalances[i];
    }
    averageRatio /= (float)lastNratios.size();
    averageBalance /= (float)lastNbalances.size();
    float ratioResolution = 0.00001f;
    float balanceResolution = 0.01f;
    float borne = 1000.0f;
    //if(lastAverageRatio > averageRatio - ratioResolution)
    if(lastAverageBalance > averageBalance - balanceResolution)
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
    if(brain->balance < bestBalanceEver - limitDeviation)
    {
        copyToBestBrain(&bestBrainEver);
    }
    else
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
        bestBrain.results.clear();
        bestBrain.score = brain->score;
        bestBrain.attempts = brain->attempts;
        bestBrain.balance = brain->balance;
        bestBrain.ratio = brain->ratio;
        mutexBestBrain.unlock();
    }
}


void Job::copyToBestBrainEver(Brain * brain)
{
    mutexBestBrainEver.lock();
    bestBrainEver.id = 0;
    bestBrainEver.inputCount = brain->inputCount;
    bestBrainEver.outputCount = brain->outputCount;
    bestBrainEver.weights = brain->weights;
    bestBrainEver.outputs = QVector<float>(brain->outputCount, 0.0f);
    bestBrainEver.neurons = QVector<Neuron>();
    bestBrainEver.inputs = QVector<float>(brain->inputCount, 0.0f);
    bestBrainEver.neuronBlueprints = brain->neuronBlueprints;
    bestBrainEver.results.clear();
    bestBrainEver.score = brain->score;
    bestBrainEver.attempts = brain->attempts;
    bestBrainEver.balance = brain->balance;
    bestBrainEver.ratio = brain->ratio;
    mutexBestBrainEver.unlock();
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
    brain->results.clear();
    brain->score = 0;
    brain->attempts = 0;
    brain->ratio = 0;
    brain->balance = 0;
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
