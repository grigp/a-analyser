#include "spectrsinglesignalfactors.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "channelsutils.h"
#include "balistogram.h"
#include "dynamosignal.h"
#include "breathsignal.h"
#include "computefft.h"
#include "signalfft.h"

SpectrSingleSignalFactors::SpectrSingleSignalFactors(const QString &testUid,
                                                     const QString &probeUid,
                                                     const QString &channelId,
                                                     const int begin,
                                                     const int end,
                                                     QObject *parent)
    : ChannelMultifactor (testUid, probeUid, channelId, begin, end, parent)
{
    if (isValid())
        calculate();
}

QString SpectrSingleSignalFactors::uid() const
{
    return SpectrSingleSignalFactorsDefines::GroupUid;
}

QString SpectrSingleSignalFactors::name() const
{
    return SpectrSingleSignalFactorsDefines::GroupName;
}

bool SpectrSingleSignalFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool SpectrSingleSignalFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           (ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctBalistogram ||
            ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctDynamo ||
            ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctBreath);
}

void SpectrSingleSignalFactors::calculate()
{
    QByteArray baSignal;
    if (DataProvider::getChannel(probeUid(), channelId(), baSignal))
    {
        SingleSignal* signal {nullptr};

        if (ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctBalistogram)
            signal = new Balistogram(baSignal);
        else
        if (ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctDynamo)
            signal = new DynamoSignal(baSignal);
        else
        if (ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctBreath)
            signal = new BreathSignal(baSignal);

        m_frequency = signal->frequency();
        m_spectr = new SignalFFT(signal, ComputeFFT::FFT_COUNT, m_frequency);

        computeFactors();

        delete signal;
    }

    addFactors();
}

void SpectrSingleSignalFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(SpectrSingleSignalFactorsDefines::GroupUid, SpectrSingleSignalFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Frequency1Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           tr("Частота первого пика"), tr("F1"), tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Amplitude1Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           tr("Амплитуда первого пика"), tr("A1"), tr("мм"), 4, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Frequency2Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           tr("Частота второго пика"), tr("F2"), tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Amplitude2Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           tr("Амплитуда второго пика"), tr("A2"), tr("мм"), 4, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Frequency3Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           tr("Частота третьего пика"), tr("F3"), tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Amplitude3Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           tr("Амплитуда третьего пика"), tr("A3"), tr("мм"), 4, 1, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Level60Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           tr("60% мощности спектра"), tr("60%"), tr("мм"), 2, 1, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Power1Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           tr("Мощность первой зоны"), tr("Pwr1"), tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Power2Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           tr("Мощность второй зоны"), tr("Pwr2"), tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Power3Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           tr("Мощность третьей зоны"), tr("Pwr3"), tr("%"), 0, 1, FactorsDefines::nsDual, 12);
}

void SpectrSingleSignalFactors::computeFactors()
{
    //! Создадим буфер, чтоб проще было
    QVector<double> data;
    for (int i = 0; i < m_spectr->points(); ++i)
        data << m_spectr->value(0, i);

    //! Для запоминания экстремумов используем карту, чтоб в один проход
    QMap<double, int> maxs;

    m_values.power1 = 0;
    m_values.power2 = 0;
    m_values.power3 = 0;

    //! По спектру
    double summ = 0;
    for (int i = 0; i < data.size(); ++i)
    {
        //! Добавление экстремумов
        if (i == 0 && data.at(i) > data.at(i+1))
            maxs.insert(data.at(i), i);
        else
        if (i == data.size() - 1 && data.at(i) > data.at(i-1))
            maxs.insert(data.at(i), i);
        else
        if (i > 0 && i < data.size() - 1 && data.at(i) > data.at(i-1) && data.at(i) > data.at(i+1))
            maxs.insert(data.at(i), i);

        //! Общая мощность
        summ += data.at(i);

        //!Мощность по зонам
        double f = static_cast<double>(i * m_frequency) / static_cast<double>(m_spectr->points());
        if (f <= 0.2)
            m_values.power1 += data.at(i);
        else
        if (f > 0.2 && f <= 2)
            m_values.power2 += data.at(i);
        else
            m_values.power3 += data.at(i);
    }

    //! Мощность по зонам в процентах
    m_values.power1 = round(m_values.power1 / summ * 100);
    m_values.power2 = round(m_values.power2 / summ * 100);
    m_values.power3 = round(100 - m_values.power1 - m_values.power2);

    //! Поиск уровня 60% мощности
    double s = 0;
    for (int i = 0; i < data.size(); ++i)
    {
        s += data.at(i);
        if (s > summ * 0.6)
        {
            m_values.pwr60 = static_cast<double>(i * m_frequency) / static_cast<double>(m_spectr->points());
            break;
        }
    }

    //! Амплитуды и частоты первых трех пиков
    if (maxs.size() > 3)
    {
        int n = maxs.keys().size();
        m_values.freq1 = static_cast<double>(maxs.value(maxs.keys().at(n - 1)) * m_frequency) / static_cast<double>(m_spectr->points());
        m_values.freq2 = static_cast<double>(maxs.value(maxs.keys().at(n - 2)) * m_frequency) / static_cast<double>(m_spectr->points());
        m_values.freq3 = static_cast<double>(maxs.value(maxs.keys().at(n - 3)) * m_frequency) / static_cast<double>(m_spectr->points());
        m_values.ampl1 = maxs.keys().at(n - 1);
        m_values.ampl2 = maxs.keys().at(n - 2);
        m_values.ampl3 = maxs.keys().at(n - 3);
    }
    else
    {
        m_values.freq1 = 0;
        m_values.freq2 = 0;
        m_values.freq3 = 0;
        m_values.ampl1 = 0;
        m_values.ampl2 = 0;
        m_values.ampl3 = 0;
    }
}

void SpectrSingleSignalFactors::addFactors()
{
    addFactor(SpectrSingleSignalFactorsDefines::Frequency1Uid, m_values.freq1);
    addFactor(SpectrSingleSignalFactorsDefines::Amplitude1Uid, m_values.ampl1);
    addFactor(SpectrSingleSignalFactorsDefines::Frequency2Uid, m_values.freq2);
    addFactor(SpectrSingleSignalFactorsDefines::Amplitude2Uid, m_values.ampl2);
    addFactor(SpectrSingleSignalFactorsDefines::Frequency3Uid, m_values.freq3);
    addFactor(SpectrSingleSignalFactorsDefines::Amplitude3Uid, m_values.ampl3);

    addFactor(SpectrSingleSignalFactorsDefines::Level60Uid, m_values.pwr60);

    addFactor(SpectrSingleSignalFactorsDefines::Power1Uid, m_values.power1);
    addFactor(SpectrSingleSignalFactorsDefines::Power2Uid, m_values.power2);
    addFactor(SpectrSingleSignalFactorsDefines::Power3Uid, m_values.power3);
}