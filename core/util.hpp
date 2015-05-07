#pragma once

#include <QString>

class MainWindow;

class Util
{
  public:
    static void init(MainWindow * mainWindow);
    static void addLog(const QString & message);
    static QString getLineFromConf(const QString &id);
    static float getRandomFloat(const float & min, const float & max);
    static int getRandomInt(const int & min, const int & max);
  private:
    static MainWindow * mainWindow;
    static QString configFilePath;
};
