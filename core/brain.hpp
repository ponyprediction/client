#pragma once


#include "core/neuron.hpp"
#include "core/neuron-blueprint.hpp"
#include "core/problem.hpp"
#include <QJsonObject>
#include <QThread>
#include <QVector>
#include <QThread>

class Job;

class Brain : public QThread
{

    friend class Job;
    Q_OBJECT

public:

    Brain();
    Brain(Job * job, const int & id, const QJsonObject & json, const int & seed);
    ~Brain();
    void start(QVector<Problem*> * problems);
    const float & getRatio(){return ratio;}
    const int & getId(){return id;}
    void mutate(float mutationFrequency, float mutationIntensity);
    QString getJson();

    QString getPrediction(const QVector<float> & inputs);

    void stop();

private:

    void compute(const QVector<float> & inputs);
    void prepareResults();
    void learn(const int & wantedResult);
    void learnSingleShow(const QVector<int> & wantedResults, const int & count);
    void autoEvaluate();
    void run();
    void load(const QJsonObject & json);
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

    QVector<float> results; // = winnnerId

    float attempts;
    float score;
    float ratio;

    QJsonObject json;

    int seed;

};
