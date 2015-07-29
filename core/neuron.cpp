#include "neuron.hpp"
#include "util.hpp"
#include <QDebug>
#include <math.h>
Neuron::Neuron()
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
        for(int i = 0 ; i < inputs.size() && i < weights.size(); i++)
        {
            output += (*inputs[i]) * (*weights[i]);
            absoluteWeight += fabs(*weights[i]);
        }
        output /= absoluteWeight;
    }
}
