#include "idsfactors.h"

#include "dataprovider.h"
#include "channelsutils.h"
#include "aanalyserapplication.h"
#include "stabilogram.h"
#include "baseutils.h"

#include <math.h>
#include <QTextStream>
#include <QDebug>

IDSFactors::IDSFactors(const QString &testUid,
                       const QString &probeUid,
                       const QString &channelId,
                       QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, parent)
{
    if (isValid())
        calculate();
}

QString IDSFactors::uid() const
{
    return IDSFactorsDefines::GroupUid;
}

QString IDSFactors::name() const
{
    return IDSFactorsDefines::GroupName;
}

bool IDSFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool IDSFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctStabilogram;
}

void IDSFactors::calculate()
{
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), channelId(), baStab))
    {
        Stabilogram stab(baStab);
        m_freqDiskr = stab.frequency();
        QVector<double> bufV; //! Буфер длин векторов линейной скорости
        QVector<double> bufW; //! Буфер значений угловой скорости

        if (stab.size() > 10)
        {
            //! Расчет буферов скорости и угловой скорости
            computeSpeedBuf(&stab, bufV, bufW);

            //! Расчет массива ФДС
            computeFDSBuf(bufV, bufW);

//            bufToFile(bufV, "C:/a-analyser/temp/v.txt");
//            bufToFile(bufW, "C:/a-analyser/temp/w.txt");
//            bufToFile(m_bufFDS, "C:/a-analyser/temp/fds.txt");

            //! ИДС
            if (m_fdsQ > 0)
                m_ids = 100 - (log10(m_fdsQ) - 2) / 0.04;
            else
                m_ids = 0;
            if (m_ids > 100)
                m_ids = 100;

            if (m_fdsQ > 1)
                m_freq = pow(log10(pow(m_fdsQ, 2)), 3) * 3;

            //! Стабильность ФДС
            computeStabilityFDS();
            //! Асимметрия ФДС
            computeAsymmetryFDS();
        }
    }

    addFactor(IDSFactorsDefines::IDSUid, m_ids);
    addFactor(IDSFactorsDefines::FDSDispUid, pow(m_fdsQ, 2));
    addFactor(IDSFactorsDefines::FreqUid, m_freq);
    addFactor(IDSFactorsDefines::StabFDSUid, m_stabFDS);
    addFactor(IDSFactorsDefines::AsymFDSUid, m_asymFDS);
    addFactor(IDSFactorsDefines::KVarFDSUid, m_kvFDS);
}

void IDSFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(IDSFactorsDefines::GroupUid, IDSFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(IDSFactorsDefines::IDSUid, IDSFactorsDefines::GroupUid,
                           tr("Индекс динамической стабилизации"), tr("ИДС"), tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(IDSFactorsDefines::FDSDispUid, IDSFactorsDefines::GroupUid,
                           tr("Дисперсия ФДС"), tr("Дисп.ФДС"), tr(""), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(IDSFactorsDefines::FreqUid, IDSFactorsDefines::GroupUid,
                           tr("Частота звука"), tr("Freq"), tr("Гц"), 0, 2, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(IDSFactorsDefines::StabFDSUid, IDSFactorsDefines::GroupUid,
                           tr("Стабильность ФДС"), tr("Стаб.ФДС"), tr("%"), 1, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(IDSFactorsDefines::AsymFDSUid, IDSFactorsDefines::GroupUid,
                           tr("Асимметрия ФДС"), tr("Асм.ФДС"), tr("%"), 1, 2, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(IDSFactorsDefines::KVarFDSUid, IDSFactorsDefines::GroupUid,
                           tr("Коэффициент вариации ФДС"), tr("КВ ФДС"), tr("%"), 1, 2, FactorsDefines::nsDual, 12);
}

double IDSFactors::fds(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_bufFDS.size());
    return m_bufFDS.at(idx);
}

void IDSFactors::computeSpeedBuf(Stabilogram *stab, QVector<double> &bufV, QVector<double> &bufW)
{
    bufV.resize(stab->size() - 1);
    bufW.resize(stab->size() - 2);
    double ox = 0;
    double oy = 0;
    double oa = 0;
    for (int i = 0; i < stab->size(); ++i)
    {
        double x = stab->value(i).x;
        double y = stab->value(i).y;

        if (i > 0)
        {
            double vx = (x - ox) * stab->frequency();
            double vy = (y - oy) * stab->frequency();
            double v = sqrt(pow(vx, 2) + pow(vy, 2));
            bufV.replace(i - 1, v);

            double a = BaseUtils::getAngleByCoord(vx, vy);
            if (i > 1)
            {
                double da = BaseUtils::getDifferenceAngles(oa, a);
                bufW.replace(i - 2, da * stab->frequency());
            }
            oa = a;
        }
        ox = x;
        oy = y;
    }
}

void IDSFactors::computeFDSBuf(const QVector<double> &bufV, const QVector<double> &bufW)
{
    m_bufFDS.resize(bufW.size() - 1);
    double ov = 0;
    double fdsMid = 0;
    for (int i = 0; i < bufW.size(); ++i)
    {
        double v = bufV.at(i);
        double w = bufW.at(i);
        if (i > 0)
        {
            double fds = (w / 2) * (v * v + v * (v - ov) + pow(v - ov, 2) / 3);
            m_bufFDS.replace(i - 1, fds);
            fdsMid = fdsMid + fds;
        }

        ov = v;
    }
    fdsMid = fdsMid / m_bufFDS.size();

    //! СКО ФДС
    foreach (auto fds, m_bufFDS)
        m_fdsQ = m_fdsQ + pow(fdsMid - fds, 2);
    if (m_bufFDS.size() - 1 > 0)
        m_fdsQ = sqrt(m_fdsQ / (m_bufFDS.size() - 1));
}

void IDSFactors::computeStabilityFDS()
{
    double mid = 0;
    double mid1 = 0;
    QVector<double> mid1Arr;
    for (int i = 0; i < m_bufFDS.size(); ++i)
    {
        //! Расчет общего среднего
        double v = fabs(m_bufFDS.at(i) / 2500);
        mid = mid + v;

        //! Расчет посекундного среднего
        if (i > 0 && (i % m_freqDiskr) == 0)
        {
            mid1 = mid1 / m_freqDiskr;
            mid1Arr.append(mid1);
            mid1 = 0;
        }
        else
            mid1 = mid1 + v;
    }
    mid = mid / m_bufFDS.size();

    //! Нахождение разницы между общим средним и средними за каждую секунду и средней разницы
    QVector<double> deltaArr;
    deltaArr.resize(mid1Arr.size());
    double delta = 0;
    for (int i = 0; i < mid1Arr.size() - 1; ++i)
    {
        deltaArr.replace(i, fabs(mid - mid1Arr.at(i)));
        delta = delta + deltaArr.at(i);
    }
    delta = delta / mid1Arr.size();

    double deltaQ = 0;
    for (int i = 0; i < deltaArr.size(); ++i)
        deltaQ = deltaQ + pow(deltaArr.at(i) - delta, 2) / static_cast<double>(deltaArr.size() - 1);
    deltaQ = sqrt(deltaQ);

    if (mid > delta)
        m_stabFDS = 100 - delta * 100 / mid;
    if (delta > 0)
        m_kvFDS = deltaQ / delta * 100;
}

void IDSFactors::computeAsymmetryFDS()
{
    double sPlus = 0;
    double sMinus = 0;
    for (int i = 0; i < m_bufFDS.size(); ++i)
    {
        double v = m_bufFDS.at(i);
        if (v > 0)
            sPlus = sPlus + v;
        else
            sMinus = sMinus + fabs(v);
    }
    m_asymFDS = (sMinus - sPlus) / (sPlus + sMinus) * 100;
}

void IDSFactors::bufToFile(const QVector<double> &buf, const QString &fn)
{
    QFile file(fn);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream fs(&file);
        foreach (auto val, buf)
            fs << QString::number(val) << "\n";
        file.close();
    }
}

