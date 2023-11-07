#include "apnoefactors.h"

#include "baseutils.h"

#include <QDebug>

ApnoeFactors::ApnoeFactors(QVector<double> signal, const int begin, const int frequency, QObject *parent)
    : QObject(parent)
    , m_signal(signal)
    , m_begin(begin)
    , m_frequency(frequency)
{
    Q_ASSERT(frequency > 0);
    calculate();
}

int ApnoeFactors::semiWavesCount() const
{
    return m_semiWaves.size();
}

ApnoeFactors::SemiWave ApnoeFactors::semiWave(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_semiWaves.size());
    return m_semiWaves.at(idx);
}

int ApnoeFactors::apnoeListCount() const
{
    return m_apnoeList.size();
}

QString ApnoeFactors::apnoeListLine(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_apnoeList.size());
    return m_apnoeList.at(idx);
}

int ApnoeFactors::logCount() const
{
    return m_log.size();
}

QString ApnoeFactors::logLine(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_log.size());
    return m_log.at(idx);
}

QString boolToStr(const bool value)
{
    if (value)
        return "true";
    return "false";
}

void appendString(QString &str, const QString &s, const int spaceCnt)
{
    str = str + s;
    if (s.length() < spaceCnt)
        for (int i = s.length(); i < spaceCnt; ++i)
            str = str + " ";
}

void ApnoeFactors::calculate()
{
    m_apnoeList.clear();
    m_log.clear();

    if (m_signal.size() > 10)
    {
        computeSemiWavesParams();
        computeAmplitudes();
//        qDebug() << "----------------";
//        qDebug() << "ampl :" << m_minAmpl << m_midAmpl << m_maxAmpl << "  " << m_qAmpl;

        QString msgLog = QString("!   ");
        appendString(msgLog, "tExtr", 10);
        appendString(msgLog, "pointB", 8);
        appendString(msgLog, "pointE", 10);
        appendString(msgLog, "pointDiap", 10);
        appendString(msgLog, "pointBnd", 9);
        appendString(msgLog, "cond", 12);
        appendString(msgLog, "timeB", 8);
        appendString(msgLog, "timeE", 10);
        appendString(msgLog, "angle", 12);
        appendString(msgLog, "isDchSW", 8);
        appendString(msgLog, "isDelay", 8);

//        qDebug() << msgLog;
        m_log << msgLog;


        int begin = 0;
        bool isDelay = false;
        m_apnoeFactsCount = 0;
        m_apnoeFactTimeAverage = 0;
        m_apnoeFactTimeMax = 0;
        double d = 0; //TODO: лишнее
        int ws = 0;
        int iPrevTop = -1;
        foreach (auto sw, m_semiWaves)
        {
            bool isDchSW = (sw.amplitude / (sw.end - sw.begin) > 0.025); // && ((sw.end - sw.begin) < (7.0 * m_frequency / 2.0));

            msgLog = "    ";
            if (!isDchSW)
                msgLog = "*   ";
            appendString(msgLog, QString::number(sw.kind), 10);
            appendString(msgLog, QString::number(sw.begin), 8);
            appendString(msgLog, QString::number(sw.end), 10);
            appendString(msgLog, QString::number(sw.end - sw.begin), 10);
            appendString(msgLog, QString::number(7.0 * m_frequency / 2.0), 9);
            appendString(msgLog, boolToStr((sw.end - sw.begin) < (7.0 * m_frequency / 2.0)), 12);
            appendString(msgLog, BaseUtils::getTimeBySecCount(static_cast<int>(static_cast<double>(sw.begin / m_frequency))), 8);
            appendString(msgLog, BaseUtils::getTimeBySecCount(static_cast<int>(static_cast<double>(sw.end / m_frequency))), 10);
            appendString(msgLog, QString::number(sw.amplitude / (sw.end - sw.begin)), 12);
            appendString(msgLog, boolToStr(isDchSW), 8);
            appendString(msgLog, boolToStr(isDelay), 8);
//            qDebug() << msgLog;
            m_log << msgLog;

            //! Участок "пологий" и длительность полуволны меньше 3,5 секунд
            if (isDchSW)
            {
                if (isDelay)
                {
                    //! Общая длительность обнаруженного участка с "пологой" волной должна быть не меньше 8 секунд
                    double t = static_cast<double>(sw.end) / m_frequency - static_cast<double>(begin) / m_frequency;
                    if (t > 8)
                    {
                        QString msgApnoe = "";
                        appendString(msgApnoe, BaseUtils::getTimeBySecCount(static_cast<int>(static_cast<double>(begin + m_begin) / m_frequency)), 10);
                        appendString(msgApnoe, BaseUtils::getTimeBySecCount(static_cast<int>(static_cast<double>(sw.end + m_begin) / m_frequency)), 10);
                        appendString(msgApnoe, QString::number(t), 10);

//                        qDebug() << "------- " + msgApnoe;
                        m_log << "------- " + msgApnoe;
                        m_apnoeList << msgApnoe;

                        ++m_apnoeFactsCount;
                        m_apnoeFactTimeAverage += t;
                        if (t > m_apnoeFactTimeMax)
                            m_apnoeFactTimeMax = t;
                    }

                    isDelay = false;
                }
                begin = sw.begin;
            }
            else
                isDelay = true;

            d = sw.amplitude / (sw.end - sw.begin); //TODO: лишнее
            if (sw.kind == ekMaximum)
            {
                if (iPrevTop > -1 && begin > iPrevTop)
                    ws = begin - iPrevTop;
                iPrevTop = begin;
            }
        }

        if (m_apnoeFactsCount > 0)
            m_apnoeFactTimeAverage /= m_apnoeFactsCount;
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
    if (m_signal.at(1) > m_signal.at(0))
        dir = 1;
    else
    if (m_signal.at(1) < m_signal.at(0))
        dir = -1;

    for (int i = 1; i < m_signal.size(); ++i)
    {
        double v = m_signal.at(i);
        //qDebug() << v;

        ExtremumKind kind = ekUndefined;
        if (v - ov > 0 && dir != 1)
            kind = ekMinimum;
        else
        if (v - ov < 0 && dir != -1)
            kind = ekMaximum;

        if (kind == ekMaximum || kind == ekMinimum)
        {
            if (prevI > -1)
            {
                m_semiWaves << SemiWave(prevI, i, fabs(v - prevV), v, prevK);
//                qDebug() << static_cast<double>(i) << kind << v;
//                qDebug() << static_cast<double>(i) / m_frequency << kind << v;
            }

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
    //! Приводим амплитуды к диапазону 0 - 100 для того, чтоб сравнивать сравнимое
    double minA = INT_MAX;
    double maxA = 0;
    foreach (auto sw, m_semiWaves)
    {
        if (sw.amplitude < minA)
            minA = sw.amplitude;
        if (sw.amplitude > maxA)
            maxA = sw.amplitude;
    }
    for (int i = 0; i < m_semiWaves.size(); ++i)
        m_semiWaves[i].amplitude = (m_semiWaves.at(i).amplitude - minA) * 100.0 / maxA;

//    qDebug() << "-----" << m_semiWaves.size();
//    foreach (auto sw, m_semiWaves)
//    {
//        qDebug() << sw.amplitude << (sw.end - sw.begin) << "     " << sw.amplitude / (sw.end - sw.begin) << "     "
//                 << BaseUtils::getTimeBySecCount(static_cast<int>(static_cast<double>(sw.begin) / m_frequency));
//    }
//    qDebug() << "-----";

    //! Находим min, max, mid и СКО
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

//    qDebug() << m_minAmpl << m_maxAmpl << m_midAmpl << m_qAmpl;
}

