#include "pulsespectrfactors.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsutils.h"
#include "dataprovider.h"
#include "ritmogram.h"
#include "computefft.h"
#include "pulsespectrfactorsdefines.h"

namespace  {
static const int FFT_COUNT = 256;

}

PulseSpectrFactors::PulseSpectrFactors(const QString &testUid,
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

QString PulseSpectrFactors::uid() const
{
    return PulseSpectrFactorsDefines::GroupUid;
}

QString PulseSpectrFactors::name() const
{
    return PulseSpectrFactorsDefines::GroupName;
}

bool PulseSpectrFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool PulseSpectrFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctRitmogram;
}

void PulseSpectrFactors::calculate()
{
    QByteArray baPulse;
    if (DataProvider::getChannel(probeUid(), channelId(), baPulse))
    {
        Ritmogram signal(baPulse);

        if (signal.size() > 255)
        {
            //! Расчет среднего значения RR - интервала
            double len = 0;
            for (int i = 0; i < signal.size(); ++i)
                len += 60.0 / signal.value(i);
            double midT = len / signal.size();

            //! Создание буфера
            QVector<double> data;
            double iv = 0;

            double curT = 0;
            for (int i = 0; i < signal.size(); ++i)
            {
                double i1 = curT;
                double i2 = curT + 60.0 / signal.value(i);
                curT += 60.0 / signal.value(i);
                do
                {
                    if (iv < curT)
                    {
                        if (i < signal.size() - 1)
                        {
                            double v1 = signal.value(i);
                            double v2 = signal.value(i + 1);
                            double v = v1;
                            if (i2 - i1 > 0)
                            {
                                if (v1 > v2)
                                    v = v1 - (v1 - v2) * (iv - i1) / (i2 - i1);
                                else
                                    v = v1 + (v2 - v1) * (iv - i1) / (i2 - i1);
                            }
                            data << 60 / v * 1000;  //! Спектр по амплитуде будет в миллисекундах, а не в уд / мин
                            iv += midT;
                        }
                        else
                            break;
                    }
                }
                while (iv <= curT);
            }
            m_freqRate = midT;

            //! Расчет спектра и показателей
            if (ComputeFFT::extendFFT(data, m_spectr, FFT_COUNT, 10))
            {
                int nHF = 0;
                int nLF = 0;
                int nVLF = 0;
                int nULF = 0;

                for (int i = 0; i < m_spectr.size(); ++i)
                {
                    double fi = i * m_freqRate / (FFT_COUNT / 2);

                    if (fi >= PulseSpectrFactorsDefines::SpectrAreases.at(PulseSpectrFactorsDefines::ULF).lo &&
                        fi <= PulseSpectrFactorsDefines::SpectrAreases.at(PulseSpectrFactorsDefines::ULF).hi)
                    {
                        m_PwULF += m_spectr.at(i);
                        ++nULF;
                    }
                    else
                    if (fi >= PulseSpectrFactorsDefines::SpectrAreases.at(PulseSpectrFactorsDefines::VLF).lo &&
                        fi <= PulseSpectrFactorsDefines::SpectrAreases.at(PulseSpectrFactorsDefines::VLF).hi)
                    {
                        m_PwVLF += m_spectr.at(i);
                        ++nVLF;
                    }
                    else
                    if (fi >= PulseSpectrFactorsDefines::SpectrAreases.at(PulseSpectrFactorsDefines::LF).lo &&
                        fi <= PulseSpectrFactorsDefines::SpectrAreases.at(PulseSpectrFactorsDefines::LF).hi)
                    {
                        m_PwLF += m_spectr.at(i);
                        ++nLF;
                    }
                    else
                    if (fi >= PulseSpectrFactorsDefines::SpectrAreases.at(PulseSpectrFactorsDefines::HF).lo &&
                        fi <= PulseSpectrFactorsDefines::SpectrAreases.at(PulseSpectrFactorsDefines::HF).hi)
                    {
                        m_PwHF += m_spectr.at(i);
                        ++nHF;
                    }
                    m_PwSum += m_spectr.at(i);
                }

                if (m_PwSum > 0)
                {
                    m_PrHF = m_PwHF / m_PwSum * 100;
                    m_PrLF = m_PwLF / m_PwSum * 100;
                    m_PrVLF = m_PwVLF / m_PwSum * 100;
                    m_PrULF = m_PwULF / m_PwSum * 100;
                }
                if (nHF > 0)
                    m_PwAvHF = m_PwHF / nHF;
                if (nLF > 0)
                    m_PwAvLF = m_PwLF / nLF;
                if (nVLF > 0)
                    m_PwAvVLF = m_PwVLF / nVLF;
                if (nULF > 0)
                    m_PwAvULF = m_PwULF / nULF;
                if (m_PwHF > 0)
                    m_LH = m_PwLF / m_PwHF;

                //! 60 % мощности спектра
                double s60 = 0;
                for (int i = 0; i < m_spectr.size(); ++i)
                {
                    s60 += m_spectr.at(i);
                    if (s60 >= m_PwSum * 0.6)
                    {
                        m_Pw60 = i * m_freqRate / (FFT_COUNT / 2);
                        break;
                    }
                }

                if (m_PrHF > 0)
                    m_CentIdx = (m_PrLF + m_PrVLF) / m_PrHF;
            }
        }
    }


    addFactor(PulseSpectrFactorsDefines::PwHFUid, m_PwHF);
    addFactor(PulseSpectrFactorsDefines::PwLFUid, m_PwLF );
    addFactor(PulseSpectrFactorsDefines::PwVLFUid, m_PwVLF);
    addFactor(PulseSpectrFactorsDefines::PwULFUid, m_PwULF);
    addFactor(PulseSpectrFactorsDefines::PwSumUid, m_PwSum);
    addFactor(PulseSpectrFactorsDefines::PrHFUid, m_PrHF);
    addFactor(PulseSpectrFactorsDefines::PrLFUid, m_PrLF);
    addFactor(PulseSpectrFactorsDefines::PrVLFUid, m_PrVLF);
    addFactor(PulseSpectrFactorsDefines::PrULFUid, m_PrULF);
    addFactor(PulseSpectrFactorsDefines::PwAvHFUid, m_PwAvHF);
    addFactor(PulseSpectrFactorsDefines::PwAvLFUid, m_PwAvLF);
    addFactor(PulseSpectrFactorsDefines::PwAvVLFUid, m_PwAvVLF);
    addFactor(PulseSpectrFactorsDefines::PwAvULFUid, m_PwAvULF);
    addFactor(PulseSpectrFactorsDefines::LHUid, m_LH);
    addFactor(PulseSpectrFactorsDefines::Pw60Uid, m_Pw60);
    addFactor(PulseSpectrFactorsDefines::CentIdxUid, m_CentIdx);

//    addFactor(PulseSpectrFactorsDefines::FqMidSpUid, m_FqMidSp);
//    addFactor(PulseSpectrFactorsDefines::LFnuUid, m_LFnu);
//    addFactor(PulseSpectrFactorsDefines::HFnuUid, m_HFnu);
}

void PulseSpectrFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(PulseSpectrFactorsDefines::GroupUid, PulseSpectrFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::PwHFUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Мощность зоны высокой частоты"), tr("HF"), tr("мс2"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::PwLFUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Мощность зоны низкой частоты"), tr("LF"), tr("мс2"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::PwVLFUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Мощность зоны очень низкой частоты"), tr("VLF"), tr("мс2"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::PwULFUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Мощность зоны ультранизкой частоты"), tr("ULF"), tr("мс2"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::PwSumUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Суммарная мощность спектра"), tr("TP"), tr("мс2"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::PrHFUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Процент мощности зоны высокой частоты"), tr("%HF"), tr("%"), 1, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::PrLFUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Процент мощности зоны низкой частоты"), tr("%LF"), tr("%"), 1, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::PrVLFUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Процент мощности зоны очень низкой частоты"), tr("%VLF"), tr("%"), 1, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::PrULFUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Процент мощности зоны ультранизкой частоты"), tr("%ULF"), tr("%"), 1, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::PwAvHFUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Средняя мощность зоны высокой частоты"), tr("AvHF"), tr("мс2"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::PwAvLFUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Средняя мощность зоны низкой частоты"), tr("AvLF"), tr("мс2"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::PwAvVLFUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Средняя мощность зоны очень низкой частоты"), tr("AvVLF"), tr("мс2"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::PwAvULFUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Средняя мощность зоны ультранизкой частоты"), tr("AvULF"), tr("мс2"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::LHUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Отношение мощности низкой частоты к высокой"), tr("LF/HF"), tr(""), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::Pw60Uid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Уровень 60% мощности спектра"), tr("60%"), tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::CentIdxUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Индекс централизации"), tr("IC"), tr(""), 2, 1, FactorsDefines::nsDual, 12);

//    static_cast<AAnalyserApplication*>(QApplication::instance())->
//            registerFactor(PulseSpectrFactorsDefines::FqMidSpUid, PulseSpectrFactorsDefines::GroupUid,
//                           tr("Средняя частота спектра"), tr("Fm"), tr("Гц"), 3, 1, FactorsDefines::nsDual, 12);
//    static_cast<AAnalyserApplication*>(QApplication::instance())->
//            registerFactor(PulseSpectrFactorsDefines::LFnuUid, PulseSpectrFactorsDefines::GroupUid,
//                           tr("Доля мощности низких частот в нормализованных единицах"), tr("LFn"), tr("%"), 2, 1, FactorsDefines::nsDual, 12);
//    static_cast<AAnalyserApplication*>(QApplication::instance())->
//            registerFactor(PulseSpectrFactorsDefines::HFnuUid, PulseSpectrFactorsDefines::GroupUid,
//                           tr("Доля мощности высоких частот в нормализованных единицах"), tr("HFn"), tr("%"), 2, 1, FactorsDefines::nsDual, 12);
}

int PulseSpectrFactors::spectrCount() const
{
    return FFT_COUNT / 2;
}

double PulseSpectrFactors::spectrValue(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < FFT_COUNT / 2);
    return m_spectr.at(idx);
}
