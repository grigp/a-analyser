#include "factorsfactory.h"

#include "classicfactors.h"
#include "vectorfactors.h"
#include "ratioprobesfactors.h"

#include <QTimer>
#include <QDebug>

FactorsFactory::FactorsFactory(QObject *parent) : QObject(parent)
{
    //! Регистрация показателей из факторсгрупп
    QTimer::singleShot(0, [=]()
    {
        ClassicFactors::registerFactors();
        VectorFactors::registerFactors();
        RatioProbesFactors::registerFactors();
    });
}

FactorsDefines::FactorInfo FactorsFactory::getFactorInfo(const QString &uid) const
{
    if (m_factors.contains(uid))
        return m_factors.value(uid);
    else
        return FactorsDefines::FactorInfo();
}

QString FactorsFactory::getGroupName(const QString &groupUid) const
{
    return m_groups.value(groupUid);
}

void FactorsFactory::registerFactor(const QString &uid, const QString &groupUid,
                                    const QString &name, const QString &shortName,
                                    const QString &measure, const int format)
{
    FactorsDefines::FactorInfo fi(uid, groupUid, name, shortName, measure, format);
    m_factors.insert(uid, fi);
}

void FactorsFactory::registerGroup(const QString &uid, const QString &name)
{
    m_groups.insert(uid, name);
    if (uid == ClassicFactorsDefines::GroupUid)
        m_creatorsChannel.insert(uid, new ClassicFactorsCreator());
}

