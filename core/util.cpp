#include "util.hpp"
#include "ui/main-window.hpp"
#include <QFile>
#include <QDir>
#include <QXmlStreamReader>
#include <iostream>


MainWindow * Util::mainWindow = nullptr;
QString Util::configFileName = "./client.conf";

bool Util::writeEnabled = true;
bool Util::overwriteEnabled = true;
bool Util::successEnabled = true;
bool Util::successOverwriteEnabled = true;
bool Util::warningsEnabled = true;
bool Util::warningOverwriteEnabled = true;
bool Util::errorEnabled = true;
bool Util::errorOverwriteEnabled = true;
bool Util::isOverwriting = false;


void Util::init(MainWindow * mainWindow)
{
    bool ok = true;
    Util::mainWindow = mainWindow;
    // check folders
    QStringList paths;
    paths << Util::getLineFromConf("pathToBrains");
    paths << Util::getLineFromConf("pathToTrainingSets");
    paths << Util::getLineFromConf("pathToProblems");
    foreach(QString path, paths)
    {
        QDir dir(path);
        if (!dir.exists()) {
            ok = dir.mkpath(".");
        }
        if(!ok)
        {
            ok = false;
            Util::writeError("cannot create " + path);
            break;
        }
    }
}


QString Util::getLineFromConf(const QString &id)
{
    QString output = "";
    QFile file(configFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Util::writeError("Can not find the conf file at : "
                         + QFileInfo(file).absoluteFilePath());
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
        Util::writeError("Can not find '" + id + "' in conf file.");
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


void Util::write(const QString & message)
{
    if(isOverwriting)
    {
        overwrite("");
        isOverwriting = false;
    }
    std::cout << message.toStdString() << std::endl;
    if(mainWindow)
    {
        emit mainWindow->newLog(message);
    }
}


void Util::writeSuccess(const QString &message)
{
    if(successEnabled)
        write(VERT + message + RESET);
}


void Util::writeWarning(const QString &message)
{
    if(warningsEnabled)
        write(QString(JAUNE) + "Warning : " + message + RESET);
}


void Util::writeError(const QString & message)
{
    if(errorEnabled)
        write(QString(ROUGE) + "Error : " + message + RESET);
}


void Util::overwrite(const QString &message)
{
    if(overwriteEnabled)
    {
        isOverwriting = true;
        std::cout << '\r';
        for(int i = 0 ; i < 80 ; i++)
        {
            std::cout << ' ';
        }
        std::cout << '\r' << message.toStdString() << std::flush;
    }
    else
    {
        write(message);
    }
}


void Util::overwriteSuccess(const QString &message)
{
    if(successEnabled && successOverwriteEnabled)
        overwrite(QString(VERT) + message + QString(RESET));
    else
        writeSuccess(message);
}


void Util::overwriteWarning(const QString &message)
{
    if(warningsEnabled && warningOverwriteEnabled)
        overwrite(QString(JAUNE)  +  "Warning : " + message + QString(RESET));
    else
        writeWarning(message);
}


void Util::overwriteError(const QString &message)
{
    if(warningsEnabled && warningOverwriteEnabled)
        overwrite(QString(JAUNE)  +  "Error : " + message + QString(RESET));
    else
        writeWarning(message);
}
