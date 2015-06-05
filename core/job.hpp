#pragma once

#include <QVector>
#include <QMutex>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "brain.hpp"
#include "core/problem.hpp"

class Job : public QObject
{
    Q_OBJECT
public:

    Job();
    Job(const int & id,
        const QString & trainingSetJson,
        const QString & brainJson,
        const int & brainCount);
    ~Job();

    void start();
    void evaluate(Brain * brain);
    void saveBestBrain(const QString & fileName);

    QString static getPrediction(QString problemsJson, QString brainJson);

    void setMutationFrequencyAuto(bool v){mutationFrequencyAuto = v;}
    void setMutationFrequency(float v);
    void setMutationFrequencyDown(float v){mutationFrequencyDown = v;}
    void setMutationFrequencyUp(float v){mutationFrequencyUp = v;}
    void setMutationFrequencyMin(float v);
    void setMutationFrequencyMax(float v);
    float getMutationFrequency(){return mutationFrequency;}
    float getMutationFrequencyDown(){return mutationFrequencyDown;}
    float getMutationFrequencyUp(){return mutationFrequencyUp;}
    float getMutationFrequencyMin(){return mutationFrequencyMin;}
    float getMutationFrequencyMax(){return mutationFrequencyMax;}

    void setMutationIntensityAuto(bool v){mutationIntensityAuto = v;}
    void setMutationIntensity(float v);
    void setMutationIntensityDown(float v){mutationIntensityDown = v;}
    void setMutationIntensityUp(float v){mutationIntensityUp = v;}
    void setMutationIntensityMin(float v);
    void setMutationIntensityMax(float v);
    float getMutationIntensity(){return mutationIntensity;}
    float getMutationIntensityDown(){return mutationIntensityDown;}
    float getMutationIntensityUp(){return mutationIntensityUp;}
    float getMutationIntensityMin(){return mutationIntensityMin;}
    float getMutationIntensityMax(){return mutationIntensityMax;}

private:

    void loadProblems(const QString & trainingSetJson);
    void loadBrains(const QString & brainJson);
    void addRatio(const float & ratio);
    void updateAverageRatio();
    void copyToBestBrain(Brain * brain);
    void copyFromBestBrain(Brain * brain);

    void upMutationFrequency();
    void downMutationFrequency();
    void upMutationIntenstity();
    void downMutationIntenstity();

    int id;
    QVector<Problem*> problems;
    Brain bestBrain;
    QVector<Brain*> brains;
    int brainCount;

    int ratiosToSaveCount;
    QVector<float> lastNratios;
    float averageRatio;
    float lastAverageRatio;

    QMutex mutexLastNratios;
    QMutex mutexAverageRatio;
    QMutex mutexBestBrain;

    float mutationFrequency;
    bool mutationFrequencyAuto;
    float mutationFrequencyUp;
    float mutationFrequencyDown;
    float mutationFrequencyMax;
    float mutationFrequencyMin;

    float mutationIntensity;
    bool mutationIntensityAuto;
    float mutationIntensityUp;
    float mutationIntensityDown;
    float mutationIntensityMax;
    float mutationIntensityMin;

};
