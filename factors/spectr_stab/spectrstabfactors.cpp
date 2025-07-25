#include "spectrstabfactors.h"

#include <QDir>
#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "channelsutils.h"
#include "signalaccess.h"
#include "stabilogram.h"
#include "computefft.h"
#include "signalfft.h"
#include "baseutils.h"


SpectrStabFactors::SpectrStabFactors(const QString &testUid,
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

QString SpectrStabFactors::uid() const
{
    return SpectrStabFactorsDefines::GroupUid;
}

QString SpectrStabFactors::name() const
{
    return nameAsConst();
}

QString SpectrStabFactors::nameAsConst()
{
    return QCoreApplication::tr("Показатели спектра стабилограммы");
}

bool SpectrStabFactors::isValid() const
{
    return isValid(testUid(), probeUid(), channelId());
}

bool SpectrStabFactors::isValid(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(testUid);
    return DataProvider::channelExists(probeUid, channelId) &&
           ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctStabilogram;
}

void SpectrStabFactors::calculate()
{
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), channelId(), baStab))
    {
        Stabilogram stab(baStab);
        m_frequency = stab.frequency();
        m_spectr = new SignalFFT(&stab, ComputeFFT::FFT_COUNT, m_frequency);
        m_freqMax = static_cast<int>((1 / (static_cast<double>(stab.size()) / static_cast<double>(stab.frequency()))) *
                                     (static_cast<double>(ComputeFFT::FFT_COUNT) / 2));
        m_duration = static_cast<double>(stab.size()) / stab.frequency();

        computeFactors();
    }

    addFactors();
}

void SpectrStabFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(SpectrStabFactorsDefines::GroupUid, nameAsConst());

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Frequency1Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Частота первого пика (фронталь)"), QCoreApplication::tr("F1X"), QCoreApplication::tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Amplitude1Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда первого пика (фронталь)"), QCoreApplication::tr("A1X"), QCoreApplication::tr("мм"), 4, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Frequency2Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Частота второго пика (фронталь)"), QCoreApplication::tr("F2X"), QCoreApplication::tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Amplitude2Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда второго пика (фронталь)"), QCoreApplication::tr("A2X"), QCoreApplication::tr("мм"), 4, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Frequency3Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Частота третьего пика (фронталь)"), QCoreApplication::tr("F3X"), QCoreApplication::tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Amplitude3Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда третьего пика (фронталь)"), QCoreApplication::tr("A3X"), QCoreApplication::tr("мм"), 4, 1, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Level60Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("60% мощности спектра (фронталь)"), QCoreApplication::tr("60%X"), QCoreApplication::tr("мм"), 2, 1, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Power1Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Мощность первой зоны (фронталь)"), QCoreApplication::tr("Pwr1X"), QCoreApplication::tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Power2Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Мощность второй зоны (фронталь)"), QCoreApplication::tr("Pwr2X"), QCoreApplication::tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Power3Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Мощность третьей зоны (фронталь)"), QCoreApplication::tr("Pwr3X"), QCoreApplication::tr("%"), 0, 1, FactorsDefines::nsDual, 12);


    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Frequency1Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Частота первого пика (сагитталь)"), QCoreApplication::tr("F1Y"), QCoreApplication::tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Amplitude1Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда первого пика (сагитталь)"), QCoreApplication::tr("A1Y"), QCoreApplication::tr("мм"), 4, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Frequency2Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Частота второго пика (сагитталь)"), QCoreApplication::tr("F2Y"), QCoreApplication::tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Amplitude2Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда второго пика (сагитталь)"), QCoreApplication::tr("A2Y"), QCoreApplication::tr("мм"), 4, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Frequency3Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Частота третьего пика (сагитталь)"), QCoreApplication::tr("F3Y"), QCoreApplication::tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Amplitude3Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Амплитуда третьего пика (сагитталь)"), QCoreApplication::tr("A3Y"), QCoreApplication::tr("мм"), 4, 1, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Level60Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("60% мощности спектра (сагитталь)"), QCoreApplication::tr("60%Y"), QCoreApplication::tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Power1Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Мощность первой зоны (сагитталь)"), QCoreApplication::tr("Pwr1Y"), QCoreApplication::tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Power2Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Мощность второй зоны (сагитталь)"), QCoreApplication::tr("Pwr2Y"), QCoreApplication::tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Power3Uid, SpectrStabFactorsDefines::GroupUid,
                           QCoreApplication::tr("Мощность третьей зоны (сагитталь)"), QCoreApplication::tr("Pwr3Y"), QCoreApplication::tr("%"), 0, 1, FactorsDefines::nsDual, 12);
}

int SpectrStabFactors::channelsCount() const
{
    if (m_spectr)
        return m_spectr->channelsCount();
    return 0;
}

int SpectrStabFactors::points() const
{
    if (m_spectr)
        return m_spectr->points();
    return 0;
}

double SpectrStabFactors::value(const int spectr, const int point) const
{
    if (m_spectr)
        return m_spectr->value(spectr, point);
    return 0;
}

void SpectrStabFactors::computeFactors()
{
    if (m_spectr->channelsCount() == 2)
    {
        computeFactorsChan(0, m_valuesX);
        computeFactorsChan(1, m_valuesY);
    }
}

void SpectrStabFactors::computeFactorsChan(const int chan, SpectrStabFactors::FactorValues &values)
{
    //! Создадим буфер, чтоб проще было
    QVector<double> data;
    for (int i = 0; i < m_spectr->points(); ++i)
    {
        data << m_spectr->value(chan, i);
    }

    //! Для запоминания экстремумов используем карту, чтоб в один проход
    QMap<double, int> maxs;

    values.power1 = 0;
    values.power2 = 0;
    values.power3 = 0;

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

        //!Мощность по зонам
        double f = static_cast<double>(i * m_freqMax) / static_cast<double>(m_spectr->points());

        //! Общая мощность
        if (f <= 6)
            summ += data.at(i);

        if (f <= 0.2)
            values.power1 += data.at(i);
        else
        if (f > 0.2 && f <= 2)
            values.power2 += data.at(i);
        else
            values.power3 += data.at(i);
    }

    //! Мощность по зонам в процентах
    values.power1 = round(values.power1 / summ * 100);
    values.power2 = round(values.power2 / summ * 100);
    values.power3 = round(100 - values.power1 - values.power2);

    //! Поиск уровня 60% мощности
    double s = 0;
    for (int i = 0; i < data.size(); ++i)
    {
        double f = static_cast<double>(i * m_freqMax) / static_cast<double>(m_spectr->points());

        //! Общая мощность
        if (f <= 6)
        {
            s += data.at(i);
            if (s > summ * 0.6)
            {
                values.pwr60 = static_cast<double>(i * m_freqMax) / static_cast<double>(m_spectr->points());
                break;
            }
        }
    }

    //! Амплитуды и частоты первых трех пиков
    if (maxs.size() > 3)
    {
        int n = maxs.keys().size();
        values.freq1 = static_cast<double>(maxs.value(maxs.keys().at(n - 1)) * m_freqMax) / static_cast<double>(m_spectr->points());
        values.freq2 = static_cast<double>(maxs.value(maxs.keys().at(n - 2)) * m_freqMax) / static_cast<double>(m_spectr->points());
        values.freq3 = static_cast<double>(maxs.value(maxs.keys().at(n - 3)) * m_freqMax) / static_cast<double>(m_spectr->points());
        values.ampl1 = maxs.keys().at(n - 1);
        values.ampl2 = maxs.keys().at(n - 2);
        values.ampl3 = maxs.keys().at(n - 3);
    }
    else
    {
        values.freq1 = 0;
        values.freq2 = 0;
        values.freq3 = 0;
        values.ampl1 = 0;
        values.ampl2 = 0;
        values.ampl3 = 0;
    }
}

void SpectrStabFactors::addFactors()
{
    addFactor(SpectrStabFactorsDefines::Frontal::Frequency1Uid, m_valuesX.freq1);
    addFactor(SpectrStabFactorsDefines::Frontal::Amplitude1Uid, m_valuesX.ampl1);
    addFactor(SpectrStabFactorsDefines::Frontal::Frequency2Uid, m_valuesX.freq2);
    addFactor(SpectrStabFactorsDefines::Frontal::Amplitude2Uid, m_valuesX.ampl2);
    addFactor(SpectrStabFactorsDefines::Frontal::Frequency3Uid, m_valuesX.freq3);
    addFactor(SpectrStabFactorsDefines::Frontal::Amplitude3Uid, m_valuesX.ampl3);

    addFactor(SpectrStabFactorsDefines::Frontal::Level60Uid, m_valuesX.pwr60);

    addFactor(SpectrStabFactorsDefines::Frontal::Power1Uid, m_valuesX.power1);
    addFactor(SpectrStabFactorsDefines::Frontal::Power2Uid, m_valuesX.power2);
    addFactor(SpectrStabFactorsDefines::Frontal::Power3Uid, m_valuesX.power3);


    addFactor(SpectrStabFactorsDefines::Sagittal::Frequency1Uid, m_valuesY.freq1);
    addFactor(SpectrStabFactorsDefines::Sagittal::Amplitude1Uid, m_valuesY.ampl1);
    addFactor(SpectrStabFactorsDefines::Sagittal::Frequency2Uid, m_valuesY.freq2);
    addFactor(SpectrStabFactorsDefines::Sagittal::Amplitude2Uid, m_valuesY.ampl2);
    addFactor(SpectrStabFactorsDefines::Sagittal::Frequency3Uid, m_valuesY.freq3);
    addFactor(SpectrStabFactorsDefines::Sagittal::Amplitude3Uid, m_valuesY.ampl3);

    addFactor(SpectrStabFactorsDefines::Sagittal::Level60Uid, m_valuesY.pwr60);

    addFactor(SpectrStabFactorsDefines::Sagittal::Power1Uid, m_valuesY.power1);
    addFactor(SpectrStabFactorsDefines::Sagittal::Power2Uid, m_valuesY.power2);
    addFactor(SpectrStabFactorsDefines::Sagittal::Power3Uid, m_valuesY.power3);
}
