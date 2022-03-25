#include "kerdoapplication.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    KerdoApplication a(argc, argv);
    MainWindow w;
    a.setMainWindow(&w);
    w.showMaximized();

    return a.exec();
}
