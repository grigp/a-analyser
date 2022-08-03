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
                                     QObject *parent)
    : ChannelMultifactor(testUid, probeUid, channelId, parent)
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
    return SpectrStabFactorsDefines::GroupName;
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
    }

    computeFactors();
    addFactors();
}

void SpectrStabFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(SpectrStabFactorsDefines::GroupUid, SpectrStabFactorsDefines::GroupName);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Frequency1Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Частота первого пика по фронтали"), tr("F1X"), tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Amplitude1Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Амплитуда первого пика по фронтали"), tr("A1X"), tr("мм"), 4, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Frequency2Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Частота второго пика по фронтали"), tr("F2X"), tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Amplitude2Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Амплитуда второго пика по фронтали"), tr("A2X"), tr("мм"), 4, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Frequency3Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Частота третьего пика по фронтали"), tr("F3X"), tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Amplitude3Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Амплитуда третьего пика по фронтали"), tr("A3X"), tr("мм"), 4, 1, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Level60Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("60% мощности спектра по фронтали"), tr("60%X"), tr("мм"), 2, 1, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Power1Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Мощность первой зоны по фронтали"), tr("Pwr1X"), tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Power2Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Мощность второй зоны по фронтали"), tr("Pwr2X"), tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Frontal::Power3Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Мощность третьей зоны по фронтали"), tr("Pwr3X"), tr("%"), 0, 1, FactorsDefines::nsDual, 12);


    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Frequency1Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Частота первого пика по сагиттали"), tr("F1Y"), tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Amplitude1Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Амплитуда первого пика по сагиттали"), tr("A1Y"), tr("мм"), 4, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Frequency2Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Частота второго пика по сагиттали"), tr("F2Y"), tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Amplitude2Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Амплитуда второго пика по сагиттали"), tr("A2Y"), tr("мм"), 4, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Frequency3Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Частота третьего пика по сагиттали"), tr("F3Y"), tr("Гц"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Amplitude3Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Амплитуда третьего пика по сагиттали"), tr("A3Y"), tr("мм"), 4, 1, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Level60Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("60% мощности спектра по сагиттали"), tr("60%Y"), tr("мм"), 2, 1, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Power1Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Мощность первой зоны по сагиттали"), tr("Pwr1Y"), tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Power2Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Мощность второй зоны по сагиттали"), tr("Pwr2Y"), tr("%"), 0, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(SpectrStabFactorsDefines::Sagittal::Power3Uid, SpectrStabFactorsDefines::GroupUid,
                           tr("Мощность третьей зоны по сагиттали"), tr("Pwr3Y"), tr("%"), 0, 1, FactorsDefines::nsDual, 12);
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
    QString path = DataDefines::aanalyserDocumentsPath() + "Export/";
    QDir dir(path);
    if (!dir.exists())
        dir.mkpath(path);

    for (int i = 0; i < m_spectr->channelsCount(); ++i)
    {
        QVector<double> data;
        for (int j = 0; j < m_spectr->points(); ++j)
            data << m_spectr->value(i, j);

//        BaseUtils::vectorToText(data, path + "fft_" + QString::number(i) + ".txt");
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
