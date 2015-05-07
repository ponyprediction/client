#pragma once

#include <QVector>
#include <QMutex>
#include "brain.hpp"
#include "core/problem.hpp"

class Job
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

    private:
        void loadProblems(const QString & problemsXml);
        void loadBrains(const QString & brainXml);
        void addRatio(const float & ratio);
        void updateAverageRatio();
        void copyToBestBrain(Brain * brain);
        void copyFromBestBrain(Brain * brain);
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
        float mutationIntensity;
};
