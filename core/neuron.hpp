#pragma once

#include <QVector>

class Neuron
{
public:
    Neuron();
    ~Neuron();
    void addExternalInput(float * input);
    void addNeuronalInput(float * input);
    void addWeight(float * weight);
    void compute();
    float getOutput(){return output;}
    float * getOutputAdress(){return &output;}
    void backPropa(double target);
    double tsigmaK;
    QVector<Neuron *> next;
    QVector<Neuron *> previous;
    QVector<float*> nextWeights;
private:
    QVector<float *> externalInputs;
    QVector<float *> neuronalInputs; // = outputs from neuron
    QVector<float *> brainalInputs; // = outputs from brain
    QVector<float *> weights; // = weights
    float output;
};
