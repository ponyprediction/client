#include "neuron.hpp"
#include "util.hpp"
#include <QDebug>
#include <math.h>


Neuron::Neuron() :
    inOutputLayer(false)
{
    biasInputs.push_back(new float(1));
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
    bool ok = true;
    float absoluteWeight = 0.0f;
    output = 0.0f;
    QVector<float*> inputs = biasInputs + externalInputs + neuronalInputs + brainalInputs;
    if(ok && inputs.size() != weights.size())
    {
        Util::writeError("Not same count of weights and inputs");
        ok = false;
    }
    if(ok)
    {
        float x;
        for(int i = 0 ; i < inputs.size() && i < weights.size(); i++)
        {
            x += (*inputs[i]) * (*weights[i]);
            absoluteWeight += fabs(*weights[i]);
        }
        if(absoluteWeight)
        {
            output = x / absoluteWeight;
        }
        else
        {
            output = 0;
        }
        if(output > 1)
        {
            output = 1;
        }
        if(output < -1)
        {
            output = -1;
        }
    }
}
