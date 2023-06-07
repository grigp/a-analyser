#include "pulsespectrfactors.h"

#include "aanalyserapplication.h"
#include "channelsutils.h"
#include "dataprovider.h"

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


    addFactor(PulseSpectrFactorsDefines::PwHFUid, 0);
    addFactor(PulseSpectrFactorsDefines::PwLFUid, 0);
    addFactor(PulseSpectrFactorsDefines::PwVLFUid, 0);
    addFactor(PulseSpectrFactorsDefines::PwULFUid, 0);
    addFactor(PulseSpectrFactorsDefines::PwSumUid, 0);
    addFactor(PulseSpectrFactorsDefines::PrHFUid, 0);
    addFactor(PulseSpectrFactorsDefines::PrLFUid, 0);
    addFactor(PulseSpectrFactorsDefines::PrVLFUid, 0);
    addFactor(PulseSpectrFactorsDefines::PrULFUid, 0);
    addFactor(PulseSpectrFactorsDefines::PwAvHFUid, 0);
    addFactor(PulseSpectrFactorsDefines::PwAvLFUid, 0);
    addFactor(PulseSpectrFactorsDefines::PwAvVLFUid, 0);
    addFactor(PulseSpectrFactorsDefines::PwAvULFUid, 0);
    addFactor(PulseSpectrFactorsDefines::LHUid, 0);
    addFactor(PulseSpectrFactorsDefines::Pw60Uid, 0);
    addFactor(PulseSpectrFactorsDefines::CentIdxUid, 0);

    addFactor(PulseSpectrFactorsDefines::FqMidSpUid, 0);
    addFactor(PulseSpectrFactorsDefines::LFnuUid, 0);
    addFactor(PulseSpectrFactorsDefines::HFnuUid, 0);
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

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::FqMidSpUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Средняя частота спектра"), tr("Fm"), tr("Гц"), 3, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::LFnuUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Доля мощности низких частот в нормализованных единицах"), tr("LFn"), tr("%"), 2, 1, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(PulseSpectrFactorsDefines::HFnuUid, PulseSpectrFactorsDefines::GroupUid,
                           tr("Доля мощности высоких частот в нормализованных единицах"), tr("HFn"), tr("%"), 2, 1, FactorsDefines::nsDual, 12);
}
