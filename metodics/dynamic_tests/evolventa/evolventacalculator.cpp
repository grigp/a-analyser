#include "evolventacalculator.h"

#include <QUuid>
#include <QDebug>

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "testresultdata.h"
#include "resultinfo.h"
#include "decartcoordinatessignal.h"
#include "evolventaresultdata.h"
#include "evolventafactors.h"

EvolventaCalculator::EvolventaCalculator(const QString &testUid, QObject *parent)
    : TestCalculator (testUid, parent)
    , m_trd(new TestResultData())
{

}

EvolventaCalculator::~EvolventaCalculator()
{
    if (m_factors)
        delete m_factors;
    if (m_trd)
        delete m_trd;
}

void EvolventaCalculator::calculate()
{
    TestCalculator::calculate();

    //! Необходимо в тесте, в котором будет возможно ни одна проба найти нужную пробу
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        m_trd->openTest(testUid());
        Q_ASSERT(ti.probes.size() == m_trd->probesCount());

        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanStab) &&
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanEvolventaResult))
                {
                    //! Результаты теста Эвольвента
                    QByteArray baData;
                    if (DataProvider::getChannel(pi.uid, ChannelsDefines::chanEvolventaResult, baData))
                        m_erData = new EvolventaResultData(baData);

                    //! Сигналы с эвольвентой
                    auto* probe = m_trd->probe(i);
                    fillEvolventaSignal(probe);
                    m_factors = new EvolventaFactors(testUid(), pi.uid);

//                    for (int i = 0; i < m_factors->size(); ++i)
//                    {
//                        addPrimaryFactor(testUid(), m_factors->factorUid(i),
//                                         m_factors->factorValue(m_factors->factorUid(i)),
//                                         0, ChannelsDefines::chanStab, pi.name);
//                    }
                }
            }
        }
    }
}

void EvolventaCalculator::fastCalculate()
{
    TestCalculator::fastCalculate();
}

int EvolventaCalculator::timeUpwinding() const
{
    if (m_erData)
        return m_erData->timeUpwinding();
    return 0;
}

int EvolventaCalculator::timeHold() const
{
    if (m_erData)
        return m_erData->timeHold();
    return 0;
}

int EvolventaCalculator::timeConvolution() const
{
    if (m_erData)
        return m_erData->timeConvolution();
    return 0;
}

int EvolventaCalculator::diap() const
{
    if (m_erData)
        return m_erData->diap();
    return 128;
}

int EvolventaCalculator::freq() const
{
    if (m_erData)
        return m_erData->freq();
    return 50;
}

int EvolventaCalculator::factorCount() const
{
    if (m_factors)
        return m_factors->size();
    return 0;
}

QString EvolventaCalculator::factorUid(const int id) const
{
    if (m_factors)
        return m_factors->factorUid(id);
    return QUuid().toString();
}

double EvolventaCalculator::factorValue(const int id) const
{
    if (m_factors)
        return m_factors->factorValue(id);
    return 0;
}

double EvolventaCalculator::factorValue(const QString &uid) const
{
    if (m_factors)
        return m_factors->factorValue(uid);
    return 0;
}

void EvolventaCalculator::fillEvolventaSignal(ProbeResultInfo *pri)
{
    auto *sigStab = pri->signal(ChannelsDefines::chanStab);
    auto *sigTarget = pri->signal(ChannelsDefines::chanTargetTraectory);

    m_frontal = new DecartCoordinatesSignal(ChannelsDefines::chanAnySignalDual, sigStab->frequency());
    m_sagittal = new DecartCoordinatesSignal(ChannelsDefines::chanAnySignalDual, sigStab->frequency());

    int cnt = static_cast<int>(fmin(sigStab->size(), sigTarget->size()));
    for (int i = 0; i < cnt; ++i)
    {
        double x = sigStab->value(0, i);
        double y = sigStab->value(1, i);
        double tx = sigTarget->value(0, i);
        double ty = sigTarget->value(1, i);

        m_frontal->addValue(x, tx);
        m_sagittal->addValue(y, ty);
    }
}

