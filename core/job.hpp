#pragma once

#include <QVector>
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
    private:
        void loadProblems(const QString & problemsXml);
        void loadBrains(const QString & brainXml);
        int id;
        QVector<Problem*> problems;
        Brain BestBrain;
        QVector<Brain*> brains;
        int brainCount;
        int interval;
};
