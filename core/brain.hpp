#pragma once

#include <QThread>
#include <QVector>
#include "core/neuron.hpp"
#include "core/neuron-blueprint.hpp"
#include "core/problem.hpp"

class Job;

class Brain /*: public QThread*/
{
    public:
        Brain();
        Brain(QXmlStreamReader & xmlReader);
        ~Brain();
        void start(QVector<Problem*> * problems);
    private:
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
};
