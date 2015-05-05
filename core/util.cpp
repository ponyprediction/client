#include "util.hpp"
#include "main-window.hpp"


MainWindow * Util::mainWindow = nullptr;

void Util::init(MainWindow * mainWindow)
{
  Util::mainWindow = mainWindow;
}

void Util::addLog(const QString & message)
{
  if(mainWindow)
    mainWindow->addLog(message);
}
