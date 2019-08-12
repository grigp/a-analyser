#include "log.h"

#include "datadefines.h"

#include <QFile>
#include <QDir>

namespace
{

static const QString LogFileName = "events.log";

}

void log(const QString &str)
{
    QDir dir(DataDefines::appDataPath() + "log/");
    if (!dir.exists())
        dir.mkpath(DataDefines::appDataPath() + "log/");
    QString fn = DataDefines::appDataPath() + "log/" + LogFileName;
    QFile fLog(fn);
    if (fLog.open(QIODevice::Append | QIODevice::Text))
    {
        QString std = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm") + "\n";
        fLog.write(std.toStdString().c_str());
        QString line = str + "\n\n";
        fLog.write(line.toStdString().c_str());
    }
}
