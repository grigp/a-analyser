#include "mainwindow.h"

#include <aanalyserapplication.h>
#include "exitcodes.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    AAnalyserApplication a(argc, argv);
    MainWindow w;
    a.setMainWindow(&w);
    w.show();

    return a.exec();
}
