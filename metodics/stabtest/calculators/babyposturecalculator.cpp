#include "babyposturecalculator.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "classicfactors.h"
#include "spectrsinglesignalfactors.h"
#include "vectorfactors.h"

BabyPostureCalculator::BabyPostureCalculator(const QString &testUid, QObject *parent)
    : StabTestCalculator(testUid, parent)
{

}

BabyPostureCalculator::~BabyPostureCalculator()
{
    if (m_fctClassic)
        delete m_fctClassic;
    if (m_fctSpectr)
        delete m_fctSpectr;
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
                m_fctSpectr = new SpectrSingleSignalFactors(testUid(), pi.uid, ChannelsDefines::chanZ);
                m_freq1 = m_fctSpectr->factorValueFormatted(SpectrSingleSignalFactorsDefines::Frequency1Uid);
                addPrimaryFactor(testUid(), SpectrSingleSignalFactorsDefines::Frequency1Uid,
                                 m_fctSpectr->factorValue(SpectrSingleSignalFactorsDefines::Frequency1Uid),
                                 0, ChannelsDefines::chanStab, pi.name);

                m_freq2 = m_fctSpectr->factorValueFormatted(SpectrSingleSignalFactorsDefines::Frequency2Uid);
                addPrimaryFactor(testUid(), SpectrSingleSignalFactorsDefines::Frequency2Uid,
                                 m_fctSpectr->factorValue(SpectrSingleSignalFactorsDefines::Frequency2Uid),
                                 0, ChannelsDefines::chanStab, pi.name);

                m_freq3 = m_fctSpectr->factorValueFormatted(SpectrSingleSignalFactorsDefines::Frequency3Uid);
                addPrimaryFactor(testUid(), SpectrSingleSignalFactorsDefines::Frequency3Uid,
                                 m_fctSpectr->factorValue(SpectrSingleSignalFactorsDefines::Frequency3Uid),
                                 0, ChannelsDefines::chanStab, pi.name);

                m_ampl1 = m_fctSpectr->factorValueFormatted(SpectrSingleSignalFactorsDefines::Amplitude1Uid);
                addPrimaryFactor(testUid(), SpectrSingleSignalFactorsDefines::Amplitude1Uid,
                                 m_fctSpectr->factorValue(SpectrSingleSignalFactorsDefines::Amplitude1Uid),
                                 0, ChannelsDefines::chanStab, pi.name);

                m_ampl2 = m_fctSpectr->factorValueFormatted(SpectrSingleSignalFactorsDefines::Amplitude2Uid);
                addPrimaryFactor(testUid(), SpectrSingleSignalFactorsDefines::Amplitude2Uid,
                                 m_fctSpectr->factorValue(SpectrSingleSignalFactorsDefines::Amplitude2Uid),
                                 0, ChannelsDefines::chanStab, pi.name);

                m_ampl3 = m_fctSpectr->factorValueFormatted(SpectrSingleSignalFactorsDefines::Amplitude3Uid);
                addPrimaryFactor(testUid(), SpectrSingleSignalFactorsDefines::Amplitude3Uid,
                                 m_fctSpectr->factorValue(SpectrSingleSignalFactorsDefines::Amplitude3Uid),
                                 0, ChannelsDefines::chanStab, pi.name);


                m_fctClassic = new ClassicFactors(testUid(), pi.uid, ChannelsDefines::chanStab);
                m_s = m_fctClassic->factorValueFormatted(ClassicFactorsDefines::SquareUid);
                addPrimaryFactor(testUid(), ClassicFactorsDefines::SquareUid,
                                 m_fctClassic->factorValue(ClassicFactorsDefines::SquareUid),
                                 0, ChannelsDefines::chanStab, pi.name);

                m_ellLen = m_fctClassic->factorValueFormatted(ClassicFactorsDefines::EllLengthUid);
                addPrimaryFactor(testUid(), ClassicFactorsDefines::EllLengthUid,
                                 m_fctClassic->factorValue(ClassicFactorsDefines::EllLengthUid),
                                 0, ChannelsDefines::chanStab, pi.name);

                m_lfs = m_fctClassic->factorValueFormatted(ClassicFactorsDefines::LFSUid);
                addPrimaryFactor(testUid(), ClassicFactorsDefines::LFSUid,
                                 m_fctClassic->factorValue(ClassicFactorsDefines::LFSUid),
                                 0, ChannelsDefines::chanStab, pi.name);

                m_ellE = m_fctClassic->factorValueFormatted(ClassicFactorsDefines::ComprRatioUid);
                addPrimaryFactor(testUid(), ClassicFactorsDefines::ComprRatioUid,
                                 m_fctClassic->factorValue(ClassicFactorsDefines::ComprRatioUid),
                                 0, ChannelsDefines::chanStab, pi.name);

                m_fctVector = new VectorFactors(testUid(), pi.uid, ChannelsDefines::chanStab);
                m_kfr = m_fctVector->factorValueFormatted(VectorFactorsDefines::KFRUid);
                addPrimaryFactor(testUid(), VectorFactorsDefines::KFRUid,
                                 m_fctVector->factorValue(VectorFactorsDefines::KFRUid),
                                 0, ChannelsDefines::chanStab, pi.name);
            }
        }
    }
}

void BabyPostureCalculator::fastCalculate()
{
    StabTestCalculator::fastCalculate();
}
