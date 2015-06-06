#include "util.hpp"
#include "ui/main-window.hpp"
#include <QFile>
#include <QDir>
#include <QXmlStreamReader>

MainWindow * Util::mainWindow = nullptr;
QString Util::configFileName = "./client.conf";

void Util::init(MainWindow * mainWindow)
{
    Util::mainWindow = mainWindow;
}

void Util::addLog(const QString & message)
{
    if(mainWindow)
    {
        emit mainWindow->newLog(message);
    }
}

QString Util::getLineFromConf(const QString &id)
{
    QString output = "";
    QFile file(configFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can not find the conf file at : "
                    + QFileInfo(file).absoluteFilePath();
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
    if(!output.size())
    {
        Util::addLog("Can not find '" + id + "' in conf file.");
        qDebug() << "Can not find '" + id + "' in conf file.";
    }
    return output;
}
float Util::getRandomFloat(const float &min, const float &max)
{
    return ((static_cast <float>(qrand()))
            * (max - min)
            / (static_cast<float>(RAND_MAX)))
            + min;
}
int Util::getRandomInt(const int &min, const int &max)
{
    int r = getRandomFloat(min-1, max+1);
    if(r < min || r > max)
        r = getRandomInt(min, max);
    return r;
}
