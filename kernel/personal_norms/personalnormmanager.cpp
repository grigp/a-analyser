#include "personalnormmanager.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

#include "aanalyserapplication.h"

PersonalNormManager::PersonalNormManager(QObject *parent)
    : QObject(parent)
{
    loadConditions();

}

QStringList PersonalNormManager::getTestConditions()
{
    QStringList retval;
    foreach (auto cond, m_tcList)
        retval << cond.uid;
    return retval;
}

bool PersonalNormManager::getTestConditionInfo(const QString &uid, DataDefines::TestConditionInfo &ci)
{
    foreach (auto cond, m_tcList)
        if (cond.uid == uid)
        {
            ci.uid = cond.uid;
            ci.name = cond.name;
            ci.description = cond.description;
            ci.norms_enabled = cond.norms_enabled;
            return true;
        }
    return false;
}

void PersonalNormManager::loadConditions()
{
    m_tcList.clear();

    QString resName = ":/pre_settings/connections.json";
    if (static_cast<AAnalyserApplication*>(QApplication::instance())->languargeCode() == DataDefines::LANG_CODE_RUS)
        resName = ":/pre_settings/conditions.json";
    else
    if (static_cast<AAnalyserApplication*>(QApplication::instance())->languargeCode() == DataDefines::LANG_CODE_ENGUSA)
        resName = ":/pre_settings/conditions_en_US.json";

    QJsonObject condObj;
    QFile file(resName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        condObj = loadDoc.object();
        file.close();
    }

    auto condArr = condObj["conditions"].toArray();
    for (int i = 0; i < condArr.size(); ++i)
    {
        auto obj = condArr.at(i).toObject();
        DataDefines::TestConditionInfo ci;

        ci.uid = obj["uid"].toString();
        ci.name = obj["name"].toString();
        ci.description = obj["description"].toString();
        ci.norms_enabled = obj["norms_enabled"].toBool();

        m_tcList.append(ci);
    }
}
