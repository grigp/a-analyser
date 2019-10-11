#include "personalnormmanager.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

#include "aanalyserapplication.h"
#include "dataprovider.h"

PersonalNormManager::PersonalNormManager(QObject *parent)
    : QObject(parent)
{
    loadConditions();

}

QStringList PersonalNormManager::getTestConditions()  const
{
    QStringList retval;
    foreach (auto cond, m_tcList)
        retval << cond.uid;
    return retval;
}

bool PersonalNormManager::getTestConditionInfo(const QString &uid, DataDefines::TestConditionInfo &ci)  const
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

bool PersonalNormManager::getPersonalNorm(const QString &testUid,
                                          QList<DataDefines::PersonalNormInfo> &pnil) const
{
    //! Получили данные теста
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
        return DataProvider::getPersonalNorm(ti.patientUid, ti.metodUid, ti.condition, pnil);
    return false;
}

int PersonalNormManager::getPersonalNormContainedTestCount(const QString &testUid) const
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
                QStringList tests = DataProvider::getTests(ti.patientUid, ti.metodUid, ti.condition);
                return tests.size();
            }
    }
    return 0;
}

void PersonalNormManager::setTestNormContained(const QString &testUid,
                                               const bool isNormContained) const
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

void PersonalNormManager::calculateAll()
{

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


void PersonalNormManager::calculate(const QString &patientUid, const QString &methodUid, const QString &conditionUid) const
{
    QList<QList<FactorsDefines::FactorValueAdvanced>> factors;
    factors.clear();

    //! Собрать в таблицу все показатели нормообразующих тестов
    fillFactorsTable(patientUid, methodUid, conditionUid, factors);

    //! Есть показатели
    if (factors.size() > 0)
    {
        QList<FactorsDefines::Norm> norms;

        //! Расчет нормативов
        calculateNormsForTable(factors, norms);

        //! И сохранение их в БД
        saveNormsToDatabase(patientUid, methodUid, conditionUid, factors, norms);
    }
}

void PersonalNormManager::fillFactorsTable(const QString &patientUid, const QString &methodUid, const QString &conditionUid,
                                           QList<QList<FactorsDefines::FactorValueAdvanced>> &factors) const
{
    QStringList tests = DataProvider::getTests(patientUid, methodUid, conditionUid);
    foreach (auto testUid, tests)
    {
        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(testUid, ti))
        {
            QList<FactorsDefines::FactorValueAdvanced> tstFactors = DataProvider::getPrimaryFactors(testUid);
            factors << tstFactors;
        }
    }
}

void PersonalNormManager::calculateNormsForTable(const QList<QList<FactorsDefines::FactorValueAdvanced> > &factors,
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
            else
            {
                FactorsDefines::Norm n = norms.at(i);
                n.value = n.value + tstFactors.at(i).value();
                norms.replace(i, n);
            }
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

void PersonalNormManager::saveNormsToDatabase(const QString &patientUid, const QString &methodUid, const QString &conditionUid,
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

