#pragma once

#include <QThread>
#include <QVector>
#include <QThread>
#include "core/neuron.hpp"
#include "core/neuron-blueprint.hpp"
#include "core/problem.hpp"

class Job;

class Brain : public QThread
{
        friend class Job;
        Q_OBJECT
    public:
        Brain();
        Brain(Job * job, const int & id, QXmlStreamReader & xmlReader);
        ~Brain();
        void start(QVector<Problem*> * problems);
        const float & getRatio(){return ratio;}
        const int & getId(){return id;}
        void mutate(float mutationFrequency, float mutationIntensity);
    private:
        void compute(const QVector<float> & inputs);
        void prepareResult();
        void learn(const int & wantedResult);
        void autoEvaluate();
        void run();
        void load(QXmlStreamReader & xmlReader);
        void initNeurons();
        Job * job;
        int neuronCount;
        int inputCount;
        int weightCount;
        int outputCount;
        int jobId;

        QVector<Neuron> neurons;
        QVector<NeuronBlueprint> neuronBlueprints;

        QVector<float> inputs;
        QVector<float> weights;
        QVector<float> outputs;

        QVector<Problem*> * problems;

        bool go;
        int currentProblemId;
        int id;

        int result; // = winnnerId

        float attempts;
        float score;
        float ratio;
};
