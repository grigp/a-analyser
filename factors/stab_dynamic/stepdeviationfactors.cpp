#include "stepdeviationfactors.h"

#include "dataprovider.h"
#include "channelsdefines.h"
#include "metodicsfactory.h"
#include "aanalyserapplication.h"
#include "stabilogram.h"

#include <QDebug>

StepDeviationFactors::StepDeviationFactors(const QString &testUid,
                                         const QString &probeUid,
                                         QObject *parent)
    : ProbeMultifactor (testUid, probeUid, parent)
{
    if (isValid())
        calculate();
}

bool StepDeviationFactors::isValid() const
{
    return isValid(testUid(), probeUid());
}

bool StepDeviationFactors::isValid(const QString &testUid, const QString &probeUid)
{
    Q_UNUSED(testUid);
    auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);

    bool te = false;
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        //! uid шаблона методики
        MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
        auto mi = metFactory->metodic(ti.metodUid);
        te = (mi.templateId == MetodicDefines::MetUid_StepDeviatoin);
    }

    return de && te;
}

void StepDeviationFactors::calculate()
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        //! Направление
        auto d = ti.params["direction"].toString();
        m_direction = BaseUtils::DirectionValueIndex.value(d);

        readSignal();
        assignSections();
    }
}

void StepDeviationFactors::registerFactors()
{

}

int StepDeviationFactors::signalSize() const
{
    return m_signal.size();
}

double StepDeviationFactors::signal(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_signal.size());
    return m_signal.at(idx);
}

double StepDeviationFactors::signalFiltred(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_signalFlt.size());
    return m_signalFlt.at(idx);
}

void StepDeviationFactors::readSignal()
{
    m_signal.clear();
    m_signalFlt.clear();

    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStab, baStab))
    {
        Stabilogram stab(baStab);
        m_freq = stab.frequency();

        for (int i = 0; i < stab.size(); ++i)
        {
            double v = 0;
            if (m_direction == BaseUtils::dirUp)
                v = stab.value(1, i);
            else
            if (m_direction == BaseUtils::dirDown)
                v = -stab.value(1, i);
            else
            if (m_direction == BaseUtils::dirRight)
                v = stab.value(0, i);
            else
            if (m_direction == BaseUtils::dirLeft)
                v = -stab.value(0, i);

            m_signal << v;
            m_signalFlt << v;
        }

        //! Фильтрация
        BaseUtils::filterLowFreq(m_signalFlt, stab.frequency(), 1, BaseUtils::fkChebyshev, 0, m_signalFlt.size() - 1);
        BaseUtils::swapVector(m_signalFlt);
        BaseUtils::filterLowFreq(m_signalFlt, stab.frequency(), 1, BaseUtils::fkChebyshev, 0, m_signalFlt.size() - 1);
        BaseUtils::swapVector(m_signalFlt);
    }
}

void StepDeviationFactors::assignSections()
{
    //! Построение списка экстремумов
    m_extrList.clear();

    if (m_signalFlt.size() < 10)
        return;

    int dir = 0;
    auto setDir = [&](const int i)
    {
        if (m_signalFlt[i] > m_signalFlt[i - 1])
            dir = 1;
        else
        if (m_signalFlt[i] < m_signalFlt[i - 1])
            dir = -1;
    };
    setDir(1);

    for (int i = 2; i < m_signalFlt.size(); ++i)
    {
        if ((m_signalFlt[i] - m_signalFlt[i-1] > 0) && (dir == -1))
        {
            BaseUtils::Extremum extr(i, BaseUtils::Minimum);
            m_extrList << extr;
        }
        if ((m_signalFlt[i] - m_signalFlt[i-1] < 0) && (dir == 1))
        {
            BaseUtils::Extremum extr(i, BaseUtils::Maximum);
            m_extrList << extr;
        }

        setDir(i);
    }

    //! Построение списка участков экстремумов
    m_Extr.clear();
    for (int i = 0; i < m_extrList.size(); ++i)
    {
        int p = 0;
        int c = m_extrList.at(i).pos;
        int n = m_signal.size() - 1;
        if (i > 0)
            p = m_extrList.at(i-1).pos;
        if (i < m_extrList.size() - 1)
            n = m_extrList.at(i+1).pos;
        int begin = static_cast<int>(c - (c - p) * 0.25);
        int end = static_cast<int>(c + (n - c) * 0.25);
        BaseUtils::Section sec(begin, end);
        m_Extr << sec;

//        //! Максимум всегда должен быть больше 0, а минимум любой
//        if (((extrList.at(i).kind == BaseUtils::Maximum) && (m_signal.at(extrList.at(i).pos) > 0)) ||
//                (extrList.at(i).kind == BaseUtils::Minimum))
//        {

//        }
    }

    //! Построение списка участков переходов
    m_Trans.clear();
    for (int i = 0; i < m_Extr.size() - 1; ++i)
    {
        int begin = m_Extr.at(i).end;
        int end = m_Extr.at(i+1).begin;
        BaseUtils::Section sec(begin, end);
        m_Trans << sec;
    }
}

