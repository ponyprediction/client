#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <QJsonObject>
#include <QMap>
#include <QVector>

class Problem
{

public:

    Problem();
    Problem(const QJsonObject & json, const int & inputCount);
    ~Problem();
    const QVector<float> & getInputs(){return inputs;}
    const QMap<int, float> getWinnings(){return winnings;}
    const int & getWantedOutput(){return wantedOutputs[0];}
    const QVector<int> & getWantedOutputs(){return wantedOutputs;}
    const int & getCount(){return teamCount;}

private:

    void load(const QJsonObject & json, const int & inputCount);
    QVector<float> inputs;
    QVector<int> wantedOutputs;
    QMap<int, float> winnings;
    int teamCount;

};

#endif // PROBLEM_HPP
