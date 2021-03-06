#pragma once

#include "brain.hpp"
#include "core/problem.hpp"
#include <QVector>
#include <QMutex>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>


class Job : public QObject
{
    Q_OBJECT

public:

    enum EvaluationMode {RATIO, BALANCE, ERROR};

    Job();
    Job(const QString & id,
        const QString & trainingSetJson,
        const QString & brainJson,
        const int & brainCount,
        bool & ok,
        const Brain::Mode & mode);
    ~Job();

    void start();
    void stop();
    void evaluate(Brain * brain);
    void saveBestBrain(const QString & fileName);

    QString static getPrediction(QString problemsJson,
                                 QString brainJson,
                                 bool & ok,
                                 Brain::Mode mode);

    void setMutationFrequencyAuto(bool v){mutationFrequencyAuto = v;}
    void setMutationFrequency(float v);
    void setMutationFrequencyDown(float v){mutationFrequencyDown = v;}
    void setMutationFrequencyUp(float v){mutationFrequencyUp = v;}
    void setMutationFrequencyMin(float v);
    void setMutationFrequencyMax(float v);
    bool getMutationFrequencyAuto(){return mutationFrequencyAuto;}
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
    bool getMutationIntensityAuto(){return mutationIntensityAuto;}
    float getMutationIntensity(){return mutationIntensity;}
    float getMutationIntensityDown(){return mutationIntensityDown;}
    float getMutationIntensityUp(){return mutationIntensityUp;}
    float getMutationIntensityMin(){return mutationIntensityMin;}
    float getMutationIntensityMax(){return mutationIntensityMax;}

    void setLimitDeviation(float v){limitDeviation = v;}
    float getLimitDeviation(){return limitDeviation;}

    float getBestRatio();
    float getAverageRatio();
    float getBestBalance();
    float getAverageBalance();
    float getBestError();
    float getAverageError();

    float getBestBalanceEver();
    float getBestErrorEver();

    QString getBestBrain();
    void setBrain(const QString & brainJson);

    QString getId(){return id;}

private:

    void loadProblems(const QString & trainingSetJson, bool & ok);
    void loadBrains(const QString & brainJson, bool & ok);

    void addRatio(const float & ratio);
    void updateAverageRatio();
    void addBalance(const float & balance);
    void updateAverageBalance();
    void addError(const float & error);
    void updateAverageError();

    void copyToBestBrain(Brain * brain);
    void copyFromBestBrain(Brain * brain);
    void copyToBestBrainEver(Brain * brain);

    void upMutationFrequency();
    void downMutationFrequency();
    void upMutationIntenstity();
    void downMutationIntenstity();

    void upMutation();
     void downMutation();

    QString id;
    QVector<Problem*> problems;
    Brain bestBrain;
    Brain bestBrainEver;
    QVector<Brain*> brains;
    int brainCount;
    QMutex mutexBestBrain;
    QMutex mutexBestBrainEver;

    int infosToSaveCount;

    QVector<float> lastNratios;
    float averageRatio;
    float lastAverageRatio;
    QMutex mutexLastNratios;
    QMutex mutexAverageRatio;
    //float bestRatioEver;

    QVector<float> lastNbalances;
    float averageBalance;
    float lastAverageBalance;
    QMutex mutexLastNbalances;
    QMutex mutexAverageBalance;
    float bestBalanceEver;

    QVector<float> lastNError;
    float averageError;
    float lastAverageError;
    QMutex mutexLastNError;
    QMutex mutexAverageError;
    float bestErrorEver;



    QDateTime session;
    QString saveDirectory;

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

    float limitDeviation;

    Brain::Mode mode;

    EvaluationMode evaluationMode;

};
