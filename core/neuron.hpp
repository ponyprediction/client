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
    float * getOutputAdress(){return & output;}
    void setInOutputLayer(bool const & b){inOutputLayer = b;}

private:

    QVector<float *> biasInputs;
    QVector<float *> externalInputs;
    QVector<float *> neuronalInputs; // = outputs from neuron
    QVector<float *> brainalInputs; // = outputs from brain
    QVector<float *> weights; // = weights
    float output;
    bool inOutputLayer;
};
