#include "motionrecognition.h"

#include <QDebug>

#include "baseutils.h"
#include "signaltransformerdefines.h"

MotionRecognition::MotionRecognition()
{

}

QString MotionRecognition::uid()
{
    return SignalTransformerDefines::MotionRecognition_UID;
}

QString MotionRecognition::name()
{
    return SignalTransformerDefines::MotionRecognition_name;
}

void MotionRecognition::transform(QVector<double> &buffer, const QJsonObject &params)
{
    auto freqSample = params["freq_sample"].toInt(50);

    QVector<double> buf;
    buf.clear();
    foreach (auto v, buffer)
        buf << v;
    buffer.clear();
    for (int i = 0; i < freqSample / 2; ++i)
        buffer << buf[i]; //0;

    double moAll = 0;
    double qAll = 0;
    computeMQ(buf, 0, buf.size(), moAll, qAll);

    int begin = 0;
    m_sko.clear();
    while (begin + freqSample < buf.size())
    {
        double m = 0;
        double q = 0;
        computeMQ(buf, begin, begin + 100, m, q);
        m_sko << q;

        begin += 1; //freqSample;
    }

    BaseUtils::filterLowFreq(m_sko, freqSample, 0.7, BaseUtils::fkCriticalAttenuation, 0, m_sko.size() - 1);
    BaseUtils::swapVector(m_sko);
    BaseUtils::filterLowFreq(m_sko, freqSample, 0.7, BaseUtils::fkCriticalAttenuation, 0, m_sko.size() - 1);
    BaseUtils::swapVector(m_sko);

    bool above = false;
    int aib = 0;
    int aie = 0;
    m_parts.clear();
    foreach (auto v, m_sko)
    {
        if (!above)
        {
            buffer << 0;
            if (v > qAll) // * 3)
            {
                aib = begin;
                above = true;
            }
        }
        else
        {
            buffer << 1;
            if (v < qAll) // * 3)
            {
                aie = begin;
                m_parts << Part(aib, aie);
                qDebug() << aib / static_cast<double>(freqSample) << aie / static_cast<double>(freqSample);
                above = false;
            }
        }
    }


    qDebug() << moAll << qAll << freqSample;
}

SignalTransformerParamsWidget *MotionRecognition::getParamsWidget()
{
    return nullptr;
}

void MotionRecognition::computeMQ(QVector<double> &buffer, const int begin, const int end, double &m, double &q)
{
    int n = end - begin;
    if (n > 0)
    {
        for (int i = begin; i < end; ++i)
            m += buffer[i];
        m /= n;

        for (int i = begin; i < end; ++i)
            q += pow(fabs(buffer[i] - m), 2) / (n - 1);
        q = sqrt(q);
    }
}
