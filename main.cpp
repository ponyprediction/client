#include <QApplication>
#include "ui/main-window.hpp"
#include "core/util.hpp"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  Util::init(&w);
  w.show();
  return a.exec();
}
