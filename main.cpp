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
    qDebug() << Q_FUNC_INFO;

    qsrand(static_cast<uint>(QDateTime::currentMSecsSinceEpoch()));
    AAnalyserApplication a(argc, argv);

    QString fn = "";
    QString code = DataDefines::LANG_CODE_RUS;
//    QString code = DataDefines::LANG_CODE_ENGUSA;
    QString codeFound = "";
    auto trLoaded = BaseUtils::getTranslatorFileName(fn, codeFound);
    qDebug() << trLoaded << fn << code << codeFound << (codeFound == code);

    QTranslator translator;
    if (codeFound == code)
    {
        if (trLoaded)
            translator.load(fn);

        a.setLanguargeCode(codeFound);
        if (trLoaded)
            a.installTranslator(&translator);
    }

    MainWindow w;
    a.setMainWindow(&w);
    w.show();

    return a.exec();
}
