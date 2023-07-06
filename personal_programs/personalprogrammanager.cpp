#include "personalprogrammanager.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QDebug>

#include "datadefines.h"
#include "personalprogramdefines.h"

PersonalProgramManager::PersonalProgramManager(QObject *parent)
    : QObject (parent)
{

}

void PersonalProgramManager::readDailyProgramList(QStandardItemModel &model)
{
    model.clear();

    QString dirName = DataDefines::appCopyPath() + "personal_programs/";
    QDir dir(dirName);
    if (!dir.exists())
        dir.mkpath(dirName);

    QFile fDP(dirName + "dp.json");
    if (fDP.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fDP.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        auto objDPs = loadDoc.object();
        fDP.close();

        auto arrDPs = objDPs["dp_list"].toArray();
        for (int i = 0; i < arrDPs.size(); ++i)
        {
            auto objDP = arrDPs.at(i).toObject();
            QString name = objDP["name"].toString();

            auto *item = new QStandardItem(name);
            item->setEditable(false);
            item->setData(objDP, PersonalProgramDefines::TableDPRoles::DPRole);
            model.appendRow(item);
        }
    }
}

void PersonalProgramManager::saveDailyProgramList(const QStandardItemModel &model)
{
    QJsonObject objDPs;
    QJsonArray arrDPs;
    for (int i = 0; i < model.rowCount(); ++i)
    {
        auto dp = model.index(i, 0).data(PersonalProgramDefines::TableDPRoles::DPRole).toJsonObject();
        arrDPs << dp;
    }
    objDPs["dp_list"] = arrDPs;

    QString dirName = DataDefines::appCopyPath() + "personal_programs/";
    QDir dir(dirName);
    if (!dir.exists())
        dir.mkpath(dirName);

    QFile fDP(dirName + "dp.json");
    if (fDP.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonDocument doc(objDPs);
        QByteArray ba = doc.toJson();
        fDP.write(ba);
    }
}
