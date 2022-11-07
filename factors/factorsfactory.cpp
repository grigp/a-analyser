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
    Q_ASSERT_X(m_factors.contains(uid), "FactorsFactory", "Factor not found");
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

int FactorsFactory::multiFactorCount(const BaseDefines::TestLevel level)
{
    switch (level)
    {
    case BaseDefines::tlTest: return m_groupsTest.size();
    case BaseDefines::tlProbe: return m_groupsProbe.size();
    case BaseDefines::tlChannel: return m_groupsChannel.size();
    case BaseDefines::tlNone: return 0;
    }
    return 0;
}

MultiFactorDescriptor *FactorsFactory::getMultiFactor(const BaseDefines::TestLevel level, const int idx)
{
    switch (level)
    {
    case BaseDefines::tlTest:
    {
        Q_ASSERT(idx >= 0 && idx < m_groupsTest.size());
        return m_groupsTest.at(idx);
    }
    case BaseDefines::tlProbe:
    {
        Q_ASSERT(idx >= 0 && idx < m_groupsProbe.size());
        return m_groupsProbe.at(idx);
    }
    case BaseDefines::tlChannel:
    {
        Q_ASSERT(idx >= 0 && idx < m_groupsChannel.size());
        return m_groupsChannel.at(idx);
    }
    case BaseDefines::tlNone: return nullptr;
    }
    return nullptr;
}

