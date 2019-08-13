#include "baseutils.h"

#include <QDir>

QString BaseUtils::getTimeBySecCount(const int secCnt, const bool isHour)
{
    int min = secCnt / 60;
    int sec = secCnt % 60;
    if (isHour)
    {
        int hour = min / 60;
        min = min % 60;
        return QString::number(hour).rightJustified(2, '0') + ":" +
               QString::number(min).rightJustified(2, '0') + ":" +
               QString::number(sec).rightJustified(2, '0');
    }
    return QString::number(min).rightJustified(2, '0') + ":" +
           QString::number(sec).rightJustified(2, '0');
}

bool BaseUtils::getTranslatorFileName(QString &fileName, QString &langCode)
{
    QDir dir("translations");
    QFileInfoList list = dir.entryInfoList();
    foreach (auto fileInfo, list)
        if (fileInfo.fileName() != "." && fileInfo.fileName() != "..")
        {
            if (fileInfo.fileName().contains("a-analyser_") &&
                fileInfo.completeSuffix() == "qm")
            {
                fileName = fileInfo.absoluteFilePath();
                langCode = fileInfo.baseName().remove("a-analyser_");
                return true;
            }
        }
    return false;
}
