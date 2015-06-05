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
private:
    void load(const QJsonObject & json, const int & inputCount);
    QVector<float> inputs;
    QVector<int> wantedOutputs;
};

#endif // PROBLEM_HPP
