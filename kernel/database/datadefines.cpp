#include "datadefines.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUuid>
#include <QDebug>

namespace
{

QString appCopyUid = "";

}

void writeCopiesFile(const QJsonArray &arr, const QString &fn)
{
    QFile fCopies(fn);
    if (fCopies.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject obj;
        obj["copies"] = arr;

        QJsonDocument doc(obj);
        QByteArray ba = doc.toJson();
        fCopies.write(ba);
        fCopies.close();
    }
}

QJsonArray readCopiesFile(const QString &fn)
{
    QFile fCopies(fn);
    if (fCopies.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fCopies.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(ba));
        QJsonObject obj = doc.object();
        fCopies.close();

        return obj["copies"].toArray();
    }
    return QJsonArray();
}


QString DataDefines::aanalyserDataPath()
{
    QString path = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QString(), QStandardPaths::LocateDirectory);
    return path + QApplication::applicationName() + '/';
}


QString DataDefines::appCopyPath()
{
    if (appCopyUid != "")
        return aanalyserDataPath() + appCopyUid + "/";
    else
    {
        QString fnCopies = aanalyserDataPath() + "copies.json";
        if (!QFile::exists(fnCopies))
        {
            //! Не нашли файл - создаем и записываем uid копии, который и возвращаем
            QJsonObject obj;
            obj["path"] = QApplication::instance()->applicationDirPath();
            appCopyUid = QUuid::createUuid().toString();
            obj["uid"] = appCopyUid;
            QJsonArray arr;
            arr.append(obj);
            writeCopiesFile(arr, fnCopies);
            return aanalyserDataPath() + appCopyUid + "/";
        }
        else
        {
            //! Нашли файл - читаем и ищем uid
            auto arr = readCopiesFile(fnCopies);
            for (int i = 0; i < arr.size(); ++i)
            {
                auto obj = arr.at(i).toObject();
                if (obj["path"] == QApplication::instance()->applicationDirPath())
                {
                    //! Нашли uid - возвращаем его и запоминаем, чтоб дальше не переспрашивать
                    appCopyUid = obj["uid"].toString();
                    return aanalyserDataPath() + appCopyUid + "/";
                }
            }
            //! Не нашли uid - создаем, записываем в файл и возвращаем
            QJsonObject obj;
            obj["path"] = QApplication::instance()->applicationDirPath();
            appCopyUid = QUuid::createUuid().toString();
            obj["uid"] = appCopyUid;
            arr.append(obj);
            writeCopiesFile(arr, fnCopies);
            return aanalyserDataPath() + appCopyUid + "/";
        }
    }
    return "";
}

QString DataDefines::dataBasesPath()
{
    return aanalyserDataPath() + "databases/";
}



