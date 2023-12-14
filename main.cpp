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

    qsrand(QDateTime::currentMSecsSinceEpoch());
    AAnalyserApplication a(argc, argv);

    QString fn = "";
    QString code = DataDefines::LANG_CODE_RUS;
    //QString code = DataDefines::LANG_CODE_ENGUSA;
    auto trLoaded = BaseUtils::getTranslatorFileName(fn, code);

    QTranslator translator;
    if (trLoaded)
        translator.load(fn);

    a.setLanguargeCode(code);
    if (trLoaded)
        a.installTranslator(&translator);

    MainWindow w;
    a.setMainWindow(&w);
    w.show();

    return a.exec();
}
