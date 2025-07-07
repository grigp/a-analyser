#include "spectrsinglesignalfactors.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "channelsutils.h"
#include "balistogram.h"
#include "dynamosignal.h"
#include "breathsignal.h"
#include "computefft.h"
#include "signalfft.h"

#include <QDebug>

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
    return nameAsConst();
}

QString SpectrSingleSignalFactors::nameAsConst()
{
    return QCoreApplication::tr("Показатели спектра одиночного сигнала");
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

//        qDebug() << "----------------------------------------------------------------------------";
//        for (int i = 0; i < signal->size(); ++i) {
//            qDebug() << signal->value(0, i);
//        }
//        qDebug() << "----------------------------------------------------------------------------";

        m_frequency = signal->frequency();
        m_spectr = new SignalFFT(signal, ComputeFFT::FFT_COUNT, m_frequency);
        m_freqMax = static_cast<int>((1 / (static_cast<double>(signal->size()) / static_cast<double>(signal->frequency()))) *
                                     (static_cast<double>(ComputeFFT::FFT_COUNT) / 2));

//        qDebug() << "----------------------------------------------------------------------------";
//        for (int i = 0; i < m_spectr->points(); ++i) {
//            qDebug() << m_spectr->value(0, i);
//        }
//        qDebug() << "----------------------------------------------------------------------------";

        computeFactors();

        delete signal;
    }

    addFactors();
}

void SpectrSingleSignalFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(SpectrSingleSignalFactorsDefines::GroupUid, nameAsConst());

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Frequency1Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           QCoreApplication::tr("Частота первого пика"), QCoreApplication::tr("F1"), QCoreApplication::tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Amplitude1Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда первого пика"), QCoreApplication::tr("A1"), QCoreApplication::tr("мм"), 4, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Frequency2Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           QCoreApplication::tr("Частота второго пика"), QCoreApplication::tr("F2"), QCoreApplication::tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Amplitude2Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда второго пика"), QCoreApplication::tr("A2"), QCoreApplication::tr("мм"), 4, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Frequency3Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           QCoreApplication::tr("Частота третьего пика"), QCoreApplication::tr("F3"), QCoreApplication::tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Amplitude3Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда третьего пика"), QCoreApplication::tr("A3"), QCoreApplication::tr("мм"), 4, 1, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Level60Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           QCoreApplication::tr("60% мощности спектра"), QCoreApplication::tr("60%"), QCoreApplication::tr("мм"), 2, 1, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Power1Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           QCoreApplication::tr("Мощность первой зоны"), QCoreApplication::tr("Pwr1"), QCoreApplication::tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Power2Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           QCoreApplication::tr("Мощность второй зоны"), QCoreApplication::tr("Pwr2"), QCoreApplication::tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrSingleSignalFactorsDefines::Power3Uid, SpectrSingleSignalFactorsDefines::GroupUid,
                           QCoreApplication::tr("Мощность третьей зоны"), QCoreApplication::tr("Pwr3"), QCoreApplication::tr("%"), 0, 1, FactorsDefines::nsDual, 12);
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
//    qDebug() << m_freqMax << m_spectr->points();
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
        double f = static_cast<double>(i * m_freqMax) / static_cast<double>(m_spectr->points());
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
//        qDebug() << i << "\t"
//                 << static_cast<double>(i * m_freqMax) / static_cast<double>(m_spectr->points())
//                 << "  \t  "
//                 << data.at(i) << "  \t   " << s << "\t" << summ << "\t" << s / summ * 100;
        if (s > summ * 0.6)
//        if (s >= summ)
        {
            m_values.pwr60 = static_cast<double>(i * m_freqMax) / static_cast<double>(m_spectr->points());
            break;
        }
    }
//    qDebug() << "---------------------------------------------------------------------------";

    //! Амплитуды и частоты первых трех пиков
    if (maxs.size() > 3)
    {
        int n = maxs.keys().size();
        m_values.freq1 = static_cast<double>(maxs.value(maxs.keys().at(n - 1)) * m_freqMax) / static_cast<double>(m_spectr->points());
        m_values.freq2 = static_cast<double>(maxs.value(maxs.keys().at(n - 2)) * m_freqMax) / static_cast<double>(m_spectr->points());
        m_values.freq3 = static_cast<double>(maxs.value(maxs.keys().at(n - 3)) * m_freqMax) / static_cast<double>(m_spectr->points());
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
