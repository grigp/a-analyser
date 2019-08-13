#include "mainwindow.h"

#include <aanalyserapplication.h>
#include "exitcodes.h"
#include "baseutils.h"
#include "datadefines.h"

#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    QString fn = "";
    QString code = DataDefines::LANG_CODE_RUS;
    auto trLoaded = BaseUtils::getTranslatorFileName(fn, code);

    QTranslator translator;
    if (trLoaded)
        translator.load(fn);

    AAnalyserApplication a(argc, argv, code);
    if (trLoaded)
        a.installTranslator(&translator);

    MainWindow w;
    a.setMainWindow(&w);
    w.show();

    return a.exec();
}
