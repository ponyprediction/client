#include "problem.hpp"
#include "util.hpp"
#include <QXmlStreamReader>
#include <QStringList>
#include <QDebug>

Problem::Problem()
{

}

Problem::Problem(const QJsonObject & json, const int & inputCount)
{
    load(json, inputCount);
}

Problem::~Problem()
{

}

void Problem::load(const QJsonObject & json, const int & inputCount)
{
    inputs.clear();
    wantedOutputs.clear();
    //
    QStringList inputsList = json["inputs"].toString().split(';');

    int inputsPerTeam = Util::getLineFromConf("inputsPerTeam").toInt();
    teamCount = inputsList.size() / inputsPerTeam;

    for(int i = 0 ; i < inputsList.size() ; i++)
    {
        inputs.push_back(inputsList[i].toFloat());
    }
    for(int i = inputs.size() ; i < inputCount ; i++)
    {
        inputs.push_back(0.0);
    }
    //
    QStringList wantedOutputsList = json["wantedOutputs"].toString().split(';');
    for(int i = 0 ; i < wantedOutputsList.size() ; i++)
    {
        wantedOutputs.push_back(wantedOutputsList[i].toInt());
    }
    QStringList gainsList = json["gain"].toString().split(";");
    for(int i = 0 ; i < gainsList.size() ; i++)
    {
        gains.push_back(gainsList[i].toFloat());
    }
}

