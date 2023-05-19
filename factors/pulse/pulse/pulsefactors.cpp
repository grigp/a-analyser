#include "pulsefactors.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "ritmogram.h"

namespace
{


}

PulseFactors::PulseFactors(const QString &testUid,
                           const QString &probeUid,
                           const QString &channelId,
                           const int begin,
                           const int end,
                           QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, begin, end, parent)
{
    if (isValid())
        calculate();
}

QString PulseFactors::uid() const
{
    return PulseFactorsDefines::GroupUid;
}

QString PulseFactors::name() const
{
    return PulseFactorsDefines::GroupName;
}

bool PulseFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool PulseFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctRitmogram;
}

void PulseFactors::calculate()
{
    QByteArray baPulse;
    if (DataProvider::getChannel(probeUid(), channelId(), baPulse))
    {
        Ritmogram signal(baPulse);

        if (signal.size() > 0)
        {
            assignStat(50, 0.0, 2000.0);
//            //! Средний пульс
//            m_pulse = 0;
//            for (int i = 0; i < signal.size(); ++i)
//                m_pulse += signal.value(i);
//            m_pulse /= signal.size();

            //! Начальная и конечная точки
            m_begin = 0;
            m_end = signal.size() - 1;
            if (begin() < end())
            {
                if (begin() > -1)
                    m_begin = begin();
                if (end() > -1)
                    m_end = end();
            }

            //! Добавление значений и предварительная обработка
            double vd = 0;
            for (int i = m_begin; i <= m_end; ++i)
            {
                double v = signal.value(i);
                if (v > 0)
                {
                    vd += (60.0 / v);
                    //qDebug() << Q_FUNC_INFO << v << vd;
                    addRRValue(60.0 / v, vd);
                }
                else
                {
                    addRRValue(0, vd);
                }
            }
            finalCalculate();

            computeAKA(&signal);
            computeConslution();
        }
    }

    addFactor(PulseFactorsDefines::PulseUid, m_HRM);
    addFactor(PulseFactorsDefines::IVRUid, m_IVR);
    addFactor(PulseFactorsDefines::PAPRUid, m_PAPR);
    addFactor(PulseFactorsDefines::VPRUid, m_VPR);
    addFactor(PulseFactorsDefines::INUid, m_INNPR);
    addFactor(PulseFactorsDefines::ModeUid, m_xMod);
    addFactor(PulseFactorsDefines::AModeUid, m_ampMod);
    addFactor(PulseFactorsDefines::MxDMnUid, m_MxDMn);
    addFactor(PulseFactorsDefines::SDNNUid, m_SDNN);
    addFactor(PulseFactorsDefines::RMSSDUid, m_RMSSD);
    addFactor(PulseFactorsDefines::pNN50Uid, m_pNN50);
    addFactor(PulseFactorsDefines::CVUid, m_CVR);
    addFactor(PulseFactorsDefines::SDSDUid, m_SDSD);
    addFactor(PulseFactorsDefines::MDUid, m_MDNN);
    addFactor(PulseFactorsDefines::AKACC0Uid, m_ACK1);
    addFactor(PulseFactorsDefines::DXUid, m_AKSh0);
    addFactor(PulseFactorsDefines::AKACClUid, m_DXRange * 1000.0);
}

void PulseFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(PulseFactorsDefines::GroupUid, PulseFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::PulseUid, PulseFactorsDefines::GroupUid,
                           tr("Средний пульс"), tr("ЧСС"), tr("уд/мин"), 0, 1, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::IVRUid, PulseFactorsDefines::GroupUid,
                           tr("Индекс вариационного размаха"), tr("ИВР"), tr(""), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::PAPRUid, PulseFactorsDefines::GroupUid,
                           tr("Показатель адекватности процессов регуляции"), tr("ПАПР"), tr(""), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::VPRUid, PulseFactorsDefines::GroupUid,
                           tr("Вегетативный показатель ритма"), tr("ВПР"), tr(""), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::INUid, PulseFactorsDefines::GroupUid,
                           tr("Индекс напряжения систем регуляции"), tr("ИН"), tr(""), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::ModeUid, PulseFactorsDefines::GroupUid,
                           tr("Мода"), tr("Мо"), tr("сек"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::AModeUid, PulseFactorsDefines::GroupUid,
                           tr("Амплитуда моды"), tr("АМо"), tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::MxDMnUid, PulseFactorsDefines::GroupUid,
                           tr("Разность между макс. и мин. значениями"), tr("MxDMn"), tr("мс"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::SDNNUid, PulseFactorsDefines::GroupUid,
                           tr("Станд. отклонение массива кардиоинтервалов"), tr("SDNN"), tr("мс"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::RMSSDUid, PulseFactorsDefines::GroupUid,
                           tr("Активность парасим. звена вегетат. регуляции"), tr("RMSSD"), tr(""), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::pNN50Uid, PulseFactorsDefines::GroupUid,
                           tr("Показатель преобладания парасимп. звена"), tr("pNN50"), tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::CVUid, PulseFactorsDefines::GroupUid,
                           tr("Коэффициент вариации"), tr("KV"), tr("%"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::SDSDUid, PulseFactorsDefines::GroupUid,
                           tr("Станд. откл. разницы между соседними RR"), tr("SDSD"), tr("мс"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::MDUid, PulseFactorsDefines::GroupUid,
                           tr("Средняя разница между соседними RR"), tr("MD"), tr("мс"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::AKACClUid, PulseFactorsDefines::GroupUid,
                           tr("Первый коэффициент автокоррел. функции"), tr("АСК1"), tr("мс"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::AKACC0Uid, PulseFactorsDefines::GroupUid,
                           tr("Число сдвигов автокор. функции до нуля"), tr("ACSh0"), tr(""), 3, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseFactorsDefines::DXUid, PulseFactorsDefines::GroupUid,
                           tr("Вариационный размах"), tr("DX"), tr(""), 0, 1, FactorsDefines::nsDual, 12);
}

int PulseFactors::statBoundsCount() const
{
    return m_bStatBnd.size();
}

double PulseFactors::statBounds(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_bStatBnd.size());
    return m_bStatBnd.at(idx);
}

int PulseFactors::statCountsCount() const
{
    return m_bStatCnt.size();
}

double PulseFactors::statCounts(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_bStatCnt.size());
    return m_bStatCnt.at(idx);
}

double PulseFactors::correlationCoef(const int idx)
{
    Q_ASSERT(idx >= 0 && idx < PulseFactorsDefines::iAKPointsCount);
    return m_masKK[idx];
}

void PulseFactors::assignStat(const double sdt, const double tMin, const double tMax)
{
    m_bStatBnd.clear();
    m_bStatCnt.clear();
    if (sdt > 0)
        m_statSize = static_cast<int>(ceil((tMax - tMin) / sdt));
    else
        m_statSize = 0;
    m_bStatBnd.resize(m_statSize + 1);
    m_bStatCnt.resize(m_statSize);
    for (int i = 0; i < m_statSize; ++i)
        m_bStatBnd[i] = i * sdt + tMin;
}

void PulseFactors::addRRValue(const double val, const double t)
{
    Q_UNUSED(t);
    ++m_nCnt;
    //! Минимум и максимум
    if (val < m_minNN)
        m_minNN = val;
    if (val > m_maxNN)
        m_maxNN = val;

    if (m_prevNN > -1)
    {
        double v2 = val - m_prevNN;
        m_RMSSD += (pow(v2, 2));
        if (fabs(v2) > 50.0)
            ++m_NN50;

        if (m_nCnt % 2 == 0)
        {
            m_meanD1 += v2;
            m_meanD2 += (pow(v2, 2));
            ++m_nD2Cnt;
        }
        m_MDNN += fabs(v2);

        m_bscC11 += m_prevNN;
        m_bscC12 += 1;
        m_bscC21 += (pow(m_prevNN, 2));
        m_bscY1 += val;
        m_bscY2 += (val * m_prevNN);
        m_bscYV2 += pow(val, 2);
    }

    m_meanNN += val;
    m_mean2NN += pow(val, 2);

    m_prevNN = val;

    //! Формирование гистограммы
      if (val < m_bStatBnd[0])
      {
        ++m_outerMin;
      }
      else
      if (val > m_bStatBnd[m_statSize])
      {
        ++m_outerMax;
      }
      else
      {
          for (int i = 0; i < m_statSize; ++i)
          {
              if ((val >= m_bStatBnd[i]) && (val <= m_bStatBnd[i+1]))
              {
                 m_bStatCnt[i] += 1;
                 break;
              }
          }
      }
}

void PulseFactors::finalCalculate()
{
    if (m_nCnt > 0)
    {
        m_meanNN /= m_nCnt;
        m_mean2NN /= m_nCnt;

        //! Стандартная процедура последовательного расчета
        m_SDNN = sqrt(fabs(m_mean2NN - m_meanNN * m_meanNN));

        if (m_meanNN > 0)
        {
            m_CVR = (m_SDNN / m_meanNN) * 100.0;
            m_HRM = 60.0 / m_meanNN; //(1000.0 / m_meanNN) * 60.0;
        }
        else
        {
            m_CVR = 0;
            m_HRM = 0;
        }

        if (m_nCnt > 1 )
        {
            m_RMSSD = sqrt(m_RMSSD / (m_nCnt - 1));
            if (m_nD2Cnt != 0)
            {
                m_meanD1 = m_meanD1 / m_nD2Cnt;
                m_meanD2 = m_meanD2 / m_nD2Cnt;
            }
            else
            {
                m_meanD1 = 0;
                m_meanD2 = 0;
            }
            m_SDSD = sqrt(fabs(m_meanD2 - m_meanD1 * m_meanD1));
            m_MDNN = m_MDNN / (m_nCnt - 1);
            m_pNN50 = 100.0 * (m_NN50 / m_nCnt);
        }

        processHist();

        m_MxDMn = m_maxNN - m_minNN; //1000.0*((60.0/aStat.MinNN) - (60.0/aStat.MinNN));
    }
}

void PulseFactors::processHist()
{
    //! Обработка гистограммы (по Баевскому)
    int v = 0;
    int iv = -1;
    m_ampMod = 0;
    if ((m_statSize == 0) || (m_nCnt == 0))
        return;
    for (int i = 0; i < m_statSize; ++i)
    {
        if (m_bStatCnt[i] > v)
        {
           v = m_bStatCnt[i];
           iv = i;
           m_xMod = (m_bStatBnd[i] + m_bStatBnd[i+1]) / 2.0;
        }
    }

    //! Получили амплитуду моды AMo
    m_ampMod = v;
    if (m_nCnt != 0)
        m_ampMod = (m_ampMod / static_cast<double>(m_nCnt)) * 100;

    m_DXRange = (m_bStatBnd[m_statSize] - m_bStatBnd[0]) / 1000.0;

    if (iv < 0) return;

    //! Обнаружение начала и конца вариационного интервала
    int i = iv;
    int i1 = i;
    while (i > 0)
    {
        if (m_bStatCnt[i-1] == 0) break;
        --i;
        i1 = i;
    }

    i = iv;
    int i2 = i;
    while (i < m_statSize - 1)
    {
        if (m_bStatCnt[i+1] == 0) break;
        ++i;
        i2 = i;
    }

    m_DXRange = (m_bStatBnd[i2] - m_bStatBnd[i1]) / 1000.0;
    m_xMod = m_xMod / 1000.0;

    if (m_DXRange > 0)
        m_IVR = m_ampMod / m_DXRange;
    else
        m_IVR = 0;
    if (m_xMod * m_DXRange > 0)
        m_VPR = 1.0 / (m_xMod * m_DXRange);
    else
        m_VPR = 1;
    if (m_xMod > 0 )
        m_PAPR = m_ampMod / m_xMod;
    else
        m_PAPR = 0;
    if (m_DXRange * m_xMod > 0)
        m_INNPR = m_ampMod / (2.0 * m_DXRange * m_xMod);
    else
        m_INNPR = m_ampMod;

    //! Вычисляем FSIM
    double v2 = m_bStatCnt[iv] * 0.2;
    i1 = iv;
    i2 = iv;
    for (int i = 0; i <= iv; ++i)
    {
        if (m_bStatCnt[i] >= v2)
        {
            i1 = i;
            break;
        }
    }

    for (int i = m_statSize - 1; i >= iv; --i)
    {
        if (m_bStatCnt[i] >= v2)
        {
            i2 = i;
            break;
        }
    }

    m_SIM = (m_bStatBnd[i2 + 1] - m_bStatBnd[i1]) / 1000.0;
    if (m_SIM > 0)
        m_SIM = m_ampMod * 4.0 / m_SIM;

    //! Вычисляем FPAR
    QVector<int> va;
    va.resize(m_statSize);

    for (int i = 0; i < m_statSize; ++i)
        va[i] = m_bStatCnt[i];

    //! Упорядочить по убыванию (?)
    for (int i = iv - 1; i >= 0; --i)
    {
        for (int j = 0; j <= i; ++j)
        {
            if (va[j] > va[i])
            {
               v = va[i];
               va[i] = va[j];
               va[j] = v;
            }
        }
    }

    v = 0;
    for (int i = 0; i < m_statSize; ++i)
    {
        v = v + abs(m_bStatCnt[i] - va[i]);
    }

    if (m_nCnt > 0)
        m_PAR = (static_cast<double>(v) / static_cast<double>(m_nCnt)) * 100;
    else
        m_PAR = 0;
}

void PulseFactors::computeConslution()
{
    if (m_HRM > 0)
    {
      //! Суммарный эффект регуляции
      if ((60 / m_HRM <= 0.66) && (60 / m_HRM > 0.5))
        m_summReg = PulseFactorsDefines::oerModerateTachycardia;
      else
      if (60 / m_HRM <= 0.5)
        m_summReg = PulseFactorsDefines::oerSevereTachycardia;
      else
      if ((60 / m_HRM >= 1.0) && (60 / m_HRM < 1.2))
        m_summReg = PulseFactorsDefines::oerModerateBradycardia;
      else
      if (60 / m_HRM >= 1.2)
        m_summReg = PulseFactorsDefines::oerSevereBradycardia;
    }

    //! Функция автоматизма
    if ((m_SDNN <= 0.02) && (m_maxNN - m_minNN <= 0.1) && (m_CVR <= 2))
        m_funcAuto = PulseFactorsDefines::afRigidRhythm;
    if ((m_SDNN >= 0.1) && (m_maxNN - m_minNN >= 0.3 * m_xMod) && (m_CVR >= 8))
        m_funcAuto = PulseFactorsDefines::afSevereSinusArrhythmia;
    if (m_maxNN - m_minNN >= 0.45 * m_xMod)
        m_funcAuto = PulseFactorsDefines::afModerateAutoDisorder;
    if ((m_SDNN >= 0.1) and (m_maxNN - m_minNN >= 0.6 * m_xMod) && (m_CVR >= 8))
        m_funcAuto = PulseFactorsDefines::afPronDisturbAuto;

    //! Вегетативный гомеостаз
    if ((m_maxNN - m_minNN <= 0.06) && (m_INNPR >= 500))
        m_vegHomst = PulseFactorsDefines::vhSeverePredominanceSNS;
    if ((m_maxNN - m_minNN <= 0.15) && (m_INNPR >= 200))
        m_vegHomst = PulseFactorsDefines::vhModeratePredominanceSNS;
    if ((m_maxNN - m_minNN >= 0.3) && (m_INNPR <= 50))
        m_vegHomst = PulseFactorsDefines::vhModeratePredominancePSNS;
    if ((m_maxNN - m_minNN >= 0.5) && (m_INNPR <= 25))
        m_vegHomst = PulseFactorsDefines::vhSeverePredominancePSNS;

    //! Устойчивость регуляции
    if (m_CVR <= 3)
        m_ustReg = PulseFactorsDefines::srCentralDysregulation;
    if ((m_CVR <= 3) && (m_INNPR >= 200) && (m_xMod >= 0.8))
        m_ustReg = PulseFactorsDefines::srDominatedDysregulationSNS;
    if ((m_CVR >= 6) && (m_INNPR <= 50) && (m_xMod <= 0.8))
        m_ustReg = PulseFactorsDefines::srDysregulationPredominancePSNS;
    if ((m_CVR >= 10) && (m_INNPR <= 25))
        m_ustReg = PulseFactorsDefines::srTransitionProcess;
}

void PulseFactors::computeAKA(Ritmogram* signal)
{
    if (signal->maxValue() - signal->minValue() < 10000)
    {
        //! Инициализация
        for (int i = 0; i < PulseFactorsDefines::iAKPointsCount; ++i)
            m_masKK[i] = 0.0;

        //! Расчет суммы квадратов
        double sSqr = 0.0;                              //! Расчет
        for (int j = m_begin; j <= m_end; ++j)
        {
            double r = signal->value(j);
            sSqr += pow(r - signal->midValue(), 2);
        }

        //! Расчет 31 коэффициента
        m_AKSh0 = -1;
        //! Расчет суммы произведений
        for (int i = 0; i < PulseFactorsDefines::iAKPointsCount; ++i)
        {
            double s = 0.0;
            int k = 0;
            for (int j = i; j <= m_end; ++j)
            {
                s += ((signal->value(j) - signal->midValue()) * (signal->value(k) - signal->midValue()));
                ++k;
            }

           //! Коэффициент корелляции
           if (sSqr > 0)
               m_masKK[i] = s / sSqr;
           else
               m_masKK[i] = 0;

           //! Первый переход линии 0
           if ((static_cast<int>(m_AKSh0) == -1) and (m_masKK[i] <= 0))
              m_AKSh0 = i;
        }

        m_ACK1 = m_masKK[1]; //! Первый коэффициент корелляции

        //! Если функция корелляции не пересекла 0 за iAKPointsCount точек корелляции, то
        //! первая точка должна быть за пределами исследуемой зоны корелляции iAKPointsCount + 1
        if (static_cast<int>(m_AKSh0) == -1)
            m_AKSh0 = PulseFactorsDefines::iAKPointsCount + 1;
    }
}
