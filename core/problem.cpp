#include "problem.hpp"

#include <QXmlStreamReader>
#include <QStringList>
#include <QDebug>

Problem::Problem()
{

}

Problem::Problem(QXmlStreamReader & xmlReader)
{
    load(xmlReader);
}

Problem::~Problem()
{

}

void Problem::load(QXmlStreamReader & xmlReader)
{
    while (!xmlReader.atEnd()
           && !(xmlReader.name() == "problem"
                && xmlReader.tokenType() == QXmlStreamReader::EndElement))
    {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if(token == QXmlStreamReader::StartElement)
        {
            if(xmlReader.name() == "inputs")
            {
                QString str = xmlReader.readElementText();
                QStringList list = str.split(';');
                for(int i = 0 ; i < list.size() ; i++)
                    inputs.push_back(list[i].toFloat());
            }
            if(xmlReader.name() == "wantedOutputs")
            {
                QString str = xmlReader.readElementText();
                QStringList list = str.split(';');
                for(int i = 0 ; i < list.size() ; i++)
                    wantedOutputs.push_back(list[i].toInt());
            }
        }
    }
}

