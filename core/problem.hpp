#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <QJsonObject>
#include <QVector>

class Problem
{

public:

    Problem();
    Problem(const QJsonObject & json, const int & inputCount);
    ~Problem();
    const QVector<float> & getInputs(){return inputs;}
    const int & getWantedOutput(){return wantedOutputs[0];}
    const QVector<int> & getWantedOutputs(){return wantedOutputs;}
    const int & getCount(){return count;}

private:

    void load(const QJsonObject & json, const int & inputCount);
    QVector<float> inputs;
    QVector<int> wantedOutputs;
    int count;

};

#endif // PROBLEM_HPP
