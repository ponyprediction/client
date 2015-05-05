#pragma once

#include <QString>

class MainWindow;

class Util
{
  public:
    static void init(MainWindow * mainWindow);
    static void addLog(const QString & message);
    static QString getLineFromConf(const QString &id);
  private:
    static MainWindow * mainWindow;
    static QString configFilePath;
};
