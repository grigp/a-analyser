#include "targetfactors.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "channelsutils.h"
#include "stabilogram.h"
#include "stabtestparams.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

TargetFactors::TargetFactors(const QString &testUid,
                             const QString &probeUid,
                             QObject *parent)
    : ProbeMultifactor(testUid, probeUid, parent)
{
    if (isValid())
        calculate();
}

QString TargetFactors::uid() const
{
    return TargetFactorsDefines::GroupUid;
}

QString TargetFactors::name() const
{
    return TargetFactorsDefines::GroupName;
}

bool TargetFactors::isValid() const
{
    return isValid(testUid(), probeUid(), ChannelsDefines::chanStab);
}

bool TargetFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(channelId);
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        //! Номер пробы
        auto num = ti.probes.indexOf(probeUid);
        if (num > -1)
        {
            //! Параметры теста
            auto params = ti.params;
            auto probes = params["probes"].toArray();
            if (num >= 0 && num < probes.size())
            {
                //! В параметрах пробы находим код стимуляции
                auto stimul = probes.at(num).toObject()["stimul"].toInt();
                return stimul == 3;
            }
        }
    }

    return false;
}

void TargetFactors::calculate()
{
    for (int i = 0; i < 10; ++i)
        m_hist[i] = 0;

    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStab, baStab))
    {
        Stabilogram stab(baStab);

        double diap = getDiap(&stab);
        double step = diap / 10.0;

        //! Расчет гистограммы
        for (int i = 0; i < stab.size(); ++i)
        {
            auto rec = stab.value(i);
            double val = sqrt(pow(rec.x/* - offsetX*/, 2) + pow(rec.y/* - offsetY*/, 2));
            int idx = 9 - trunc(val / step);
            if (idx < 0)
                idx = 0;
            if (idx > 9)
                idx = 9;
            ++m_hist[idx];
        }

        //! Пересчет в процентах
        double max = 0;
        for (int i = 0; i < 10; ++i)
            m_score = m_score + m_hist[i];
        for (int i = 0; i < 10; ++i)
        {
            m_totalScore = m_totalScore + m_hist[i] * 0.1 * (i + 1);
            if (m_score != 0)
                m_hist[i] = m_hist[i] / m_score * 100;
            else
                m_hist[i] = 0;
            if (m_hist[i] > max)
                max = m_hist[i];
        }

        //! Расчет кол-ва очков
        m_score = 0;
        for (int i = 0; i < 10; ++i)
            m_score = m_score + m_hist[i] * 0.1 * (i + 1);
    }

    addFactor(TargetFactorsDefines::TargetScoreUid, m_score);
    addFactor(TargetFactorsDefines::TargetTotalScoreUid, m_totalScore);
}

void TargetFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(TargetFactorsDefines::GroupUid, TargetFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TargetFactorsDefines::TargetScoreUid, TargetFactorsDefines::GroupUid,
                           tr("Количество набранных очков"), tr("Очки"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TargetFactorsDefines::TargetTotalScoreUid, TargetFactorsDefines::GroupUid,
                           tr("Суммарный балл"), tr("Сумм. балл"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
}

int TargetFactors::histogram(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < 10);
    return m_hist[idx];
}

int TargetFactors::getDiap(Stabilogram *stab) const
{
    Q_UNUSED(stab);
    int retval = 1;

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        StabTestParams::StabTestParams params(ti.params);
        if (params.probes.size() == 1)
        {
            auto scale = params.probes.at(0).scale;
            for (int i = 0; i < scale; ++i)
                retval *= 2;
        }
    }
    return 128 / retval;

//    double max = 0;
//    for (int i = 0; i < stab->size(); ++i)
//    {
//        auto val = stab->value(i);
//        if (fabs(val.x) > max)
//            max = fabs(val.x);
//        if (fabs(val.y) > max)
//            max = fabs(val.y);
//    }
//    int retval = 1;
//    while (retval < max)
//        retval = retval * 2;
//    return retval;
}
