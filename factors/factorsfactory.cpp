#include "factorsfactory.h"

#include "aanalyserbuild.h"
#include "multifactordescriptor.h"

#include <QTimer>
#include <QDebug>

FactorsFactory::FactorsFactory(QObject *parent) : QObject(parent)
{
    //! Регистрация показателей из факторсгрупп
    QTimer::singleShot(0, [=]()
    {
        AAnalyserBuild::registerFactors();
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
}

void FactorsFactory::registerGroup(MultiFactorDescriptor *group)
{
    if (group->level() == BaseDefines::tlTest)
        m_groupsTest << group;
    else
    if (group->level() == BaseDefines::tlProbe)
        m_groupsProbe << group;
    else
    if (group->level() == BaseDefines::tlChannel)
        m_groupsChannel << group;
}

