#include "datadefines.h"



QString DataDefines::dataBasesPath()
{
    return appDataPath() + "databases/";
}

QString DataDefines::appDataPath()
{
    QString path = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QString(), QStandardPaths::LocateDirectory);
    return path + QApplication::applicationName() + '/';
}
