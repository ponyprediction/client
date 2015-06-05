#pragma once

#include <QVector>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QJsonObject>

class NeuronBlueprint
{
public:
    NeuronBlueprint();
    NeuronBlueprint(const QJsonObject & json);
    ~NeuronBlueprint();

    void addExternalInputId(const int & inputId){
        externalInputIds.push_back(inputId);}
    void addNeuronalInputId(const int &  inputId){
        neuronalInputIds.push_back(inputId);}
    void addBrainalInputId(const int &  inputId){
        brainalInputIds.push_back(inputId);}
    void addWeightId(const int &  weightId){
        weightIds.push_back(weightId);}
    //private:
    QVector<int> externalInputIds;
    QVector<int> neuronalInputIds;
    QVector<int> brainalInputIds;
    QVector<int> weightIds;
};
