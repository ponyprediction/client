#include "util.hpp"
#include "main-window.hpp"
#include <QFile>
#include <QDir>
#include <QXmlStreamReader>

MainWindow * Util::mainWindow = nullptr;
QString Util::configFilePath = "./conf.xml";

void Util::init(MainWindow * mainWindow)
{
    Util::mainWindow = mainWindow;
}

void Util::addLog(const QString & message)
{
    if(mainWindow)
        mainWindow->addLog(message);
}

QString Util::getLineFromConf(const QString &id)
{
    QString output = "";
    QFile file(configFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can not find the conf file in : " + QDir::currentPath();
        return QString();
    }
    QXmlStreamReader xml(&file);
    while (!xml.atEnd())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement)
        {
            if(xml.name() == id)
            {
                output = xml.readElementText();
            }
        }
    }
    return output;
}
