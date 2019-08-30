#include "idsfactors.h"

#include "dataprovider.h"
#include "channelsutils.h"
#include "aanalyserapplication.h"
#include "stabilogram.h"

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
        QVector<double> bufV; //! Буфер длин векторов линейной скорости
        QVector<double> bufW; //! Буфер значений угловой скорости

        if (stab.size() > 10)
        {
            //! Расчет буферов скорости и угловой скорости
            computeSpeedBuf(&stab, bufV, bufW);

            //! Расчет массива ФДС
            computeFDSBuf(bufV, bufW);

//            bufToFile(bufV, "C:/111/v.txt");
//            bufToFile(bufW, "C:/111/w.txt");
//            bufToFile(m_bufFDS, "C:/111/fds.txt");

            //! ИДС
            if (m_fdsQ > 0)
                m_ids = 100 - (log10(m_fdsQ) - 2) / 0.04;
            else
                m_ids = 0;
            if (m_ids > 100)
                m_ids = 100;

            if (m_fdsQ > 1)
                m_freq = pow(log10(pow(m_fdsQ, 2)), 3) * 3;
        }
    }

    addFactor(IDSFactorsDefines::IDSUid, m_ids);
    addFactor(IDSFactorsDefines::FDSDispUid, pow(m_fdsQ, 2));
    addFactor(IDSFactorsDefines::FreqUid, m_freq);
}

void IDSFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(IDSFactorsDefines::GroupUid, tr("Показатели динамической стабилизации"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(IDSFactorsDefines::IDSUid, IDSFactorsDefines::GroupUid,
                           tr("Индекс динамической стабилизации"), tr("ИДС"), tr("%"), 0);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(IDSFactorsDefines::FDSDispUid, IDSFactorsDefines::GroupUid,
                           tr("Дисперсия ФДС"), tr("Дисп.ФДС"), tr(""), 0);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(IDSFactorsDefines::FreqUid, IDSFactorsDefines::GroupUid,
                           tr("Частота звука"), tr("Freq"), tr("Гц"), 0);
}

double IDSFactors::fds(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_bufFDS.size());
    return m_bufFDS.at(idx);
}

void IDSFactors::computeSpeedBuf(Stabilogram *stab, QVector<double> &bufV, QVector<double> &bufW)
{
    bufV.resize(stab->size() - 1);
    bufW.resize(stab->size() - 1);
    double ox = 0;
    double oy = 0;
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

            double w = 0;
            if (fabs(x) > 0 && fabs(y) > 0)
                w = v / sqrt(x * x + y * y);
            bufW.replace(i - 1, w);
        }
        ox = x;
        oy = y;
    }
}

void IDSFactors::computeFDSBuf(const QVector<double> &bufV, const QVector<double> &bufW)
{
    m_bufFDS.resize(bufV.size() - 1);
    double ov = 0;
    double fdsMid = 0;
    for (int i = 0; i < bufV.size(); ++i)
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

