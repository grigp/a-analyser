#include "triangleconslutionfactors.h"

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "trianglefactors.h"

TriangleConslutionFactors::TriangleConslutionFactors(const QString &testUid,
                                                     const QString &probeUid,
                                                     TriangleFactors* factors,
                                                     QObject *parent)
    : ProbeMultifactor (testUid, probeUid, parent)
    , m_factors(factors)
{
    Q_ASSERT(m_factors != nullptr);
    if (isValid())
        calculate();
}

bool TriangleConslutionFactors::isValid() const
{
    return isValid(testUid(), probeUid());
}

bool TriangleConslutionFactors::isValid(const QString &testUid, const QString &probeUid)
{
    Q_UNUSED(testUid);
    auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
    auto se = DataProvider::channelExists(probeUid, ChannelsDefines::chanTriangleResult);
    return de && se;
}

void TriangleConslutionFactors::calculate()
{
    compFctAccRepeat();
    compFctAccPos();
    compFctShapeAnalysis();
    compFctCorrections();
    compFctMidErrors();
    compFctChangeFactors();
    compFctOther();

    addFactors();
}

void TriangleConslutionFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(TriangleConslutionFactorsDefines::GroupUid, tr("Показатели теста \"Треугольник\". Заключение"));

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::AccRepeatUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Точность повторения"), tr("AccRep"), tr("%"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::AccMidXUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Точность позиции (фронталь)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::AccMidYUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Точность позиции (сагитталь)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::AccFormUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Точность формы"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::KorrMotorTstCntUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Кол-во моторных коррекций (обучение)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::KorrKognTstCntUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Кол-во когнитивных коррекций (обучение)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::KorrMotorAnlCntUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Кол-во моторных коррекций (анализ)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::KorrKognAnlCntUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Кол-во когнитивных коррекций (анализ)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::KorrDominTstUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Преобладание коррекций (обучение)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::KorrDominAnlUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Преобладание коррекций (анализ)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::MidPointErrTstUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Средняя ошибка точки (обучение)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::MidSquareErrTstUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Средняя ошибка площади (обучение)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::MidSideErrTstUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Средняя ошибка длины сторон (обучение)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::MidTimeErrAnlUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Среднее изменение времени прохождения тр-ка (анализ)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::MidSquareErrAnlUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Среднее изменение площади тр-ка (анализ)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::MidPosErrAnlUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Изменение позиции треугольника"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::MidAmplErrAnlUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Среднее изменение амплитуды тр-ка (анализ)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::LatentMovingTstUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Время начала движения после появления сигнала (обучение)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::ErrCntKognTstUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Точность выполнения когнитивным сигналом (обучение)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::ErrCntMotorTstUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Точность моторной коррекции (обучение)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::ErrCntKognAnlUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Точность выполнения когнитивным сигналом (анализ)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleConslutionFactorsDefines::ErrCntMotorAnlUid, TriangleConslutionFactorsDefines::GroupUid,
                           tr("Точность моторной коррекции (анализ)"), tr(""), tr(""), 1, 3, FactorsDefines::nsDual, 12);

}

void TriangleConslutionFactors::compFctAccRepeat()
{
    auto lenT = m_factors->factorValue(TriangleFactorsDefines::Training::TimeUid);
    auto lenA = m_factors->factorValue(TriangleFactorsDefines::Analysis::TimeUid);
    double len = lenA - lenT;

    auto sqT = m_factors->factorValue(TriangleFactorsDefines::Training::SquareUid);
    auto sqA = m_factors->factorValue(TriangleFactorsDefines::Analysis::SquareUid);
    double sq = sqA - sqT;

    auto vT = m_factors->factorValue(TriangleFactorsDefines::Training::SpeedUid);
    auto vA = m_factors->factorValue(TriangleFactorsDefines::Analysis::SpeedUid);
    double v = vA - vT;

    if (len + sq + v > 0)
        m_accRepeat = 1 / ((len + sq + v) / 3) * 100;
}

void TriangleConslutionFactors::compFctAccPos()
{
    //! Ошибки верхней точки
    auto upXTrain = m_factors->factorValue(TriangleFactorsDefines::Training::UpErrSysXUid);
    auto upYTrain = m_factors->factorValue(TriangleFactorsDefines::Training::UpErrSysYUid);
    auto upXAnal = m_factors->factorValue(TriangleFactorsDefines::Analysis::UpErrSysXUid);
    auto upYAnal = m_factors->factorValue(TriangleFactorsDefines::Analysis::UpErrSysYUid);

    //! Ошибки правой точки
    auto rtXTrain = m_factors->factorValue(TriangleFactorsDefines::Training::RightErrSysXUid);
    auto rtYTrain = m_factors->factorValue(TriangleFactorsDefines::Training::RightErrSysYUid);
    auto rtXAnal = m_factors->factorValue(TriangleFactorsDefines::Analysis::RightErrSysXUid);
    auto rtYAnal = m_factors->factorValue(TriangleFactorsDefines::Analysis::RightErrSysYUid);

    //! Ошибки левой точки
    auto lfXTrain = m_factors->factorValue(TriangleFactorsDefines::Training::LeftErrSysXUid);
    auto lfYTrain = m_factors->factorValue(TriangleFactorsDefines::Training::LeftErrSysYUid);
    auto lfXAnal = m_factors->factorValue(TriangleFactorsDefines::Analysis::LeftErrSysXUid);
    auto lfYAnal = m_factors->factorValue(TriangleFactorsDefines::Analysis::LeftErrSysYUid);

    //! Ошибки центра треугольника
    auto midXTrain = m_factors->factorValue(TriangleFactorsDefines::Training::MidErrSysXUid);
    auto midYTrain = m_factors->factorValue(TriangleFactorsDefines::Training::MidErrSysYUid);
    auto midXAnal = m_factors->factorValue(TriangleFactorsDefines::Analysis::MidErrSysXUid);
    auto midYAnal = m_factors->factorValue(TriangleFactorsDefines::Analysis::MidErrSysYUid);

    m_accMidX = ( (upXAnal - upXTrain) + (rtXAnal - rtXTrain) + (lfXAnal - lfXTrain) + (midXAnal - midXTrain) ) / 4;
    m_accMidY = ( (upYAnal - upYTrain) + (rtYAnal - rtYTrain) + (lfYAnal - lfYTrain) + (midYAnal - midYTrain) ) / 4;
}

void TriangleConslutionFactors::compFctShapeAnalysis()
{
    auto topXTrain = m_factors->triangleTraining().topCorner().x();
    auto topYTrain = m_factors->triangleTraining().topCorner().y();
    auto leftXTrain = m_factors->triangleTraining().leftDownCorner().x();
    auto leftYTrain = m_factors->triangleTraining().leftDownCorner().y();
    auto rightXTrain = m_factors->triangleTraining().rightDownCorner().x();
    auto rightYTrain = m_factors->triangleTraining().rightDownCorner().y();

    auto topXAnal = m_factors->triangleAnalysis().topCorner().x();
    auto topYAnal = m_factors->triangleAnalysis().topCorner().y();
    auto leftXAnal = m_factors->triangleAnalysis().leftDownCorner().x();
    auto leftYAnal = m_factors->triangleAnalysis().leftDownCorner().y();
    auto rightXAnal = m_factors->triangleAnalysis().rightDownCorner().x();
    auto rightYAnal = m_factors->triangleAnalysis().rightDownCorner().y();

    double aTest = sqrt(pow(rightXTrain - topXTrain, 2) + pow(rightYTrain - topYTrain, 2));
    double bTest = sqrt(pow(leftXTrain - rightXTrain, 2) + pow(leftYTrain - rightYTrain, 2));
    double cTest = sqrt(pow(topXTrain - leftXTrain, 2) + pow(topYTrain - leftYTrain, 2));
    double aAnal = sqrt(pow(rightXAnal - topXAnal, 2) + pow(rightYAnal - topYAnal, 2));
    double bAnal = sqrt(pow(leftXAnal - rightXAnal, 2) + pow(leftYAnal - rightYAnal, 2));
    double cAnal = sqrt(pow(topXAnal - leftXAnal, 2) + pow(topYAnal - leftYAnal, 2));

    double a = 0;
    if (aTest > 0)
       a = (aAnal - aTest) / aTest * 100;
    double b = 0;
    if (bTest > 0)
       b = (bAnal - bTest) / bTest * 100;
    double c = 0;
    if (cTest > 0)
        c = (cAnal - cTest) / cTest * 100;
    m_accForm = (a + b + c) / 3;
}

void TriangleConslutionFactors::compFctCorrections()
{
    //! Кол-во коррекций
    m_korrMotorTstCnt = m_factors->factorValue(TriangleFactorsDefines::Training::KorrCntMotorUid);
    m_korrKognTstCnt = m_factors->factorValue(TriangleFactorsDefines::Training::KorrCntKognUid);
    m_korrMotorAnlCnt = m_factors->factorValue(TriangleFactorsDefines::Analysis::KorrCntMotorUid);
    m_korrKognAnlCnt = m_factors->factorValue(TriangleFactorsDefines::Analysis::KorrCntKognUid);

    //! Преобладание коррекций
    auto powerKogn = m_factors->factorValue(TriangleFactorsDefines::Training::KognPwrUid);
    auto powerMotor = m_factors->factorValue(TriangleFactorsDefines::Training::MotorPwrUid);
    if (powerKogn + powerMotor > 0)
        m_korrDominTst = static_cast<int>((powerKogn - powerMotor) / (powerKogn + powerMotor) * 100);

    powerKogn = m_factors->factorValue(TriangleFactorsDefines::Analysis::KognPwrUid);
    powerMotor = m_factors->factorValue(TriangleFactorsDefines::Analysis::MotorPwrUid);
    if (powerKogn + powerMotor > 0)
        m_korrDominAnl = static_cast<int>((powerKogn - powerMotor) / (powerKogn + powerMotor) * 100);
}

void TriangleConslutionFactors::compFctMidErrors()
{
    //! Средняя ошибка по точкам
    auto stdTopX = m_factors->triangleTraining().topCorner().x();
    auto stdTopY = m_factors->triangleTraining().topCorner().y();
    auto stdLeftX = m_factors->triangleTraining().leftDownCorner().x();
    auto stdLeftY = m_factors->triangleTraining().leftDownCorner().y();
    auto stdRightX = m_factors->triangleTraining().rightDownCorner().x();
    auto stdRightY = m_factors->triangleTraining().rightDownCorner().y();

    auto trgTopX = m_factors->triangleOriginal().topCorner().x();
    auto trgTopY = m_factors->triangleOriginal().topCorner().y();
    auto trgLeftX = m_factors->triangleOriginal().leftDownCorner().x();
    auto trgLeftY = m_factors->triangleOriginal().leftDownCorner().y();
    auto trgRightX = m_factors->triangleOriginal().rightDownCorner().x();
    auto trgRightY = m_factors->triangleOriginal().rightDownCorner().y();

    m_midPointErrTst = (fabs(trgTopX - stdTopX) + fabs(trgTopY - stdTopY) +
                        fabs(trgLeftX - stdLeftX) + fabs(trgLeftY - stdLeftY) +
                        fabs(trgRightX - stdRightX) + fabs(trgRightY - stdRightY)) / 6;


    //! Средняя ошибка по площади
    auto sTrain = m_factors->triangleTraining().square();
    auto sOrig = m_factors->triangleOriginal().square();
    if (sOrig > 0)
        m_midSquareErrTst = (sTrain - sOrig) / sOrig * 100;

    //! Средняя ошибка по сторонам
    m_midSideErrTst = (fabs(m_factors->triangleTraining().sideTL() - m_factors->triangleOriginal().sideTL()) +
                       fabs(m_factors->triangleTraining().sideTR() - m_factors->triangleOriginal().sideTR()) +
                       fabs(m_factors->triangleTraining().sideLR() - m_factors->triangleOriginal().sideLR())) / 3;
}

void TriangleConslutionFactors::compFctChangeFactors()
{
    //! Изменение времени прохождения треугольника на этапе анализа
    auto aTest = m_factors->factorValue(TriangleFactorsDefines::Training::TimeUid);
    auto aAnal = m_factors->factorValue(TriangleFactorsDefines::Analysis::TimeUid);
    if (aTest > 0)
        m_midTimeErrAnl = aAnal / aTest;

    //! Изменение площади треугольника на этапе анализа
    aTest = m_factors->factorValue(TriangleFactorsDefines::Training::SquareUid);
    aAnal = m_factors->factorValue(TriangleFactorsDefines::Analysis::SquareUid);
    if (aTest > 0)
        m_midSquareErrAnl = (aAnal - aTest) / aTest * 100;

    //! Изменение позиции треугольника
    m_midPosErrAnl = ((fabs(m_factors->triangleAnalysis().topCorner().x() - m_factors->triangleTraining().topCorner().x()) +
                        fabs(m_factors->triangleAnalysis().topCorner().y() - m_factors->triangleTraining().topCorner().y()) / 2 +
                       (fabs(m_factors->triangleAnalysis().leftDownCorner().x() - m_factors->triangleTraining().leftDownCorner().x())) +
                        fabs(m_factors->triangleAnalysis().leftDownCorner().y() - m_factors->triangleTraining().leftDownCorner().y())) / 2 +
                       (fabs(m_factors->triangleAnalysis().rightDownCorner().x() - m_factors->triangleTraining().rightDownCorner().x()) +
                        fabs(m_factors->triangleAnalysis().rightDownCorner().y() - m_factors->triangleTraining().rightDownCorner().y())) / 2) / 3;

    //! Изменение амплитуды треугольников на этапе анализа
    m_midAmplErrAnl = (fabs(m_factors->triangleAnalysis().sideTL() - m_factors->triangleTraining().sideTL()) +
                       fabs(m_factors->triangleAnalysis().sideTR() - m_factors->triangleTraining().sideTR()) +
                       fabs(m_factors->triangleAnalysis().sideLR() - m_factors->triangleTraining().sideLR())) / 3;

}

void TriangleConslutionFactors::compFctOther()
{
    //! Время начала движения после появления сигнала
    m_latentMovingTst = m_factors->factorValue(TriangleFactorsDefines::LatentMovingUid);

    //! Ошибки
    m_errCntMotorTst = m_factors->factorValue(TriangleFactorsDefines::Training::MotorErrUid);
    m_errCntKognTst = m_factors->factorValue(TriangleFactorsDefines::Training::KognErrUid);
    m_errCntMotorAnl = m_factors->factorValue(TriangleFactorsDefines::Analysis::MotorErrUid);
    m_errCntKognAnl = m_factors->factorValue(TriangleFactorsDefines::Analysis::KognErrUid);
}

void TriangleConslutionFactors::addFactors()
{
    addFactor(TriangleConslutionFactorsDefines::AccRepeatUid, m_accRepeat);
    addFactor(TriangleConslutionFactorsDefines::AccMidXUid, m_accMidX);
    addFactor(TriangleConslutionFactorsDefines::AccMidYUid, m_accMidY);
    addFactor(TriangleConslutionFactorsDefines::AccFormUid, m_accForm);
    addFactor(TriangleConslutionFactorsDefines::KorrMotorTstCntUid, m_korrMotorTstCnt);
    addFactor(TriangleConslutionFactorsDefines::KorrKognTstCntUid, m_korrKognTstCnt);
    addFactor(TriangleConslutionFactorsDefines::KorrMotorAnlCntUid, m_korrMotorAnlCnt);
    addFactor(TriangleConslutionFactorsDefines::KorrKognAnlCntUid, m_korrKognAnlCnt);
    addFactor(TriangleConslutionFactorsDefines::KorrDominTstUid, m_korrDominTst);
    addFactor(TriangleConslutionFactorsDefines::KorrDominAnlUid, m_korrDominAnl);
    addFactor(TriangleConslutionFactorsDefines::MidPointErrTstUid, m_midPointErrTst);
    addFactor(TriangleConslutionFactorsDefines::MidSquareErrTstUid, m_midSquareErrTst);
    addFactor(TriangleConslutionFactorsDefines::MidSideErrTstUid, m_midSideErrTst);

    addFactor(TriangleConslutionFactorsDefines::MidTimeErrAnlUid, m_midTimeErrAnl);
    addFactor(TriangleConslutionFactorsDefines::MidSquareErrAnlUid, m_midSquareErrAnl);
    addFactor(TriangleConslutionFactorsDefines::MidPosErrAnlUid, m_midPosErrAnl);
    addFactor(TriangleConslutionFactorsDefines::MidAmplErrAnlUid, m_midAmplErrAnl);

    addFactor(TriangleConslutionFactorsDefines::LatentMovingTstUid, m_latentMovingTst);

    addFactor(TriangleConslutionFactorsDefines::ErrCntKognTstUid, m_errCntKognTst);
    addFactor(TriangleConslutionFactorsDefines::ErrCntMotorTstUid, m_errCntMotorTst);
    addFactor(TriangleConslutionFactorsDefines::ErrCntKognAnlUid, m_errCntKognAnl);
    addFactor(TriangleConslutionFactorsDefines::ErrCntMotorAnlUid, m_errCntMotorAnl);
}



