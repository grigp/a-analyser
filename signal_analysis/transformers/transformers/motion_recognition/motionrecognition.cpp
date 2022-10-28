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
    //! Частота дискретизации
    auto freqSample = params["freq_sample"].toInt(50);

    //! Исходный сигнал - во внутренний буфер
    QVector<double> buf;
    buf.clear();
    foreach (auto v, buffer)
        buf << v;
    buffer.clear();

    //! Расчет общего СКО
    double moAll = 0;
    double qAll = 0;
    computeMQ(buf, 0, buf.size(), moAll, qAll);

    //! Расчет динамики СКО
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

    //! Фильтрация динамики СКО
    BaseUtils::filterLowFreq(m_sko, freqSample, 0.7, BaseUtils::fkCriticalAttenuation, 0, m_sko.size() - 1);
    BaseUtils::swapVector(m_sko);
    BaseUtils::filterLowFreq(m_sko, freqSample, 0.7, BaseUtils::fkCriticalAttenuation, 0, m_sko.size() - 1);
    BaseUtils::swapVector(m_sko);

    //! Поиск на динамике СКО участков превышения общего СКО
    bool above = false;
    int aib = 0;
    int aie = 0;
    m_partsMotions.clear();
    for (int i = 0; i < m_sko.size(); ++i)
    {
        auto v = m_sko.at(i);
        if (!above)
        {
            if (v > qAll) // * 3)
            {
                aib = i;
                above = true;
            }
        }
        else
        {
            if (v < qAll * 0.5) // * 3)
            {
                aie = i;
                m_partsMotions << Part(aib, aie);
                above = false;
            }
        }
    }

    //! Поиск участков, между которыми меньше 10 секунд. Их надо объединить
    QVector<int> toDelete;
    int i = 0;
    while (i < m_partsMotions.size() - 1)
    {
        if ((m_partsMotions.at(i+1).begin - m_partsMotions.at(i).end) < freqSample * 10)
        {
            m_partsMotions.replace(i, Part(m_partsMotions.at(i).begin, m_partsMotions.at(i+1).end));
            toDelete << i+1;
            ++i;
        }
        ++i;
    }
    //! Удаление участков
    foreach (auto idx, toDelete)
        m_partsMotions.removeAt(idx);

    //! Заполнение буфера преобразованного сигнала в зависимости от участка
    //! 1 - превышение, есть двигательная активность
    //! 0 - нет превышения, человек в покое
    int n = 0;
//    qDebug() << m_sko.size();
//    qDebug() << "-------------";
    for (int i = 0; i < m_sko.size(); ++i)
    {
        Part part(0, 0);
        if (n < m_partsMotions.size())
            part = m_partsMotions.at(n);
//        qDebug() << i << "  " << part.begin << part.end << "  " << n << m_parts.size();
        if (i >= part.begin && i <= part.end)
        {
            if (qAll >= 0.15)
                buffer << m_presentMotion;
            else
                buffer << m_noMotion;
        }
        else
            buffer << m_noMotion;
        if (i == part.end)
            ++n;
    }

    for (int i = 0; i < freqSample; ++i)
        buffer << m_noMotion;

    //! Создание списка участков без двигательной активности
    m_partsNoMotions.clear();
    if (qAll >= 0.15)
    {
        aie = -1;
        foreach (auto part, m_partsMotions)
        {
            if (part.begin != 0 && m_partsNoMotions.size() == 0)
                m_partsNoMotions << Part(0, part.begin);
            if (aie > -1)
                m_partsNoMotions << Part(aie, part.begin);
            aie = part.end;
        }
        if (aie < m_sko.size() - 1)
            m_partsNoMotions << Part(aie, m_sko.size() + freqSample - 1);
    }
    else
        m_partsNoMotions << Part(0, m_sko.size() + freqSample - 1);
}

SignalTransformerParamsWidget *MotionRecognition::getParamsWidget()
{
    return nullptr;
}

void MotionRecognition::setValues(const int noMotion, const int presentMotion)
{
    m_noMotion = noMotion;
    m_presentMotion = presentMotion;
}

int MotionRecognition::partsNoMotionCount() const
{
    return m_partsNoMotions.size();
}

MotionRecognition::Part MotionRecognition::partNoMotion(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_partsNoMotions.size());
    return m_partsNoMotions.at(idx);
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
