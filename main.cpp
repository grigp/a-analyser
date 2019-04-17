#include "mainwindow.h"
#include <QApplication>
#include <aanalyserapplication.h>

int main(int argc, char *argv[])
{
    AAnalyserApplication a(argc, argv);
    MainWindow w;
    a.setMainWindow(&w);
    w.show();

    return a.exec();
}
