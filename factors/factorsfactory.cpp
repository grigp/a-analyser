#include "factorsfactory.h"

#include "classicfactors.h"
#include "vectorfactors.h"
#include "ratioprobesfactors.h"
#include "jumpheightsinglefactors.h"
#include "hoppingfactors.h"
#include "idsfactors.h"
#include "targetfactors.h"
#include "trenresultfactors.h"

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
        JumpHeightSingleFactors::registerFactors();
        HoppingFactors::registerFactors();
        IDSFactors::registerFactors();
        TargetFactors::registerFactors();
        TrenResultFactors::registerFactors();
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
                                    const QString &measure, const int format,
                                    const double multiplier, const FactorsDefines::NormSide side, const int agingPeriod)
{
    FactorsDefines::FactorInfo fi(uid, groupUid, name, shortName, measure,
                                  format, multiplier, side, agingPeriod);
    m_factors.insert(uid, fi);
}

void FactorsFactory::registerGroup(const QString &uid, const QString &name)
{
    m_groups.insert(uid, name);
    if (uid == ClassicFactorsDefines::GroupUid)
        m_creatorsChannel.insert(uid, new ClassicFactorsCreator());
}

