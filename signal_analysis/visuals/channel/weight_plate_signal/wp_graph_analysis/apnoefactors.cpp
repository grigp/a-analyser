#include "apnoefactors.h"

#include "baseutils.h"

#include <QDebug>

ApnoeFactors::ApnoeFactors(QVector<double> signal, QObject *parent)
    : QObject(parent)
    , m_signal(signal)
{
    calculate();
}

void ApnoeFactors::calculate()
{
    if (m_signal.size() > 10)
    {
        computeSemiWavesParams();
        computeAmplitudes();
        qDebug() << "----------------";
        qDebug() << "ampl :" << m_minAmpl << m_midAmpl << m_maxAmpl << "  " << m_qAmpl;

        int begin = 0;
        bool isDelay = false;
        foreach (auto sw, m_semiWaves)
        {
            if (sw.amplitude >= m_midAmpl - m_qAmpl)
            {
                if (isDelay)
                {
                    qDebug() << BaseUtils::getTimeBySecCount(static_cast<int>(begin / 150.0)) <<
                                BaseUtils::getTimeBySecCount(static_cast<int>(sw.end / 150.0));
                    isDelay = false;
                }
                begin = sw.begin;
            }
            else
                isDelay = true;
//            QString s = "";
//            if (sw.amplitude < m_midAmpl - m_qAmpl)
//                s = "no";
//            qDebug() << sw.begin / 150.0 << " " << sw.amplitude << (m_midAmpl - m_qAmpl) << "    "
//                     << s;
        }
    }
}

void ApnoeFactors::computeSemiWavesParams()
{
    m_semiWaves.clear();

    double ov = m_signal.at(0);
    int prevI = -1;
    double prevV = 0;
    ExtremumKind prevK = ekUndefined;
    int dir = 0;
    for (int i = 1; i < m_signal.size(); ++i)
    {
        double v = m_signal.at(i);

        ExtremumKind kind = ekUndefined;
        if (v - ov > 0 && dir != 1)
            kind = ekMinimum;
        else
        if (v - ov < 0 && dir != -1)
            kind = ekMaximum;

        if (kind == ekMaximum || kind == ekMinimum)
        {
            if (prevI > -1)
                m_semiWaves << SemiWave(prevI, i, fabs(v - prevV), prevK);

            prevI = i;
            prevV = v;
            prevK = kind;
        }

        dir = static_cast<int>((v - ov) / fabs(v - ov));
        ov = v;
    }
}

void ApnoeFactors::computeAmplitudes()
{
    m_minAmpl = INT_MAX;
    m_maxAmpl = 0;
    m_midAmpl = 0;

    foreach (auto sw, m_semiWaves)
    {
        if (sw.amplitude < m_minAmpl)
            m_minAmpl = sw.amplitude;
        if (sw.amplitude > m_maxAmpl)
            m_maxAmpl = sw.amplitude;
        m_midAmpl += sw.amplitude;
    }
    m_midAmpl /= m_semiWaves.size();

    m_qAmpl = 0;
    foreach (auto sw, m_semiWaves)
        m_qAmpl += pow(fabs(sw.amplitude - m_midAmpl), 2) / (m_semiWaves.size() - 1);
    m_qAmpl = sqrt(m_qAmpl);
}

