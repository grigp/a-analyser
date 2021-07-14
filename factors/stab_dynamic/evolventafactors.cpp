#include "evolventafactors.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"

EvolventaFactors::EvolventaFactors(const QString &testUid,
                                   const QString &probeUid,
                                   QObject *parent)
    : ProbeMultifactor (testUid,probeUid, parent)
{
    if (isValid())
        calculate();
}

bool EvolventaFactors::isValid() const
{
    return isValid(testUid(), probeUid());
}

bool EvolventaFactors::isValid(const QString &testUid, const QString &probeUid)
{
    Q_UNUSED(testUid);
    auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
    auto se = DataProvider::channelExists(probeUid, ChannelsDefines::chanEvolventaResult);
    return de && se;
}

void EvolventaFactors::calculate()
{

}

void EvolventaFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(EvolventaFactorsDefines::GroupUid, tr("Показатели теста \"Эвольвента\""));

    ///<---------------------------------------------------------------------------
    ///< Фронтальная составляющая
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::SummErr, EvolventaFactorsDefines::GroupUid,
                           tr("Суммарная ошибка"), tr("SummErr"), tr("мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::MidErr, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка"), tr("MidErr"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::MidErrE, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка на экстремуме"), tr("MidErrExtr"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::MidErrB, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка на переходе"), tr("MidErrBr"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::CrsCntE, EvolventaFactorsDefines::GroupUid,
                           tr("Сред. кол-во пересечений на экстремуме"), tr("CrsCntExtr"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::CrsCntB, EvolventaFactorsDefines::GroupUid,
                           tr("Сред. кол-во пересечений на переходе"), tr("CrsCntBr"), tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::ExtrCntE, EvolventaFactorsDefines::GroupUid,
                           tr("Среднее кол-во пиков на экстремуме"), tr("ExtrCntE"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::ExtrCntB, EvolventaFactorsDefines::GroupUid,
                           tr("Среднее кол-во пиков на переходе"), tr("ExtrCntB"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::MidAmplE, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя амплитуда между пиками на экстремуме"), tr("MidAmplE"), tr("мм"), 3, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::MidAmplB, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя амплитуда между пиками на переходе"), tr("MidAmplB"), tr("мм"), 3, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::CMidErrE, EvolventaFactorsDefines::GroupUid,
                           tr("Изменение средней ошибки на экстремуме"), tr("CMidErrE"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::CMidAmpE, EvolventaFactorsDefines::GroupUid,
                           tr("Изменение средней амплитуды на экстремуме"), tr("CMidAmplE"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);

    //// R - retention
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::InterCntR, EvolventaFactorsDefines::GroupUid,
                           tr("Кол-во пересечений траектории цели при удержании"), tr("InterCntR"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::InterLenR, EvolventaFactorsDefines::GroupUid,
                           tr("Длит. между пересечениями траектории цели при удержании"), tr("InterLenR"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::SemiWavLen, EvolventaFactorsDefines::GroupUid,
                           tr("Длительность полуволны"), tr("SemiWavLen"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Frontal::SemiWavAmpl, EvolventaFactorsDefines::GroupUid,
                           tr("Амплитуда полуволны"), tr("SemiWavAmpl"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);

    ///<---------------------------------------------------------------------------
    ///< Сагиттальная составляющая
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::SummErr, EvolventaFactorsDefines::GroupUid,
                           tr("Суммарная ошибка"), tr("SummErr"), tr("мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::MidErr, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка"), tr("MidErr"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::MidErrE, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка на экстремуме"), tr("MidErrExtr"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::MidErrB, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка на переходе"), tr("MidErrBr"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::CrsCntE, EvolventaFactorsDefines::GroupUid,
                           tr("Сред. кол-во пересечений на экстремуме"), tr("CrsCntExtr"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::CrsCntB, EvolventaFactorsDefines::GroupUid,
                           tr("Сред. кол-во пересечений на переходе"), tr("CrsCntBr"), tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::ExtrCntE, EvolventaFactorsDefines::GroupUid,
                           tr("Среднее кол-во пиков на экстремуме"), tr("ExtrCntE"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::ExtrCntB, EvolventaFactorsDefines::GroupUid,
                           tr("Среднее кол-во пиков на переходе"), tr("ExtrCntB"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::MidAmplE, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя амплитуда между пиками на экстремуме"), tr("MidAmplE"), tr("мм"), 3, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::MidAmplB, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя амплитуда между пиками на переходе"), tr("MidAmplB"), tr("мм"), 3, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::CMidErrE, EvolventaFactorsDefines::GroupUid,
                           tr("Изменение средней ошибки на экстремуме"), tr("CMidErrE"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::CMidAmpE, EvolventaFactorsDefines::GroupUid,
                           tr("Изменение средней амплитуды на экстремуме"), tr("CMidAmplE"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);

    //// R - retention
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::InterCntR, EvolventaFactorsDefines::GroupUid,
                           tr("Кол-во пересечений траектории цели при удержании"), tr("InterCntR"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::InterLenR, EvolventaFactorsDefines::GroupUid,
                           tr("Длит. между пересечениями траектории цели при удержании"), tr("InterLenR"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::SemiWavLen, EvolventaFactorsDefines::GroupUid,
                           tr("Длительность полуволны"), tr("SemiWavLen"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::Sagittal::SemiWavAmpl, EvolventaFactorsDefines::GroupUid,
                           tr("Амплитуда полуволны"), tr("SemiWavAmpl"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);

    ///<---------------------------------------------------------------------------
    ///< Общие показатели коррекций
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::DAPercent, EvolventaFactorsDefines::GroupUid,
                           tr("Опережение маркера цели"), tr("DAPercent"), tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::DAhead, EvolventaFactorsDefines::GroupUid,
                           tr("Суммарное опережение маркера цели"), tr("DAhead"), tr("сек"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::SemiWavLenDAC, EvolventaFactorsDefines::GroupUid,
                           tr("Длительность полуволны при опережении"), tr("SemiWavLenDAC"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::SemiWavAmplDAC, EvolventaFactorsDefines::GroupUid,
                           tr("Амплитуда полуволны при опережении"), tr("SemiWavAmplDAC"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::KorrCount, EvolventaFactorsDefines::GroupUid,
                           tr("Общее количество коррекций"), tr("CorrCount"), tr(""), 0, 3, FactorsDefines::nsDual, 12);

    ///<---------------------------------------------------------------------------
    ///< Моторные коррекции
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CorrectionsMotor::Percent, EvolventaFactorsDefines::GroupUid,
                           tr("Процент коррекций"), tr("Percent"), tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CorrectionsMotor::Error, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка траектории"), tr("Error"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CorrectionsMotor::Amplitude, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя амплитуда"), tr("Amplitude"), tr("мм"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CorrectionsMotor::TimeSumm, EvolventaFactorsDefines::GroupUid,
                           tr("Суммарная длительность"), tr("TimeSumm"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CorrectionsMotor::TimeMid, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя длительность"), tr("TimeMid"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CorrectionsMotor::Power, EvolventaFactorsDefines::GroupUid,
                           tr("Мощность коррекций"), tr("Power"), tr("мм/сек"), 2, 3, FactorsDefines::nsDual, 12);

    ///<---------------------------------------------------------------------------
    ///< Когнитивные коррекции
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CorrectionsKognitive::Percent, EvolventaFactorsDefines::GroupUid,
                           tr("Процент коррекций"), tr("Percent"), tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CorrectionsKognitive::Error, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка траектории"), tr("Error"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CorrectionsKognitive::Amplitude, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя амплитуда"), tr("Amplitude"), tr("мм"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CorrectionsKognitive::TimeSumm, EvolventaFactorsDefines::GroupUid,
                           tr("Суммарная длительность"), tr("TimeSumm"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CorrectionsKognitive::TimeMid, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя длительность"), tr("TimeMid"), tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CorrectionsKognitive::Power, EvolventaFactorsDefines::GroupUid,
                           tr("Мощность коррекций"), tr("Power"), tr("мм/сек"), 2, 3, FactorsDefines::nsDual, 12);
}
