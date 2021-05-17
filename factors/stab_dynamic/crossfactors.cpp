#include "crossfactors.h"

#include "aanalyserapplication.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"

CrossFactors::CrossFactors(const QString &testUid,
                           const QString &probeUid,
                           QObject *parent)
    : ProbeMultifactor (testUid, probeUid, parent)
{
    if (isValid())
        calculate();
}

bool CrossFactors::isValid() const
{
    return isValid(testUid(), probeUid());
}

bool CrossFactors::isValid(const QString &testUid, const QString &probeUid)
{
    Q_UNUSED(testUid);
    auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
    auto se = DataProvider::channelExists(probeUid, ChannelsDefines::chanCrossResult);
    return de && se;

//    Q_UNUSED(probeUid);
//    DataDefines::TestInfo ti;
//    if (DataProvider::getTestInfo(testUid, ti))
//        foreach (auto probeUid, ti.probes)
//        {
//            auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
//            auto se = DataProvider::channelExists(probeUid, ChannelsDefines::chanCrossResult);
//            if (!se)
//                return false;
//        }
//    return ti.probes.size() > 1 && ti.probes.size() <= 5;
}

void CrossFactors::calculate()
{
    m_LUp = 1;  ///TODO: Временная заглушка для отладки GUI
    m_LDn = 2;
    m_LRt = 3;
    m_LLf = 4;
    m_SZone = 5;
    m_UD = 6;
    m_RL = 7;
    m_SF = 8;

    addFactor(CrossFactorsDefines::LUpUid, m_LUp);
    addFactor(CrossFactorsDefines::LDnUid, m_LDn);
    addFactor(CrossFactorsDefines::LRtUid, m_LRt);
    addFactor(CrossFactorsDefines::LLfUid, m_LLf);
    addFactor(CrossFactorsDefines::SZoneUid, m_SZone);
    addFactor(CrossFactorsDefines::UDUid, m_UD);
    addFactor(CrossFactorsDefines::RLUid, m_RL);
    addFactor(CrossFactorsDefines::SFUid, m_SF);
}

void CrossFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(CrossFactorsDefines::GroupUid, tr("Показатели теста на устойчивость"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::LUpUid, CrossFactorsDefines::GroupUid,
                           tr("Отклонение вперед"), tr("LUp"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::LDnUid, CrossFactorsDefines::GroupUid,
                           tr("Отклонение назад"), tr("LDown"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::LRtUid, CrossFactorsDefines::GroupUid,
                           tr("Отклонение вправо"), tr("LRight"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::LLfUid, CrossFactorsDefines::GroupUid,
                           tr("Отклонение влево"), tr("LLeft"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::SZoneUid, CrossFactorsDefines::GroupUid,
                           tr("Площадь зоны перермещения"), tr("SZone"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::UDUid, CrossFactorsDefines::GroupUid,
                           tr("Отношение вперед / назад"), tr("Up/Dn"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::RLUid, CrossFactorsDefines::GroupUid,
                           tr("Отношение вправо / влево"), tr("Rt/Lf"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(CrossFactorsDefines::SFUid, CrossFactorsDefines::GroupUid,
                           tr("Отношение сагитталь / фронталь"), tr("Sg/Fr"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
}
