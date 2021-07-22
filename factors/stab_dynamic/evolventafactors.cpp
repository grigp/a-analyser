#include "evolventafactors.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "evolventaresultdata.h"
#include "stabilogram.h"
#include "decartcoordinatessignal.h"
#include "fragment.h"

namespace
{
/*!
 * \brief Изменение значения функции CHangeVALueDIRection ChValDir enum
 */
enum ChValDir
{
      cvdNoData = -10
    , cvdDecrease = -1
    , cvdNo = 0
    , cvdIncreace = 1
};

}

EvolventaFactors::EvolventaFactors(const QString &testUid,
                                   const QString &probeUid,
                                   QObject *parent)
    : ProbeMultifactor (testUid,probeUid, parent)
{
    if (isValid())
        calculate();
}

EvolventaFactors::~EvolventaFactors()
{
    if (m_stab)
        delete m_stab;
    if (m_target)
        delete m_target;
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

void showFragment(QList<Fragment*> &fragments, const QString name)
{
    QString s = "";
    foreach (auto frag, fragments)
        s = s + QString::number(frag->begin()) + ":" + QString::number(frag->end()) + " ";
    qDebug() << name + " ) " + s;
}

void EvolventaFactors::calculate()
{
    getMarkStages();
    getSignals();
    fillListsFragments(0, m_extXList, m_brnXList);
    fillListsFragments(1, m_extYList, m_brnYList);
    calculateFragmentsFactors(0, m_extXList, m_fragExtValuesX);
    calculateFragmentsFactors(1, m_extYList, m_fragExtValuesY);
    calculateFragmentsFactors(0, m_brnXList, m_fragBrnValuesX);
    calculateFragmentsFactors(1, m_brnYList, m_fragBrnValuesY);
    calculateErrorsFactors(0, m_ceValuesX);
    calculateErrorsFactors(1, m_ceValuesY);
    calculateWEFactors();

    addFactor(EvolventaFactorsDefines::CommonErrorsFrontal::SummErr, m_ceValuesX.summErr);
    addFactor(EvolventaFactorsDefines::CommonErrorsFrontal::MidErr, m_ceValuesX.midErr);
    addFactor(EvolventaFactorsDefines::FragmentExtremumFrontal::MidErr, m_fragExtValuesX.midErr);
    addFactor(EvolventaFactorsDefines::FragmentBranchFrontal::MidErr, m_fragBrnValuesX.midErr);
    addFactor(EvolventaFactorsDefines::FragmentExtremumFrontal::CrsCnt, m_fragExtValuesX.crsCnt);
    addFactor(EvolventaFactorsDefines::FragmentBranchFrontal::CrsCnt, m_fragBrnValuesX.crsCnt);
    addFactor(EvolventaFactorsDefines::FragmentExtremumFrontal::ExtrCnt, m_fragExtValuesX.extrCnt);
    addFactor(EvolventaFactorsDefines::FragmentBranchFrontal::ExtrCnt, m_fragBrnValuesX.extrCnt);
    addFactor(EvolventaFactorsDefines::FragmentExtremumFrontal::MidAmpl, m_fragExtValuesX.midAmpl);
    addFactor(EvolventaFactorsDefines::FragmentBranchFrontal::MidAmpl, m_fragBrnValuesX.midAmpl);
    addFactor(EvolventaFactorsDefines::FragmentExtremumFrontal::CMidErr, m_fragExtValuesX.cMidAmp);
    addFactor(EvolventaFactorsDefines::FragmentBranchFrontal::CMidErr, m_fragBrnValuesX.cMidAmp);
    addFactor(EvolventaFactorsDefines::FragmentExtremumFrontal::CMidAmp, m_fragExtValuesX.cMidAmp);
    addFactor(EvolventaFactorsDefines::FragmentBranchFrontal::CMidAmp, m_fragBrnValuesX.cMidAmp);
    addFactor(EvolventaFactorsDefines::Frontal::InterCntR, m_frontalValues.interCntR);
    addFactor(EvolventaFactorsDefines::Frontal::InterLenR, m_frontalValues.interLenR);
    addFactor(EvolventaFactorsDefines::Frontal::SemiWavLen, m_frontalValues.semiWavLen);
    addFactor(EvolventaFactorsDefines::Frontal::SemiWavAmpl, m_frontalValues.semiWavAmpl);

    addFactor(EvolventaFactorsDefines::CommonErrorsSagittal::SummErr, m_ceValuesY.summErr);
    addFactor(EvolventaFactorsDefines::CommonErrorsSagittal::MidErr, m_ceValuesY.midErr);
    addFactor(EvolventaFactorsDefines::FragmentExtremumSagittal::MidErr, m_fragExtValuesY.midErr);
    addFactor(EvolventaFactorsDefines::FragmentBranchSagittal::MidErr, m_fragBrnValuesY.midErr);
    addFactor(EvolventaFactorsDefines::FragmentExtremumSagittal::CrsCnt, m_fragExtValuesY.crsCnt);
    addFactor(EvolventaFactorsDefines::FragmentBranchSagittal::CrsCnt, m_fragBrnValuesY.crsCnt);
    addFactor(EvolventaFactorsDefines::FragmentExtremumSagittal::ExtrCnt, m_fragExtValuesY.extrCnt);
    addFactor(EvolventaFactorsDefines::FragmentBranchSagittal::ExtrCnt, m_fragBrnValuesY.extrCnt);
    addFactor(EvolventaFactorsDefines::FragmentExtremumSagittal::MidAmpl, m_fragExtValuesY.midAmpl);
    addFactor(EvolventaFactorsDefines::FragmentBranchSagittal::MidAmpl, m_fragBrnValuesY.midAmpl);
    addFactor(EvolventaFactorsDefines::FragmentExtremumSagittal::CMidErr, m_fragExtValuesY.cMidAmp);
    addFactor(EvolventaFactorsDefines::FragmentBranchSagittal::CMidErr, m_fragBrnValuesY.cMidAmp);
    addFactor(EvolventaFactorsDefines::FragmentExtremumSagittal::CMidAmp, m_fragExtValuesY.cMidAmp);
    addFactor(EvolventaFactorsDefines::FragmentBranchSagittal::CMidAmp, m_fragBrnValuesY.cMidAmp);
    addFactor(EvolventaFactorsDefines::Sagittal::InterCntR, m_sagittalValues.interCntR);
    addFactor(EvolventaFactorsDefines::Sagittal::InterLenR, m_sagittalValues.interLenR);
    addFactor(EvolventaFactorsDefines::Sagittal::SemiWavLen, m_sagittalValues.semiWavLen);
    addFactor(EvolventaFactorsDefines::Sagittal::SemiWavAmpl, m_sagittalValues.semiWavAmpl);

    addFactor(EvolventaFactorsDefines::DAPercent, m_commonValues.dAPercent);
    addFactor(EvolventaFactorsDefines::DAhead, m_commonValues.dAhead);
    addFactor(EvolventaFactorsDefines::SemiWavLenDAC, m_commonValues.semiWavLenDAC);
    addFactor(EvolventaFactorsDefines::SemiWavAmplDAC, m_commonValues.semiWavAmplDAC);
    addFactor(EvolventaFactorsDefines::KorrCount, m_commonValues.korrCount);

    addFactor(EvolventaFactorsDefines::CorrectionsMotor::Percent, m_motorCorrValues.percent);
    addFactor(EvolventaFactorsDefines::CorrectionsMotor::Error, m_motorCorrValues.error);
    addFactor(EvolventaFactorsDefines::CorrectionsMotor::Amplitude, m_motorCorrValues.amplitude);
    addFactor(EvolventaFactorsDefines::CorrectionsMotor::TimeSumm, m_motorCorrValues.timeSumm);
    addFactor(EvolventaFactorsDefines::CorrectionsMotor::TimeMid, m_motorCorrValues.timeMid);
    addFactor(EvolventaFactorsDefines::CorrectionsMotor::Power, m_motorCorrValues.power);

    addFactor(EvolventaFactorsDefines::CorrectionsKognitive::Percent, m_kognCorrValues.percent);
    addFactor(EvolventaFactorsDefines::CorrectionsKognitive::Error, m_kognCorrValues.error);
    addFactor(EvolventaFactorsDefines::CorrectionsKognitive::Amplitude, m_kognCorrValues.amplitude);
    addFactor(EvolventaFactorsDefines::CorrectionsKognitive::TimeSumm, m_kognCorrValues.timeSumm);
    addFactor(EvolventaFactorsDefines::CorrectionsKognitive::TimeMid, m_kognCorrValues.timeMid);
    addFactor(EvolventaFactorsDefines::CorrectionsKognitive::Power, m_kognCorrValues.power);
}

void EvolventaFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(EvolventaFactorsDefines::GroupUid, tr("Показатели теста \"Эвольвента\""));

    ///<---------------------------------------------------------------------------
    ///< Фронтальная составляющая
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CommonErrorsFrontal::SummErr, EvolventaFactorsDefines::GroupUid,
                           tr("Суммарная ошибка"), tr("SummErr"), tr("мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CommonErrorsFrontal::MidErr, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка"), tr("MidErr"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentExtremumFrontal::MidErr, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка на экстремуме"), tr("MidErrExtr"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentBranchFrontal::MidErr, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка на переходе"), tr("MidErrBr"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentExtremumFrontal::CrsCnt, EvolventaFactorsDefines::GroupUid,
                           tr("Сред. кол-во пересечений на экстремуме"), tr("CrsCntExtr"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentBranchFrontal::CrsCnt, EvolventaFactorsDefines::GroupUid,
                           tr("Сред. кол-во пересечений на переходе"), tr("CrsCntBr"), tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentExtremumFrontal::ExtrCnt, EvolventaFactorsDefines::GroupUid,
                           tr("Среднее кол-во пиков на экстремуме"), tr("ExtrCntE"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentBranchFrontal::ExtrCnt, EvolventaFactorsDefines::GroupUid,
                           tr("Среднее кол-во пиков на переходе"), tr("ExtrCntB"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentExtremumFrontal::MidAmpl, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя амплитуда между пиками на экстремуме"), tr("MidAmplE"), tr("мм"), 3, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentBranchFrontal::MidAmpl, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя амплитуда между пиками на переходе"), tr("MidAmplB"), tr("мм"), 3, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentExtremumFrontal::CMidErr, EvolventaFactorsDefines::GroupUid,
                           tr("Изменение средней ошибки на экстремуме"), tr("CMidErrE"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentBranchFrontal::CMidErr, EvolventaFactorsDefines::GroupUid,
                           tr("Изменение средней ошибки на переходе"), tr("CMidErrB"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentExtremumFrontal::CMidAmp, EvolventaFactorsDefines::GroupUid,
                           tr("Изменение средней амплитуды на экстремуме"), tr("CMidAmplE"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentBranchFrontal::CMidAmp, EvolventaFactorsDefines::GroupUid,
                           tr("Изменение средней амплитуды на переходе"), tr("CMidAmplB"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);

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
            registerFactor(EvolventaFactorsDefines::CommonErrorsSagittal::SummErr, EvolventaFactorsDefines::GroupUid,
                           tr("Суммарная ошибка"), tr("SummErr"), tr("мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::CommonErrorsSagittal::MidErr, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка"), tr("MidErr"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentExtremumSagittal::MidErr, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка на экстремуме"), tr("MidErrExtr"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentBranchSagittal::MidErr, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя ошибка на переходе"), tr("MidErrBr"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentExtremumSagittal::CrsCnt, EvolventaFactorsDefines::GroupUid,
                           tr("Сред. кол-во пересечений на экстремуме"), tr("CrsCntExtr"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentBranchSagittal::CrsCnt, EvolventaFactorsDefines::GroupUid,
                           tr("Сред. кол-во пересечений на переходе"), tr("CrsCntBr"), tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentExtremumSagittal::ExtrCnt, EvolventaFactorsDefines::GroupUid,
                           tr("Среднее кол-во пиков на экстремуме"), tr("ExtrCntE"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentBranchSagittal::ExtrCnt, EvolventaFactorsDefines::GroupUid,
                           tr("Среднее кол-во пиков на переходе"), tr("ExtrCntB"), tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentExtremumSagittal::MidAmpl, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя амплитуда между пиками на экстремуме"), tr("MidAmplE"), tr("мм"), 3, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentBranchSagittal::MidAmpl, EvolventaFactorsDefines::GroupUid,
                           tr("Средняя амплитуда между пиками на переходе"), tr("MidAmplB"), tr("мм"), 3, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentExtremumSagittal::CMidErr, EvolventaFactorsDefines::GroupUid,
                           tr("Изменение средней ошибки на экстремуме"), tr("CMidErrE"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentBranchSagittal::CMidErr, EvolventaFactorsDefines::GroupUid,
                           tr("Изменение средней ошибки на переходе"), tr("CMidErrB"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentExtremumSagittal::CMidAmp, EvolventaFactorsDefines::GroupUid,
                           tr("Изменение средней амплитуды на экстремуме"), tr("CMidAmplE"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(EvolventaFactorsDefines::FragmentBranchSagittal::CMidAmp, EvolventaFactorsDefines::GroupUid,
                           tr("Изменение средней амплитуды на переходе"), tr("CMidAmplB"), tr("мм"), 2, 3, FactorsDefines::nsDual, 12);

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

void EvolventaFactors::getMarkStages()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanEvolventaResult, baData))
    {
        EvolventaResultData probeData(baData);
        m_timeUpwinding = probeData.timeUpwinding();
        m_timeHold = probeData.timeHold();
        m_timeConvolution = probeData.timeConvolution();
        m_freq = probeData.freq();
    }
}

void EvolventaFactors::getSignals()
{
    //! Чтение стабилограммы
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStab, baStab))
        m_stab = new Stabilogram(baStab);

    //! Чтение траектории цели
    QByteArray baTarget;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanTargetTraectory, baTarget))
        m_target = new DecartCoordinatesSignal(baTarget);
}

void EvolventaFactors::fillListsFragments(const int chan, QList<Fragment*> &extList, QList<Fragment*> &brnList)
{
    extList.clear();
    brnList.clear();

    //! Выход для слишком малого сигнала
    if (m_target->size() < 10)
        return;

    ChValDir cvd = cvdNoData;
    QVector<int> el;
    //! Поиск экстремумов. Цикл по сигналу
    for (int i = 1; i < m_target->size(); ++i)
    {
        double v1 = m_target->value(chan, i - 1);
        double v2 = m_target->value(chan, i);

        if (v2 > v1)
        {
            if (cvd != cvdIncreace)  //! Минимум
                el.append(i);
            cvd = cvdIncreace;
        }
        else
        if (v2 < v1)
        {
            if (cvd != cvdDecrease)  //! Максимум
                el.append(i);
            cvd = cvdDecrease;
        }
        else
        if (v2 == v2)
            cvd = cvdNo;
    }

    //! Построение списка диапазонов
    if (el.size() > 2)
    {
        for (int i = 0; i < el.size(); ++i)
        {
            if (i == 0)               //! Первый диапазон
                extList.append(new Fragment(0, el[i] + (el[i+1] - el[i]) / 4, m_freq));
            else
            if (i == el.size() - 1)   //! Последний диапазон
                extList.append(new Fragment(el[i] - (el[i] - el[i-1]) / 4, m_target->size() - 1, m_freq));
            else                      //! Промежуточные диапазоны
                extList.append(new Fragment(el[i] - (el[i] - el[i-1]) / 4,
                                            el[i] + (el[i+1] - el[i]) / 4, m_freq));
        }

        // Диапазоны переходов (промежутки между диапазонами экстремумов)
        for (int i = 0; i < extList.size() - 2; ++i)
            brnList.append(new Fragment(extList[i]->end() + 1, extList[i+1]->begin() - 1, m_freq));
    }
}

bool EvolventaFactors::isExtremum(const int idx, const int begin, const int end,
                                  const double v, const double vp, const double vn)
{
    if (idx > begin && idx < end && vp < INT_MAX && vn < INT_MAX)
        return (v > vp && v > vn) || (v < vp && v < vn);
    return false;
}

void EvolventaFactors::calculateFragmentsFactors(const int chan,
                                                 QList<Fragment *> &fragList,
                                                 EvolventaFactorsDefines::FragmentFactors &factors)
{
    factors.midErr = 0;
    factors.crsCnt = 0;
    factors.extrCnt = 0;
    factors.midAmpl = 0;
    factors.cMidErr = 0;
    factors.cMidAmp = 0;

    int cf = 0;   ///< Текущий фрагмент
    bool isFrag = false;
    bool isFirstExtr = true;
    double aVal = 0;
    double vp = INT_MAX;
    double vn = INT_MAX;
    double v = 0;
    double me = 0;
    double cc = 0;
    int ec = 0;
    double ma = 0;
    double chMidAmpl1 = 0;
    double chMidAmpl2 = 0;
    double chMidErr1 = 0;
    double chMidErr2 = 0;

    //! У стабилограммы и траектории цели могут быть разные размеры
    int size = qMin(m_stab->size(), m_target->size());
    //! Цикл по стабилограмме
    for (int i = 0; i < size; ++i)
    {
        vn = m_stab->value(chan, i);
        double tv = m_target->value(chan, i);
        double tvp = 0;
        if (i > 0)
            tvp = m_target->value(chan, i - 1);

        //! Внутри текущего фрагмента
        if (i >= fragList[cf]->begin() && i <= fragList[cf]->end())
        {
            isFrag = true;

            //! Средняя ошибка
            me = me + fabs(vn - tv);
            //! Кол-во переходов
            if (i > 0 && (BaseUtils::sign(v - tvp) != BaseUtils::sign(vn - tv)))
              ++cc;
            //! Показатели экстремумов
            if (isExtremum(i, fragList[cf]->begin(), fragList[cf]->end(), v, vp, vn))
            {
                //! Кол-во экстремумов
                ++ec;
                //! Амплитуда между экстремумами
                if (!isFirstExtr)
                {
                  ma = ma + fabs(vn - aVal);
                  //! Изменение средней амплитуды
                  if (i <= (fragList[cf]->begin() + fragList[cf]->end()) / 2)
                    chMidAmpl1 = chMidAmpl1 + fabs(vn - aVal);
                  else
                    chMidAmpl2 = chMidAmpl2 + fabs(vn - aVal);

                  aVal = vn;
                }
                isFirstExtr = false;
            }
            //! Изменение средней ошибки
             if (i <= (fragList[cf]->begin() + fragList[cf]->end()) / 2)
              chMidErr1 = chMidErr1 + fabs(vn - tv);
            else
              chMidErr2 = chMidErr2 + fabs(vn - tv);
        }
        else
        //! Вышли за пределы текущего фрагмента
        {
            if (isFrag)
            {
                factors.midErr = factors.midErr + me / static_cast<double>(fragList[cf]->end() - fragList[cf]->begin()); //! Средняя ошибка
                factors.crsCnt = factors.crsCnt + cc;  //! Кол-во переходов
                factors.extrCnt = factors.extrCnt + ec;
                factors.midAmpl = factors.midAmpl + ma;
                factors.cMidErr = factors.cMidErr + chMidErr2 - chMidErr1;
                factors.cMidAmp = factors.cMidAmp + chMidAmpl2 - chMidAmpl1;

                me = 0; cc = 0; ec = 0;
                ma = 0; aVal = 0;
                vp = INT_MAX; vn = INT_MAX;
                chMidErr1 = 0; chMidErr2 = 0;
                chMidAmpl1 = 0; chMidAmpl2 = 0;

                isFirstExtr = true;
                ++cf;
                if (cf >= fragList.size())
                    break;
            }
            isFrag = false;
        }

        vp = v;
        v = vn;
    }

    if (fragList.size() > 0)
    {
      factors.midErr = factors.midErr / fragList.size();
      factors.crsCnt = factors.crsCnt / fragList.size();
      factors.extrCnt = factors.extrCnt / fragList.size();
      factors.midAmpl = factors.midAmpl / fragList.size();
      factors.cMidErr = factors.cMidErr / fragList.size();
      factors.cMidAmp = factors.cMidAmp / fragList.size();
    }
}

void EvolventaFactors::calculateErrorsFactors(const int chan, EvolventaFactorsDefines::CommonErrorsFactorValues &factors)
{
    factors.summErr = 0;
    factors.midErr = 0;

    //! У стабилограммы и траектории цели могут быть разные размеры
    int size = qMin(m_stab->size(), m_target->size());
    //! Цикл по стабилограмме
    for (int i = 0; i < size; ++i)
    {
        double v = m_stab->value(chan, i);
        double tv = m_target->value(chan, i);
        factors.summErr = factors.summErr + fabs(v - tv);
    }
    if (size > 0)
        factors.midErr = factors.summErr / static_cast<double>(size);
}

void EvolventaFactors::calculateWEFactors()
{
    m_frontalValues.interCntR = 0;
    m_frontalValues.interLenR = 0;
    m_sagittalValues.interCntR = 0;
    m_sagittalValues.interLenR = 0;
    m_commonValues.dAhead = 0;
    m_motorCorrValues.error = 0;
    m_motorCorrValues.amplitude = 0;
    m_motorCorrValues.timeSumm = 0;
    m_kognCorrValues.error = 0;
    m_kognCorrValues.amplitude = 0;
    m_kognCorrValues.timeSumm = 0;

    double x{0}, y{0};
    double tx{0}, ty{0};
    bool firstFC = true;                       //! Первая итерация расчета показателей
    int posVX{0}, posVY{0};
    int sWLenX{0}, sWLenY{0};                  //! Счетчики длительности полуволн
    int sWCountX{0}, sWCountY{0};
    double dAheadCur = 0;                      //! Опережение или отставание
    int dAPlCnt{0}, dAMiCnt{0};                //! Кол-во опережений и отставаний
    double ovx{0}, ovy{0}, ovDAC{0};           //! Предыдущие значения отклонений
    int dirX{0}, dirY{0}, dirDAC{0};           //! Направление изменения сигнала
    int extrIX{-1}, extrIY{-1}, extrIDAC{-1};  //! Индекс точки экстремума
    double extrVX{0}, extrVY{0}, extrDAC{0};   //! Значение сигнала в точке экстремума
    double timeX{0}, timeY{0}, timeDAC{0}, timeI{0};  //! Средние длительность и амплитуда
    double amplX{0}, amplY{0}, amplDAC{0}, amplI{0};
    int extrCntX{0}, extrCntY{0}, extrCntDAC{0};      //! Кол-во экстремумов
    double errExtr{0};                                //! Ошибка на экстремуме
    int zoneMotor{0}, zoneKogn{0};                    //! Кол-во полуволн для каждой зоны

    //! У стабилограммы и траектории цели могут быть разные размеры
    int size = qMin(m_stab->size(), m_target->size());
    //! Цикл по стабилограмме
    for (int i = 0; i < size; ++i)
    {
        x = m_stab->value(0, i);
        y = m_stab->value(1, i);
        tx = m_target->value(0, i);
        ty = m_target->value(1, i);

        double vx = x - tx;
        double vy = y - ty;

        //! На этапе удержания амплитуды
        if (i >= m_timeHold && i < m_timeConvolution)
        {
            if (firstFC)
            {
                posVX = BaseUtils::sign(vx);
                posVY = BaseUtils::sign(vy);
            }
            else
            {
                if (BaseUtils::sign(vx) != posVX && BaseUtils::sign(vx) != 0)
                {
                    m_frontalValues.interCntR = m_frontalValues.interCntR + 1;
                    posVX = BaseUtils::sign(vx);
                    m_frontalValues.interLenR = m_frontalValues.interLenR + (sWLenX / m_freq);
                    ++sWCountX;
                    sWLenX = 0;
                }
                else
                    ++sWLenX;
                if (BaseUtils::sign(vy) != posVY && BaseUtils::sign(vy) != 0)
                {
                    m_sagittalValues.interCntR = m_sagittalValues.interCntR + 1;
                    posVY = BaseUtils::sign(vy);
                    m_sagittalValues.interLenR = m_sagittalValues.interLenR + (sWLenY / m_freq);
                    ++sWCountY;
                    sWLenY = 0;
                }
                else
                    ++sWLenY;
            }
        }

        //! Определение, опережает ли маркер цель
        dAheadCur = getAheadValue (tx, ty, x, y);
        m_commonValues.dAhead = m_commonValues.dAhead + dAheadCur;
        if (dAheadCur > 0)
            ++dAPlCnt;
        else
            ++dAMiCnt;

        //! Амплитуда и длительность полуволны
        if (i > 0)
        {
            //! Экстремум по фронтали
            if (((vx < ovx) && (dirX == BaseUtils::PositiveValue)) ||
                ((vx > ovx) && (dirX == BaseUtils::NegativeValue)))
            {
                if ((extrIX > -1) && (static_cast<double>(i - extrIX) / static_cast<double>(m_freq) >= 0.05))
                {
                  timeX = timeX + static_cast<double>(i - extrIX) / static_cast<double>(m_freq);
                  amplX = amplX + fabs(vx - extrVX);

                  ++extrCntX;
                }

                extrIX = i;
                extrVX = vx;
            }

            //! Экстремум по сагиттали
            if (((vy < ovy) && (dirY == BaseUtils::PositiveValue)) ||
                ((vy > ovy) && (dirY == BaseUtils::NegativeValue)))
            {
                if ((extrIY > -1) && (static_cast<double>(i - extrIY) / static_cast<double>(m_freq) >= 0.05))
                {
                  timeY = timeY + static_cast<double>(i - extrIY) / static_cast<double>(m_freq);
                  amplY = amplY + fabs(vx - extrVY);

                  ++extrCntY;
                }

                extrIY = i;
                extrVY = vy;
            }

            //! Экстремум по динамике опережения
            if (((dAheadCur < ovDAC) && (dirDAC == BaseUtils::PositiveValue)) ||
                ((dAheadCur > ovDAC) && (dirDAC == BaseUtils::NegativeValue)))
            {
              if (extrDAC > -1)
              {
                timeI = static_cast<double>(i - extrIDAC) / static_cast<double>(m_freq);
                amplI = fabs(dAheadCur - extrDAC);
                timeDAC = timeDAC + timeI;
                amplDAC = amplDAC + amplI;

                //! Ошибка на экстремуме
                errExtr = sqrt(pow(vx, 2) + pow(vy, 2));

                //! Расчет кол-ва экстремумов по зонам
                if ((timeI >= EvolventaFactorsDefines::ZoneMotorLo) && (timeI <= EvolventaFactorsDefines::ZoneMotorHi))
                {
                  ++zoneMotor;
                  m_motorCorrValues.error = m_motorCorrValues.error + errExtr;
                  m_motorCorrValues.amplitude = m_motorCorrValues.amplitude + amplI;
                  m_motorCorrValues.timeSumm = m_motorCorrValues.timeSumm + timeI;
                }
                else
                if ((timeI >= EvolventaFactorsDefines::ZoneKognLo) && (timeI <= EvolventaFactorsDefines::ZoneKognHi))
                {
                  ++zoneKogn;
                  m_kognCorrValues.error = m_kognCorrValues.error + errExtr;
                  m_kognCorrValues.amplitude = m_kognCorrValues.amplitude + amplI;
                  m_kognCorrValues.timeSumm = m_kognCorrValues.timeSumm + timeI;
                }
                ++extrCntDAC;
              }

              extrIDAC = i;
              extrDAC = dAheadCur;
            }
        }

        dirX = BaseUtils::sign(vx - ovx);
        dirY = BaseUtils::sign(vy - ovy);
        dirDAC = BaseUtils::sign(dAheadCur - ovDAC);
        ovx = vx;
        ovy = vy;
        ovDAC = dAheadCur;
        firstFC = false;
    }

    if (extrCntX > 0)
    {
        m_frontalValues.semiWavLen = timeX / static_cast<double>(extrCntX);
        m_frontalValues.semiWavAmpl = amplX / static_cast<double>(extrCntX);
    }
    if (extrCntY > 0)
    {
        m_sagittalValues.semiWavLen = timeY / static_cast<double>(extrCntY);
        m_sagittalValues.semiWavAmpl = amplY / static_cast<double>(extrCntY);
    }
    if (extrCntDAC > 0)
    {
        m_commonValues.semiWavLenDAC = timeDAC / static_cast<double>(extrCntDAC);
        m_commonValues.semiWavAmplDAC = amplDAC / static_cast<double>(extrCntDAC);
    }

    if (sWCountX > 0)
        m_frontalValues.interLenR = m_frontalValues.interLenR / sWCountX;
    if (sWCountY > 0)
        m_sagittalValues.interLenR = m_sagittalValues.interLenR / sWCountY;
    if (dAPlCnt + dAMiCnt > 0)
        m_commonValues.dAPercent = static_cast<double>(dAPlCnt - dAMiCnt) / static_cast<double>(dAPlCnt + dAMiCnt) * 100;

    m_commonValues.korrCount = extrCntDAC;
    if (extrCntDAC > 0)
    {
        m_motorCorrValues.percent = static_cast<double>(zoneMotor) / static_cast<double>(extrCntDAC) * 100;
        m_kognCorrValues.percent = static_cast<double>(zoneKogn) / static_cast<double>(extrCntDAC) * 100;
    }

    if (zoneMotor > 0)
    {
        m_motorCorrValues.error = m_motorCorrValues.error / static_cast<double>(zoneMotor);
        m_motorCorrValues.amplitude = m_motorCorrValues.amplitude / static_cast<double>(zoneMotor);
        m_motorCorrValues.timeMid = m_motorCorrValues.timeSumm / static_cast<double>(zoneMotor);
    }
    if (zoneKogn > 0)
    {
        m_kognCorrValues.error = m_kognCorrValues.error / static_cast<double>(zoneKogn);
        m_kognCorrValues.amplitude = m_kognCorrValues.amplitude / static_cast<double>(zoneKogn);
        m_kognCorrValues.timeMid = m_kognCorrValues.timeSumm / static_cast<double>(zoneKogn);
    }
    m_motorCorrValues.power = (m_motorCorrValues.timeMid - EvolventaFactorsDefines::ZoneMotorLo) * m_motorCorrValues.amplitude * zoneMotor;
    m_kognCorrValues.power = (m_kognCorrValues.timeMid - EvolventaFactorsDefines::ZoneKognLo) * m_kognCorrValues.amplitude * zoneKogn;
}

double EvolventaFactors::getAheadValue(const double tx, const double ty, const double x, const double y) const
{
    double tr{0}, tph{0}, mr{0}, mph{0};

    BaseUtils::convertDecartToPolar(tx, ty, tr, tph );
    BaseUtils::convertDecartToPolar(x, y, mr, mph );

    if ((tph < M_PI_4) && (mph > 7 * M_PI_4))
        return  tph - (mph - 2 * M_PI);
    else
    if ((mph < M_PI_4) && (tph > 7 * M_PI_4))
        return tph - M_PI * 2 - mph;
    else
        return tph - mph;
}

