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

    if(inputs.size() != weights.size())
    {
        qDebug() <<"problem" << inputs.size() << weights.size();
    }
    for(int i = 0 ; i < inputs.size() && i <weights.size(); i++)
    {
        /*if(!((*inputs[i]) <= 1 && (*inputs[i]) >= -1))
      qDebug() << "Inputs value out of range [-1;1] : " << (*inputs[i]);
    if(!((*weights[i]) <= 1 && (*weights[i]) >= -1))
      qDebug() << "Weight value out of range [-1;1] : " << (*weights[i]);*/
        output += (*inputs[i]) * (*weights[i]);
        absoluteWeight += fabs(*weights[i]);
    }
    output /= absoluteWeight;
    /*if(!(output <= 1 && output >= -1))
    qDebug() << "Output value out of range [-1;1] : " << output;*/

    /*QString aa = "";
  for (int i = 0 ; i < inputs.size() ; i++)
    aa += QString::number(*inputs[i]) +  " | ";
    qDebug() << aa << output;*/
}
