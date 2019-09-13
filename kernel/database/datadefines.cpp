#include "datadefines.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUuid>
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


QString DataDefines::aanalyserDataPath()
{
    QString path = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QString(), QStandardPaths::LocateDirectory);
    return path + QApplication::applicationName() + '/';
}


QString DataDefines::appCopyPath()
{
    if (m_appCopyUid != "")
        return aanalyserDataPath() + m_appCopyUid + "/";
    else
    {
        QString fnCopies = aanalyserDataPath() + "copies.json";
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
            return aanalyserDataPath() + m_appCopyUid + "/";
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
                    return aanalyserDataPath() + m_appCopyUid + "/";
                }
            }
            //! Не нашли uid - создаем, записываем в файл и возвращаем
            QJsonObject obj;
            obj["path"] = QApplication::instance()->applicationDirPath();
            m_appCopyUid = QUuid::createUuid().toString();
            obj["uid"] = m_appCopyUid;
            arr.append(obj);
            writeCopiesFile(arr, fnCopies);
            return aanalyserDataPath() + m_appCopyUid + "/";
        }
    }
    return "";
}

QString DataDefines::dataBasesPath()
{
    return aanalyserDataPath() + "databases/";
}


QString DataDefines::appCopyUid()
{
    appCopyPath();
    return m_appCopyUid;
}

QList<DataDefines::DatabaseInfo> DataDefines::getDatabases()
{
    QList<DatabaseInfo> retval;
    retval.clear();
    QDir dir = dataBasesPath();
    QFileInfoList list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (auto fileInfo, list)
    {
        DatabaseInfo info = getDatabaseInfo(dataBasesPath() + fileInfo.fileName() + "/");
        retval << info;
    }

    return retval;
}

DataDefines::DatabaseInfo DataDefines::getDatabaseInfo(const QString &dbFolder)
{
    DatabaseInfo retval;

    QStringList folders = dbFolder.split("/");
    if (folders.size() > 2)
        retval.name = folders.at(folders.size() - 2);
    else
        retval.name = "";
    retval.version = DataBaseVersionCode;

    QFile fileRec(dbFolder + "db.info");
    if (fileRec.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fileRec.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        QJsonObject info = loadDoc.object();
        fileRec.close();

        retval.comment = info["comment"].toString();
        retval.version = info["version"].toString();
    }

    return retval;
}

void DataDefines::setDatabaseComment(const QString &dbFolder, const QString &comment)
{
    DatabaseInfo info = getDatabaseInfo(dbFolder);
    QJsonObject obj;
    obj["name"] = info.name;
    obj["comment"] = comment;
    obj["version"] = info.version;

    QFile fileRec(dbFolder + "db.info");
    fileRec.remove();
    if (fileRec.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonDocument doc(obj);
        QByteArray ba = doc.toJson();
        fileRec.write(ba);

        fileRec.close();
    }
}
