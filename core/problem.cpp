#include "problem.hpp"
#include "util.hpp"
#include <QXmlStreamReader>
#include <QStringList>
#include <QDebug>
#include <QMap>
#include <QJsonArray>
#include <QJsonObject>


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
    // Inputs
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
    // Wanted outputs
    QStringList wantedOutputsList = json["wantedOutputs"].toString().split(';');
    for(int i = 0 ; i < wantedOutputsList.size() ; i++)
    {
        wantedOutputs.push_back(wantedOutputsList[i].toInt());
    }
    // Winnings
    QJsonObject win = json["winnings"].toObject();
    QJsonArray singleShow = win["singleShow"].toArray();
    for(int i = 0 ; i< singleShow.size() ; i++)
    {
        winnings.insert(singleShow[i].toObject()["id"].toString().toInt(),
                singleShow[i].toObject()["winning"].toString().toDouble());
    }
    // Targets
    QStringList targetsList = json["targets"].toString().split(';');
    for(int i = 0 ; i < targetsList.size() ; i++)
    {
        targets.push_back(targetsList[i].toInt());
    }
}

