#include "ratioprobesfactors.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsutils.h"

#include "classicfactors.h"
#include "vectorfactors.h"

#include "aanalyserapplication.h"

namespace
{
struct RatioFactorsUid
{
    QString SUid;
    QString KFRUid;
    RatioFactors() {}
};

RatioFactorsUid RFUid[4] = {{RatioProbesFactorsDefines::Probe2SUid, RatioProbesFactorsDefines::Probe2KFRUid},
                            {RatioProbesFactorsDefines::Probe3SUid, RatioProbesFactorsDefines::Probe3KFRUid},
                            {RatioProbesFactorsDefines::Probe4SUid, RatioProbesFactorsDefines::Probe4KFRUid},
                            {RatioProbesFactorsDefines::Probe5SUid, RatioProbesFactorsDefines::Probe5KFRUid}};

}

RatioProbesFactors::RatioProbesFactors(const QString &testUid, QObject *parent)
    : TestMultifactor(testUid, parent)
{
    if (isValid())
        calculate();
}

bool RatioProbesFactors::isValid() const
{
    return isValid(testUid());
}

bool RatioProbesFactors::isValid(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
        foreach (auto probeUid, ti.probes)
        {
            auto ce = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
            if (!ce)
                return false;
        }
    return ti.probes.size() > 1 && ti.probes.size() <= 5;
}

void RatioProbesFactors::calculate()
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        //! Первая проба - фоновые значения
        double S0 = 0;
        double KFR0 = 0;
        if (ti.probes.size() > 0)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(0), pi))
            {
                auto fctClass = new ClassicFactors(testUid(), pi.uid, ChannelsDefines::chanStab);
                S0 = fctClass->factorValue(ClassicFactorsDefines::SquareUid);
                delete fctClass;
                auto fctVector = new VectorFactors(testUid(), pi.uid, ChannelsDefines::chanStab);
                KFR0 = fctVector->factorValue(VectorFactorsDefines::KFRUid);
                delete fctVector;
            }
        }

        //! Последующие пробы - отношения
        for (int i = 1; i < ti.probes.size(); ++i)
        {
            if (i == 5)
                break;
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                auto fctClass = new ClassicFactors(testUid(), pi.uid, ChannelsDefines::chanStab);
                auto S = fctClass->factorValue(ClassicFactorsDefines::SquareUid);
                delete fctClass;
                auto fctVector = new VectorFactors(testUid(), pi.uid, ChannelsDefines::chanStab);
                auto KFR = fctVector->factorValue(VectorFactorsDefines::KFRUid);
                delete fctVector;

                addFactor(RFUid[i - 1].SUid, S / S0 * 100);
                addFactor(RFUid[i - 1].KFRUid, KFR0 / KFR * 100);
            }
        }
    }
}

void RatioProbesFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(RatioProbesFactorsDefines::GroupUid, tr("Показатели отношений"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(RatioProbesFactorsDefines::Probe2SUid, RatioProbesFactorsDefines::GroupUid,
                           tr("Коэффициент отношения по площади"), tr("KS"), tr("%"), 0);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(RatioProbesFactorsDefines::Probe2KFRUid, RatioProbesFactorsDefines::GroupUid,
                           tr("Коэффициент отношения по КФР"), tr("KKFR"), tr("%"), 0);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(RatioProbesFactorsDefines::Probe3SUid, RatioProbesFactorsDefines::GroupUid,
                           tr("Коэффициент отношения по площади"), tr("KS"), tr("%"), 0);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(RatioProbesFactorsDefines::Probe3KFRUid, RatioProbesFactorsDefines::GroupUid,
                           tr("Коэффициент отношения по КФР"), tr("KKFR"), tr("%"), 0);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(RatioProbesFactorsDefines::Probe4SUid, RatioProbesFactorsDefines::GroupUid,
                           tr("Коэффициент отношения по площади"), tr("KS"), tr("%"), 0);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(RatioProbesFactorsDefines::Probe4KFRUid, RatioProbesFactorsDefines::GroupUid,
                           tr("Коэффициент отношения по КФР"), tr("KKFR"), tr("%"), 0);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(RatioProbesFactorsDefines::Probe5SUid, RatioProbesFactorsDefines::GroupUid,
                           tr("Коэффициент отношения по площади"), tr("KS"), tr("%"), 0);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(RatioProbesFactorsDefines::Probe5KFRUid, RatioProbesFactorsDefines::GroupUid,
                           tr("Коэффициент отношения по КФР"), tr("KKFR"), tr("%"), 0);
}
