#ifndef CONNECTION_H
#define CONNECTION_H
#include "core/neuron.hpp"

class Connection
{
public:
    Connection();
    ~Connection();
    Neuron * previous;
    Neuron * next;
    float *  weights;
    float tsigmaK;
};

#endif // CONNECTION_H
