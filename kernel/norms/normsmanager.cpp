#include "normsmanager.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

#include "aanalyserapplication.h"
#include "settingsprovider.h"
#include "dataprovider.h"

NormsManager::NormsManager(QObject *parent)
    : QObject(parent)
{
    loadConditions();
    loadDopuskGroupNorms();

}

QStringList NormsManager::getTestConditions()  const
{
    QStringList retval;
    foreach (auto cond, m_tcList)
        retval << cond.uid;
    return retval;
}

bool NormsManager::getTestConditionInfo(const QString &uid, DataDefines::TestConditionInfo &ci)  const
{
    if (uid == "")
    {
        ci.uid = m_tcList.at(0).uid;
        ci.name = m_tcList.at(0).name;
        ci.description = m_tcList.at(0).description;
        ci.norms_enabled = m_tcList.at(0).norms_enabled;
        return true;
    }
    else
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
    }
    return false;
}

bool NormsManager::getPersonalNorm(const QString &testUid,
                                          QList<DataDefines::PersonalNormInfo> &pnil) const
{
    //! Получили данные теста
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
        return DataProvider::getPersonalNorm(ti.patientUid, ti.metodUid, ti.condition, pnil);
    return false;
}

int NormsManager::getPersonalNormContainedTestCount(const QString &testUid) const
{
    //! Получили данные теста
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        //! Получили данные условий проведения
        DataDefines::TestConditionInfo tci;
        if (getTestConditionInfo(ti.condition, tci))
            //! Разрешается строить нормы по данным условиям проведения
            if (tci.norms_enabled)
            {
                //! По тестам
                int cnt = 0;
                QStringList tests = DataProvider::getTests(ti.patientUid, ti.metodUid, ti.condition);
                foreach (auto testUid, tests)
                {
                    //! Суммируем только нормообразующие
                    if (isTestNormContained(testUid))
                        ++cnt;
                }
                return cnt;
            }
    }
    return 0;
}

void NormsManager::setTestNormContained(const QString &testUid,
                                               const bool isNormContained)
{
    //! Получили данные теста
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        //! Получили данные условий проведения
        DataDefines::TestConditionInfo tci;
        if (getTestConditionInfo(ti.condition, tci))
            //! Разрешается строить нормы по данным условиям проведения
            if (tci.norms_enabled)
            {
                //! Включаем обследование в нормообразующие
                DataProvider::setTestNormContained(testUid, isNormContained);
                //! Перерасчет индивидуальной нормы
                calculate(ti.patientUid, ti.metodUid, ti.condition);
            }
    }
}

void NormsManager::calculate(const QString &patientUid, const QString &methodUid, const QString &conditionUid)
{
    QList<QList<FactorsDefines::FactorValueAdvanced>> factors;
    factors.clear();

    //! Собрать в таблицу все показатели нормообразующих тестов
    fillFactorsTable(patientUid, methodUid, conditionUid, factors);

    //! Есть показатели и их не меньше, чем минимальное кол-во
    int minTestCount = SettingsProvider::valueFromRegAppCopy("Norms", "MinTestCount", 5).toInt();

    if (factors.size() >= minTestCount)
    {
        QList<FactorsDefines::Norm> norms;
        //! Расчет нормативов
        calculateNormsForTable(factors, norms);
        //! И сохранение их в БД
        saveNormsToDatabase(patientUid, methodUid, conditionUid, factors, norms);
        //! Оповещение мира об изменении нормы
        emit personalNormRecalculated(patientUid, methodUid, conditionUid);
    }
    else
    {
        //! Если показтелей меньше, чем необходимо, то надо удалить норму, если она была ранее построена
        if (DataProvider::personalNormExists(patientUid, methodUid, conditionUid))
        {
            DataProvider::deletePersonalNorm(patientUid, methodUid, conditionUid);
            //! Оповещение мира об удалении нормы
            emit personalNormDeleted(patientUid, methodUid, conditionUid);
        }
        //! Оповещение мира об изменении кол-ва нормообразующих тестов
        emit personalNormContainedChange(patientUid, methodUid, conditionUid);
    }
}

void NormsManager::calculateAll()
{

}

bool NormsManager::getGroupNorms(const QString &methodicUid, const QString &conditionUid,
                                 QList<DataDefines::GroupNormInfo> &gni) const
{
    gni.clear();
    bool retval = false;
    foreach (auto gNorm, m_groupNorms)
        if (gNorm.methodicUid == methodicUid && gNorm.conditionUid == conditionUid)
        {
            gni << gNorm;
            retval = true;
        }
    return retval;
}

void NormsManager::loadConditions()
{
    m_tcList.clear();

    QString resName = ":/pre_settings/conditions.json";
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

void NormsManager::loadDopuskGroupNorms()
{
    m_groupNorms.clear();

    QString resName = ":/pre_settings/group_norms.json";
    QJsonObject normsObj;
    QFile file(resName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
        normsObj = loadDoc.object();
        file.close();
    }

    auto normsArr = normsObj["norms"].toArray();
    for (int i = 0; i < normsArr.size(); ++i)
    {
        auto obj = normsArr.at(i).toObject();

        DataDefines::GroupNormInfo gni;
        gni.methodicUid = obj["methodic"].toString();
        gni.conditionUid = obj["condition"].toString();
        gni.factorUid = obj["factor"].toString();
        gni.probeNum = obj["probe_num"].toInt();
        gni.border = obj["border"].toDouble();
        gni.conditionBorder = obj["condition_border"].toDouble();
        m_groupNorms.append(gni);
    }
}


void NormsManager::fillFactorsTable(const QString &patientUid, const QString &methodUid, const QString &conditionUid,
                                           QList<QList<FactorsDefines::FactorValueAdvanced>> &factors) const
{
    QStringList tests = DataProvider::getTests(patientUid, methodUid, conditionUid);
    foreach (auto testUid, tests)
    {
        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(testUid, ti))
        {
            //! Заносим только нормообразующие
            if (isTestNormContained(testUid))
            {
                QList<FactorsDefines::FactorValueAdvanced> tstFactors = DataProvider::getPrimaryFactors(testUid);
                factors << tstFactors;
            }
        }
    }
}

void NormsManager::calculateNormsForTable(const QList<QList<FactorsDefines::FactorValueAdvanced> > &factors,
                                                 QList<FactorsDefines::Norm> &norms) const
{
    //! Расчет среднего значения
    bool first = true;
    foreach (auto tstFactors, factors)
    {
        for (int i = 0; i < tstFactors.size(); ++i)
        {
            if (first)
                norms.append(FactorsDefines::Norm(0, 0));
            FactorsDefines::Norm n = norms.at(i);
            n.value = n.value + tstFactors.at(i).value();
            norms.replace(i, n);
        }
        first = false;
    }
    for (int i = 0; i < norms.size(); ++i)
    {
        FactorsDefines::Norm n = norms.at(i);
        n.value = n.value / factors.size();
        norms.replace(i, n);
    }

    //! Расчет СКО
    foreach (auto tstFactors, factors)
    {
        for (int i = 0; i < tstFactors.size(); ++i)
        {
            FactorsDefines::Norm n = norms.at(i);
            n.stdDev = n.stdDev + pow(fabs(tstFactors.at(i).value() - n.value), 2) /
                    (factors.size() - 1);
            norms.replace(i, n);
        }
    }
    for (int i = 0; i < norms.size(); ++i)
    {
        FactorsDefines::Norm n = norms.at(i);
        n.stdDev = sqrt(n.stdDev);
        norms.replace(i, n);
    }
}

void NormsManager::saveNormsToDatabase(const QString &patientUid, const QString &methodUid, const QString &conditionUid,
                                              const QList<QList<FactorsDefines::FactorValueAdvanced> > &factors,
                                              const QList<FactorsDefines::Norm> &norms) const
{
    for (int i = 0; i < factors.at(0).size(); ++i)
    {
        if (i < norms.size())
            DataProvider::setPersonalNorm(patientUid, methodUid, conditionUid,
                                          factors.at(0).at(i).uid(), factors.at(0).at(i).probeNum(),
                                          norms.at(i).value, norms.at(i).stdDev);
    }
}

bool NormsManager::isTestNormContained(const QString &testUid)  const
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
        return ti.isNormContained && (ti.dateTime.daysTo(QDateTime::currentDateTime()) / 30 < 12);

    return false;
}

