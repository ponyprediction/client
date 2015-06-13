#include <QApplication>
#include "ui/main-window.hpp"
#include "core/util.hpp"
#include <time.h>
#include <QTime>


int main(int argc, char *argv[])
{
    QTime time = QTime::currentTime();
    unsigned int seed = time.msec();
    qsrand(seed);
    //
    QApplication a(argc, argv);
    MainWindow w;
    Util::init(&w);
    w.show();
    //
    return a.exec();
}
