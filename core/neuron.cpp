#include "neuron.hpp"
#include <QDebug>
#include <math.h>
Neuron::Neuron()
{

}

Neuron::~Neuron()
{

}

void Neuron::addExternalInput(float * input)
{
    externalInputs.push_back(input);
}

void Neuron::addNeuronalInput(float * input)
{
    neuronalInputs.push_back(input);
}

void Neuron::addWeight(float * weight)
{
    weights.push_back(weight);
}

void Neuron::compute()
{
    float absoluteWeight = 0.0f;
    output = 0.0f;
    QVector<float*> inputs = externalInputs + neuronalInputs + brainalInputs;  
    for(int i = 0 ; i < inputs.size() && i <weights.size(); i++)
    {
        output += (*inputs[i]) * (*weights[i]);
        //absoluteWeight += fabs(*weights[i]);
    }
    output = 1/(1+exp(output));
    //output /= absoluteWeight;
}
