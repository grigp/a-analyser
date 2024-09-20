#include "babyposturecalculator.h"

#include "dataprovider.h"
#include "channelsdefines.h"
#include "classicfactors.h"
#include "spectrstabfactors.h"
#include "spectrsinglesignalfactors.h"
#include "vectorfactors.h"

#include <QDebug>

BabyPostureCalculator::BabyPostureCalculator(const QString &testUid, QObject *parent)
    : StabTestCalculator(testUid, parent)
{

}

BabyPostureCalculator::~BabyPostureCalculator()
{
    if (m_fctClassic)
        delete m_fctClassic;
    if (m_fctSpectrStab)
        delete m_fctSpectrStab;
    if (m_fctSpectrZ)
        delete m_fctSpectrZ;
    if (m_fctVector)
        delete m_fctVector;
}

void BabyPostureCalculator::calculate()
{
    StabTestCalculator::calculate();

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid(), ti))
    {
        if (ti.probes.size() > 0)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(0), pi))
            {
                m_factors = createFactors();
                assignFactors(ChannelsDefines::chanStab, ChannelsDefines::chanZ, pi, m_factors, true);

                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanStab1) &
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanZ1) &
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanStab2) &
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanZ2))
                {

                    m_factors1 = createFactors();
                    assignFactors(ChannelsDefines::chanStab1, ChannelsDefines::chanZ1, pi, m_factors1, false);
                    m_factors2 = createFactors();
                    assignFactors(ChannelsDefines::chanStab2, ChannelsDefines::chanZ2, pi, m_factors2, false);
                }
            }
        }
    }
}

void BabyPostureCalculator::fastCalculate()
{
    StabTestCalculator::fastCalculate();
}

int BabyPostureCalculator::factorsCount() const
{
    return m_factors.size();
}

BabyPostureCalculator::FactorInfo BabyPostureCalculator::factor(const int idx) const
{
    Q_ASSERT((idx >= 0) && (idx < m_factors.size()));
    return m_factors.at(idx);
}

int BabyPostureCalculator::factors1Count() const
{
    return m_factors1.size();
}

BabyPostureCalculator::FactorInfo BabyPostureCalculator::factor1(const int idx) const
{
    Q_ASSERT((idx >= 0) && (idx < m_factors1.size()));
    return m_factors1.at(idx);
}

int BabyPostureCalculator::factors2Count() const
{
    return m_factors2.size();
}

BabyPostureCalculator::FactorInfo BabyPostureCalculator::factor2(const int idx) const
{
    Q_ASSERT((idx >= 0) && (idx < m_factors2.size()));
    return m_factors2.at(idx);
}

QList<BabyPostureCalculator::FactorInfo> BabyPostureCalculator::createFactors()
{
    return QList<BabyPostureCalculator::FactorInfo>()
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     VectorFactorsDefines::VMidUid, 3,
                                                     QCoreApplication::tr("Скорость ОЦД"),
                                                     QCoreApplication::tr("V (мм/с)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     ClassicFactorsDefines::SquareUid, 0,
                                                     QCoreApplication::tr("Площадь статокинезиограммы 95"),
                                                     QCoreApplication::tr("s95 (мм2)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     ClassicFactorsDefines::LFSUid, 0,
                                                     QCoreApplication::tr("Отношение длины статокинезиограммы к её площади"),
                                                     QCoreApplication::tr("LFS95 (1/мм)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     ClassicFactorsDefines::EllLengthUid, 0,
                                                     QCoreApplication::tr("Длина эллипса статокинезиограммы"),
                                                     QCoreApplication::tr("Le95 (мм)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     ClassicFactorsDefines::EllWidthUid, 0,
                                                     QCoreApplication::tr("Ширина эллипса статокинезиограммы"),
                                                     QCoreApplication::tr("We95 (мм)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     ClassicFactorsDefines::ComprRatioUid, 0,
                                                     QCoreApplication::tr("Отношение длины эллипса к его ширине"),
                                                     QCoreApplication::tr("Le-We (ед)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Frontal::Level60Uid, 1,
                                                     QCoreApplication::tr("Уровень 60% мощности спектра во фронтальной плоскости"),
                                                     QCoreApplication::tr("xf60% (Гц)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Sagittal::Level60Uid, 1,
                                                     QCoreApplication::tr("Уровень 60% мощности спектра во сагиттальной плоскости"),
                                                     QCoreApplication::tr("yf60% (Гц)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrSingleSignalFactorsDefines::Level60Uid, 2,
                                                     QCoreApplication::tr("Уровень 60% мощности спектра во вертикальной составляющей"),
                                                     QCoreApplication::tr("zf60% (Гц)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     VectorFactorsDefines::KFRUid, 3,
                                                     QCoreApplication::tr("Качество функции равновесия"),
                                                     QCoreApplication::tr("КФР (%)"),
                                                     ""))

            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Frontal::Frequency1Uid, 1,
                                                     QCoreApplication::tr("Частота 1-го максимума спектра по фронтальной составляющей"),
                                                     QCoreApplication::tr("Xf1 (Гц)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Frontal::Amplitude1Uid, 1,
                                                     QCoreApplication::tr("Амплитуда 1-го максимума спектра по фронтальной составляющей"),
                                                     QCoreApplication::tr("Xa1 (мм)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Sagittal::Frequency1Uid, 1,
                                                     QCoreApplication::tr("Частота 1-го максимума спектра по сагиттальной составляющей"),
                                                     QCoreApplication::tr("Yf1 (Гц)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Sagittal::Amplitude1Uid, 1,
                                                     QCoreApplication::tr("Амплитуда 1-го максимума спектра по сагиттальной составляющей"),
                                                     QCoreApplication::tr("Ya1 (мм)"),
                                                     ""))

            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Frontal::Frequency2Uid, 1,
                                                     QCoreApplication::tr("Частота 2-го максимума спектра по фронтальной составляющей"),
                                                     QCoreApplication::tr("Xf2 (Гц)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Frontal::Amplitude2Uid, 1,
                                                     QCoreApplication::tr("Амплитуда 2-го максимума спектра по фронтальной составляющей"),
                                                     QCoreApplication::tr("Xa2 (мм)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Sagittal::Frequency2Uid, 1,
                                                     QCoreApplication::tr("Частота 2-го максимума спектра по сагиттальной составляющей"),
                                                     QCoreApplication::tr("Yf2 (Гц)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Sagittal::Amplitude2Uid, 1,
                                                     QCoreApplication::tr("Амплитуда 2-го максимума спектра по сагиттальной составляющей"),
                                                     QCoreApplication::tr("Ya2 (мм)"),
                                                     ""))

            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Frontal::Frequency3Uid, 1,
                                                     QCoreApplication::tr("Частота 3-го максимума спектра по фронтальной составляющей"),
                                                     QCoreApplication::tr("Xf3 (Гц)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Frontal::Amplitude3Uid, 1,
                                                     QCoreApplication::tr("Амплитуда 3-го максимума спектра по фронтальной составляющей"),
                                                     QCoreApplication::tr("Xa3 (мм)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Sagittal::Frequency3Uid, 1,
                                                     QCoreApplication::tr("Частота 3-го максимума спектра по сагиттальной составляющей"),
                                                     QCoreApplication::tr("Yf3 (Гц)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrStabFactorsDefines::Sagittal::Amplitude3Uid, 1,
                                                     QCoreApplication::tr("Амплитуда 3-го максимума спектра по сагиттальной составляющей"),
                                                     QCoreApplication::tr("Ya3 (мм)"),
                                                     ""))

            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrSingleSignalFactorsDefines::Frequency1Uid, 2,
                                                     QCoreApplication::tr("Частота 1-го максимума спектра по вертикальной составляющей"),
                                                     QCoreApplication::tr("Zf1 (Гц)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrSingleSignalFactorsDefines::Amplitude1Uid, 2,
                                                     QCoreApplication::tr("Амплитуда 1-го максимума спектра по вертикальной составляющей"),
                                                     QCoreApplication::tr("Za1 (мм)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrSingleSignalFactorsDefines::Frequency2Uid, 2,
                                                     QCoreApplication::tr("Частота 2-го максимума спектра по вертикальной составляющей"),
                                                     QCoreApplication::tr("Zf2 (Гц)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrSingleSignalFactorsDefines::Amplitude2Uid, 2,
                                                     QCoreApplication::tr("Амплитуда 2-го максимума спектра по вертикальной составляющей"),
                                                     QCoreApplication::tr("Za2 (мм)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrSingleSignalFactorsDefines::Frequency3Uid, 2,
                                                     QCoreApplication::tr("Частота 3-го максимума спектра по вертикальной составляющей"),
                                                     QCoreApplication::tr("Zf3 (Гц)"),
                                                     ""))
            << BabyPostureCalculator::FactorInfo(std::make_tuple(
                                                     SpectrSingleSignalFactorsDefines::Amplitude3Uid, 2,
                                                     QCoreApplication::tr("Амплитуда 3-го максимума спектра по вертикальной составляющей"),
                                                     QCoreApplication::tr("Za3 (мм)"),
                                                     ""))
               ;
}

void BabyPostureCalculator::assignFactors(const QString& ciStab, const QString& ciZ,
                                          DataDefines::ProbeInfo& pi,
                                          QList<BabyPostureCalculator::FactorInfo>& factors,
                                          const bool isPrimary)
{
    auto fctClassic = new ClassicFactors(testUid(), pi.uid, ciStab);
    auto fctSpectrStab = new SpectrStabFactors(testUid(), pi.uid, ciStab);
    auto fctSpectrZ = new SpectrSingleSignalFactors(testUid(), pi.uid, ciZ);
    auto fctVector = new VectorFactors(testUid(), pi.uid, ciStab);

    for (int i = 0; i < factors.size(); ++i)
    {
        auto fi = factors.at(i);

        QString chanUid = ChannelsDefines::chanStab;
        double v = 0;
        if (fi.group == 0)
        {
            v = fctClassic->factorValue(fi.uid);
            fi.valueFmt = fctClassic->factorValueFormatted(fi.uid);
        }
        else
        if (fi.group == 1)
        {
            v = fctSpectrStab->factorValue(fi.uid);
            fi.valueFmt = fctSpectrStab->factorValueFormatted(fi.uid);
        }
        else
        if (fi.group == 2)
        {
            v = fctSpectrZ->factorValue(fi.uid);
            chanUid = ChannelsDefines::chanZ;
            fi.valueFmt = fctSpectrZ->factorValueFormatted(fi.uid);
        }
        if (fi.group == 3)
        {
            v = fctVector->factorValue(fi.uid);
            fi.valueFmt = fctVector->factorValueFormatted(fi.uid);
        }
        if (isPrimary)
            addPrimaryFactor(testUid(), fi.uid, v, 0, chanUid, pi.name);

        factors.replace(i, fi);
    }
}

