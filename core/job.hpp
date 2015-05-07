#pragma once

#include <QVector>
#include <QMutex>
#include "brain.hpp"
#include "core/problem.hpp"

class Job : public QObject
{
    public:

        Job();
        Job(const int & id,
            const QString & problemsXml,
            const QString & brainXml,
            const int & brainCount,
            const int & interval);
        ~Job();
        void start();

        void evaluate(Brain * brain);

        void setMutationFrequencyAuto(bool v){mutationFrequencyAuto = v;}
        void setMutationFrequency(float v){mutationFrequency = v;}
        float getMutationFrequency(){return mutationFrequency;}

        void setMutationIntensityAuto(bool v){mutationIntensityAuto = v;}
        void setMutationIntensity(float v){mutationIntensity = v;}
        float getMutationIntensity(){return mutationIntensity;}

    private:

        void loadProblems(const QString & problemsXml);
        void loadBrains(const QString & brainXml);
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
        int interval;

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
