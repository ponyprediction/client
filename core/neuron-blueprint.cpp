#include "neuron-blueprint.hpp"
#include <QDebug>

NeuronBlueprint::NeuronBlueprint()
{

}

NeuronBlueprint::NeuronBlueprint(QXmlStreamReader & xmlReader)
{
    externalInputIds.clear();
    neuronalInputIds.clear();
    brainalInputIds.clear();
    weightIds.clear();
    while(!xmlReader.atEnd()
          && !(xmlReader.name() == "neuron"
               && xmlReader.tokenType() == QXmlStreamReader::EndElement))
    {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if(token == QXmlStreamReader::StartElement) // Attributs x 5
        {
            if(xmlReader.name() == "externalInputIds")
            {
                QString str = xmlReader.readElementText();
                if(str.size())
                {
                    QStringList strList = str.split(';');
                    for(int i = 0 ; i < strList.size() ; i++)
                        addExternalInputId(strList[i].toInt());
                }
            }
            if(xmlReader.name() == "neuronalInputIds")
            {
                QString str = xmlReader.readElementText();
                if(str.size())
                {
                    QStringList strList = str.split(';');
                    for(int i = 0 ; i < strList.size() ; i++)
                        addNeuronalInputId(strList[i].toInt());
                }
            }
            if(xmlReader.name() == "brainalInputIds")
            {
                QString str = xmlReader.readElementText();
                if(str.size())
                {
                    QStringList strList = str.split(';');
                    for(int i = 0 ; i < strList.size() ; i++)
                        addBrainalInputId(strList[i].toInt());
                }
            }
            if(xmlReader.name() == "weightIds")
            {
                QString str = xmlReader.readElementText();
                if(str.size())
                {
                    QStringList strList = str.split(';');
                    for(int i = 0 ; i < strList.size() ; i++)
                        addWeightId(strList[i].toInt());
                }
            }
        }
    }
}

NeuronBlueprint::~NeuronBlueprint()
{

}

/*void NeuronBlueprint::writeToXML(QXmlStreamWriter & xml)
{
    /*xml.writeStartElement("neuron");
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

