#include "job.hpp"

#include <QXmlStreamReader>
#include <QDebug>

Job::Job()
{

}

Job::Job(const int & id,
         const QString & problemsXml,
         const QString & brainXml,
         const int & brainCount,
         const int & interval) :
    id(id),
    problems(),
    BestBrain(),
    brains(),
    brainCount(brainCount),
    interval(interval)
{
    loadProblems(problemsXml);
    loadBrains(brainXml);
}

Job::~Job()
{

}

void Job::start()
{
    for(int i = 0 ; i < brainCount ; i++)
        brains[i]->start(&problems);
}

void Job::loadProblems(const QString & problemsXml)
{
    problems.clear();
    QXmlStreamReader xmlReader(problemsXml);
    while (!xmlReader.atEnd())
    {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if(token == QXmlStreamReader::StartElement)
        {
            if(xmlReader.name() == "problem")
            {
                problems.push_back(new Problem(xmlReader));
            }
        }
    }
}

void Job::loadBrains(const QString & brainXml)
{
    QXmlStreamReader xmlReader(brainXml);
    brains.clear();
    for(int i = 0 ; i < brainCount ; i++)
        brains.push_back(new Brain(xmlReader));
}
