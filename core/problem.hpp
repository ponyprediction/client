#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <QXmlStreamReader>
#include <QVector>

class Problem
{
    public:
        Problem();
        Problem(QXmlStreamReader & xmlReader);
        ~Problem();
        const QVector<float> & getInputs(){return inputs;}
        const int & getWantedOutput(){return wantedOutputs[0];}
    private:
        void load(QXmlStreamReader & xmlReader);
        QVector<float> inputs;
        QVector<int> wantedOutputs;
};

#endif // PROBLEM_HPP
