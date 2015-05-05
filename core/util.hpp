#pragma once

#include <QString>

class MainWindow;

class Util
{
  public:
    static void init(MainWindow * mainWindow);
    static void addLog(const QString & message);
  private:
    static MainWindow * mainWindow;
};
