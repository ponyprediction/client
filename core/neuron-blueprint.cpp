#include "neuron-blueprint.hpp"
#include <QDebug>

NeuronBlueprint::NeuronBlueprint()
{

}

NeuronBlueprint::NeuronBlueprint(const QJsonObject & json)
{
    externalInputIds.clear();
    neuronalInputIds.clear();
    brainalInputIds.clear();
    weightIds.clear();
    // externalInputIds
    {
        QString str = json["externalInputIds"].toString();
        if(str.size())
        {
            QStringList strList = str.split(';');
            for(int i = 0 ; i < strList.size() ; i++)
                addExternalInputId(strList[i].toInt());
        }
    }
    // neuronalInputIds
    {
        QString str = json["neuronalInputIds"].toString();
        if(str.size())
        {
            QStringList strList = str.split(';');
            for(int i = 0 ; i < strList.size() ; i++)
                addNeuronalInputId(strList[i].toInt());
        }
    }
    // brainalInputIds
    {
        QString str = json["brainalInputIds"].toString();
        if(str.size())
        {
            QStringList strList = str.split(';');
            for(int i = 0 ; i < strList.size() ; i++)
                addBrainalInputId(strList[i].toInt());
        }
    }
    // weightIds
    {
        QString str = json["weightIds"].toString();
        if(str.size())
        {
            QStringList strList = str.split(';');
            for(int i = 0 ; i < strList.size() ; i++)
                addWeightId(strList[i].toInt());
        }
    }
}

NeuronBlueprint::~NeuronBlueprint()
{

}

/*void NeuronBlueprint::writeToXML(QXmlStreamWriter & xml)
{
    xml.writeStartElement("neuron");
    {
        QString str1 = "";
        QString str2 = "";
        QString str3 = "";
        QString str4 = "";
        if(externalInputIds.size())
        {
            str1 = QString::number(externalInputIds[0]);
            for(int i = 1 ; i < externalInputIds.size() ;  i++)
                str1 += ";" + QString::number(externalInputIds[i]);
        }
        if(neuronalInputIds.size())
        {
            str2 = QString::number(neuronalInputIds[0]);
            for(int i = 1 ; i < neuronalInputIds.size() ;  i++)
                str2 += ";" + QString::number(neuronalInputIds[i]);
        }
        if(brainalInputIds.size())
        {
            str3 = QString::number(brainalInputIds[0]);
            for(int i = 1 ; i < brainalInputIds.size() ;  i++)
                str3 += ";" + QString::number(brainalInputIds[i]);
        }
        if(weightIds.size())
        {
            str4 = QString::number(weightIds[0]);
            for(int i = 1 ; i < weightIds.size() ;  i++)
                str4 += ";" + QString::number(weightIds[i]);
        }
        xml.writeTextElement("external-input-ids", str1);
        xml.writeTextElement("neuronal-input-ids", str2);
        xml.writeTextElement("brainal-input-ids", str3);
        xml.writeTextElement("weight-ids", str4);
    }
    xml.writeEndElement();
}*/

