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
    QDir dir(DataDefines::appCopyPath() + "log/");
    if (!dir.exists())
        dir.mkpath(DataDefines::appCopyPath() + "log/");
    QString fn = DataDefines::appCopyPath() + "log/" + LogFileName;
    QFile fLog(fn);
    if (fLog.open(QIODevice::Append | QIODevice::Text))
    {
        QString std = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm") + "\n";
        fLog.write(std.toStdString().c_str());
        QString line = str + "\n\n";
        fLog.write(line.toStdString().c_str());
    }
}
