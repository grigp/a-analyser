#include "datadefines.h"

#include <QApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUuid>
#include <QStandardPaths>
#include <QDir>

#include <QDebug>

namespace
{

QString m_appCopyUid = "";

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



QString DataDefines::kerdoDataPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + '/';
}



QString DataDefines::kerdoDocumentsPath()
{
    QString path = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
    path = path + "kerdo/";
    QDir dir(path);
    if (!dir.exists())
        dir.mkpath(path);
    return path;
}



QString DataDefines::appCopyPath()
{
    if (m_appCopyUid != "")
        return kerdoDataPath() + m_appCopyUid + "/";
    else
    {
        QString fnCopies = kerdoDataPath() + "copies.json";
        if (!QFile::exists(fnCopies))
        {
            //! Не нашли файл - создаем и записываем uid копии, который и возвращаем
            QJsonObject obj;
            obj["path"] = QApplication::instance()->applicationDirPath();
            m_appCopyUid = QUuid::createUuid().toString();
            obj["uid"] = m_appCopyUid;
            QJsonArray arr;
            arr.append(obj);
            writeCopiesFile(arr, fnCopies);
            return kerdoDataPath() + m_appCopyUid + "/";
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
                    m_appCopyUid = obj["uid"].toString();
                    return kerdoDataPath() + m_appCopyUid + "/";
                }
            }
            //! Не нашли uid - создаем, записываем в файл и возвращаем
            QJsonObject obj;
            obj["path"] = QApplication::instance()->applicationDirPath();
            m_appCopyUid = QUuid::createUuid().toString();
            obj["uid"] = m_appCopyUid;
            arr.append(obj);
            writeCopiesFile(arr, fnCopies);
            return kerdoDataPath() + m_appCopyUid + "/";
        }
    }
}



QString DataDefines::dataBasesPath()
{
    return kerdoDataPath() + "databases/";
}



QString DataDefines::appCopyUid()
{
    appCopyPath();
    return m_appCopyUid;
}

