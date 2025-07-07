#include "trianglefactors.h"

#include <QDebug>

#include "baseutils.h"
#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "stabilogram.h"
#include "triangleresultdata.h"

namespace
{
static const int PercentPointInCorner = 2;

}

TriangleFactors::TriangleFactors(const QString &testUid,
                                 const QString &probeUid,
                                 QObject *parent)
    : ProbeMultifactor (testUid, probeUid, parent)
{
    if (isValid())
        calculate();
}

QString TriangleFactors::uid() const
{
    return TriangleFactorsDefines::GroupUid;
}

QString TriangleFactors::name() const
{
    return nameAsConst();
}

QString TriangleFactors::nameAsConst()
{
    return QCoreApplication::tr("Показатели теста \"Треугольник\"");
}

bool TriangleFactors::isValid() const
{
    return isValid(testUid(), probeUid());
}

bool TriangleFactors::isValid(const QString &testUid, const QString &probeUid)
{
    Q_UNUSED(testUid);
    auto de = DataProvider::channelExists(probeUid, ChannelsDefines::chanStab);
    auto se = DataProvider::channelExists(probeUid, ChannelsDefines::chanTriangleResult);
    return de && se;
}

void TriangleFactors::calculate()
{
    getTriangleData();
    readSignal();
    computeTrianglesBounds();
    computeTriangles();
    computeSKOValues();
    computePosDeviations(0, m_firstAnalysisTriangle - 1, m_upDevTest, m_rtDevTest, m_lfDevTest, m_midDevTest);
    computePosDeviations(m_firstAnalysisTriangle, m_triangles.size() - 1, m_upDevAnal, m_rtDevAnal, m_lfDevAnal, m_midDevAnal);
    computeLatentMoving();
    computeCorrectionsFactors();

    addFactors();
}

void TriangleFactors::registerFactors()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerGroup(TriangleFactorsDefines::GroupUid, nameAsConst());

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::TimeUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя длительность прохода (обучение)"), QCoreApplication::tr("LenTest"), QCoreApplication::tr("сек"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::TimeQUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Разброс длительности прохода (обучение)"), QCoreApplication::tr("LenQTest"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::SquareUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя площадь треугольников (обучение)"), QCoreApplication::tr("SqrTest"), QCoreApplication::tr("кв.мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::SquareQUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Разброс площади треугольников (обучение)"), QCoreApplication::tr("SqrQTest"), QCoreApplication::tr("кв.мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::SpeedUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя скорость прохождения (обучение)"), QCoreApplication::tr("SpdTest"), QCoreApplication::tr("мм/сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::SpeedQUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Разброс скорости прохождения (обучение)"), QCoreApplication::tr("SpdQTest"), QCoreApplication::tr("мм/сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Среднее смещение треуг. по фронтали (обучение)"), QCoreApplication::tr("TrXTest"), QCoreApplication::tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Среднее смещение треуг. по сагиттали (обучение)"), QCoreApplication::tr("TrYTest"), QCoreApplication::tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::AngleUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Угол наклона треугольника (обучение)"), QCoreApplication::tr("AngleTest"), QCoreApplication::tr("град"), 2, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::UpErrSysXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка верхней вершины X (обучение)"), QCoreApplication::tr("UESXTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::UpErrSysYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка верхней вершины Y (обучение)"), QCoreApplication::tr("UESYTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::UpErrRndXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка верхней вершины X (обучение)"), QCoreApplication::tr("UERXTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::UpErrRndYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка верхней вершины Y (обучение)"), QCoreApplication::tr("UERYTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::RightErrSysXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка правой вершины X (обучение)"), QCoreApplication::tr("RESXTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::RightErrSysYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка правой вершины Y (обучение)"), QCoreApplication::tr("RESYTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::RightErrRndXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка правой вершины X (обучение)"), QCoreApplication::tr("RERXTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::RightErrRndYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка правой вершины Y (обучение)"), QCoreApplication::tr("RERYTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::LeftErrSysXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка левой вершины X (обучение)"), QCoreApplication::tr("LESXTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::LeftErrSysYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка левой вершины Y (обучение)"), QCoreApplication::tr("LESYTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::LeftErrRndXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка левой вершины X (обучение)"), QCoreApplication::tr("LERXTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::LeftErrRndYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка левой вершины Y (обучение)"), QCoreApplication::tr("LERYTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MidErrSysXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка центра треугольника X (обучение)"), QCoreApplication::tr("MESXTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MidErrSysYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка центра треугольника Y (обучение)"), QCoreApplication::tr("MESYTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MidErrRndXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка центра треугольника X (обучение)"), QCoreApplication::tr("MERXTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MidErrRndYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка центра треугольника Y (обучение)"), QCoreApplication::tr("MERYTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::SemiWavLenDACUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя длительность коррекций (обучение)"), QCoreApplication::tr("SWLDACTest"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::SemiWavAmplDACUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя амплитуда коррекций (обучение)"), QCoreApplication::tr("SWADACTest"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::KorrCntKognUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Кол-во когнитивных коррекций (обучение)"), QCoreApplication::tr("KKCntTest"), QCoreApplication::tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::KorrCntMotorUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Кол-во моторных коррекций (обучение)"), QCoreApplication::tr("KMCntTest"), QCoreApplication::tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MotorPercUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Процент моторных коррекций (обучение)"), QCoreApplication::tr("MPercTest"), QCoreApplication::tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::KognPercUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Процент когнитивных коррекций (обучение)"), QCoreApplication::tr("KPercTest"), QCoreApplication::tr("%"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MotorErrUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя ошибка моторных коррекций (обучение)"), QCoreApplication::tr("MErrTest"), QCoreApplication::tr("мм"), 3, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::KognErrUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя ошибка когнитивных коррекций (обучение)"), QCoreApplication::tr("KErrTest"), QCoreApplication::tr("мм"), 3, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MotorMidAUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя амплитуда моторных коорекций (обучение)"), QCoreApplication::tr("MMidTest"), QCoreApplication::tr("мм"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::KognMidAUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя амплитуда когнитивных коорекций (обучение)"), QCoreApplication::tr("KMidTest"), QCoreApplication::tr("мм"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MotorSumTUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Суммарная длительность моторных коррекций (обучение)"), QCoreApplication::tr("MSumTest"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::KognSumTUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Суммарная длительность когнитивных коррекций (обучение)"), QCoreApplication::tr("KSumTest"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MotorMidTUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя длительность моторных коррекций (обучение)"), QCoreApplication::tr("MMidTest"), QCoreApplication::tr("сек"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::KognMidTUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя длительность когнитивных коррекций (обучение)"), QCoreApplication::tr("KMidTest"), QCoreApplication::tr("сек"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Training::MotorPwrUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Мощность моторных коррекций (обучение)"), QCoreApplication::tr("MPowTest"), QCoreApplication::tr("мм/сек"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
          registerFactor(TriangleFactorsDefines::Training::KognPwrUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Мощность когнитивных коррекций (обучение)"), QCoreApplication::tr("KPowTest"), QCoreApplication::tr("мм/сек"), 4, 3, FactorsDefines::nsDual, 12);


    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::TimeUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя длительность прохода (анализ)"), QCoreApplication::tr("LenAnal"), QCoreApplication::tr("сек"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::TimeQUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Разброс длительности прохода (анализ)"), QCoreApplication::tr("LenQAnal"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::SquareUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя площадь треугольников (анализ)"), QCoreApplication::tr("SqrAnal"), QCoreApplication::tr("кв.мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::SquareQUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Разброс площади треугольников (анализ)"), QCoreApplication::tr("SqrQAnal"), QCoreApplication::tr("кв.мм"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::SpeedUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя скорость прохождения (анализ)"), QCoreApplication::tr("SpdAnal"), QCoreApplication::tr("мм/сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::SpeedQUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Разброс скорости прохождения (анализ)"), QCoreApplication::tr("SpdQAnal"), QCoreApplication::tr("мм/сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Среднее смещение треуг. по фронтали (анализ)"), QCoreApplication::tr("TrXAnal"), QCoreApplication::tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Среднее смещение треуг. по сагиттали (анализ)"), QCoreApplication::tr("TrYTest"), QCoreApplication::tr("мм"), 1, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::AngleUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Угол наклона треугольника (анализ)"), QCoreApplication::tr("AngleAnal"), QCoreApplication::tr("град"), 2, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::UpErrSysXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка верхней вершины X (анализ)"), QCoreApplication::tr("UESXAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::UpErrSysYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка верхней вершины Y (анализ)"), QCoreApplication::tr("UESYAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::UpErrRndXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка верхней вершины X (анализ)"), QCoreApplication::tr("UERXAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::UpErrRndYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка верхней вершины Y (анализ)"), QCoreApplication::tr("UERYAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::RightErrSysXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка правой вершины X (анализ)"), QCoreApplication::tr("RESXAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::RightErrSysYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка правой вершины Y (анализ)"), QCoreApplication::tr("RESYAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::RightErrRndXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка правой вершины X (анализ)"), QCoreApplication::tr("RERXAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::RightErrRndYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка правой вершины Y (анализ)"), QCoreApplication::tr("RERYAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::LeftErrSysXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка левой вершины X (анализ)"), QCoreApplication::tr("LESXAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::LeftErrSysYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка левой вершины Y (анализ)"), QCoreApplication::tr("LESYAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::LeftErrRndXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка левой вершины X (анализ)"), QCoreApplication::tr("LERXAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::LeftErrRndYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка левой вершины Y (анализ)"), QCoreApplication::tr("LERYAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MidErrSysXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка центра треугольника X (анализ)"), QCoreApplication::tr("MESXAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MidErrSysYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Системная ошибка центра треугольника Y (анализ)"), QCoreApplication::tr("MESYAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MidErrRndXUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка центра треугольника X (анализ)"), QCoreApplication::tr("MERXAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MidErrRndYUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Случайная ошибка центра треугольника Y (анализ)"), QCoreApplication::tr("MERYAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::SemiWavLenDACUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя длительность коррекций (анализ)"), QCoreApplication::tr("SWLDACAnal"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::SemiWavAmplDACUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя амплитуда коррекций (анализ)"), QCoreApplication::tr("SWADACAnal"), QCoreApplication::tr("мм"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::KorrCntKognUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Кол-во когнитивных коррекций (анализ)"), QCoreApplication::tr("KKCntAnal"), QCoreApplication::tr(""), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::KorrCntMotorUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Кол-во моторных коррекций (анализ)"), QCoreApplication::tr("KMCntAnal"), QCoreApplication::tr(""), 0, 3, FactorsDefines::nsDual, 12);

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MotorPercUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Процент моторных коррекций (анализ)"), QCoreApplication::tr("MPercAnal"), QCoreApplication::tr("%"), 0, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::KognPercUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Процент когнитивных коррекций (анализ)"), QCoreApplication::tr("KPercAnal"), QCoreApplication::tr("%"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MotorErrUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя ошибка моторных коррекций (анализ)"), QCoreApplication::tr("MErrAnal"), QCoreApplication::tr("мм"), 3, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::KognErrUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя ошибка когнитивных коррекций (анализ)"), QCoreApplication::tr("KErrAnal"), QCoreApplication::tr("мм"), 3, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MotorMidAUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя амплитуда моторных коорекций (анализ)"), QCoreApplication::tr("MMidAnal"), QCoreApplication::tr("мм"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::KognMidAUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя амплитуда когнитивных коорекций (анализ)"), QCoreApplication::tr("KMidAnal"), QCoreApplication::tr("мм"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MotorSumTUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Суммарная длительность моторных коррекций (анализ)"), QCoreApplication::tr("MSumAnal"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::KognSumTUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Суммарная длительность когнитивных коррекций (анализ)"), QCoreApplication::tr("KSumAnal"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MotorMidTUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя длительность моторных коррекций (анализ)"), QCoreApplication::tr("MMidAnal"), QCoreApplication::tr("сек"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::KognMidTUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Средняя длительность когнитивных коррекций (анализ)"), QCoreApplication::tr("KMidAnal"), QCoreApplication::tr("сек"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::Analysis::MotorPwrUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Мощность моторных коррекций (анализ)"), QCoreApplication::tr("MPowAnal"), QCoreApplication::tr("мм/сек"), 4, 3, FactorsDefines::nsDual, 12);
    static_cast<AAnalyserApplication*>(QApplication::instance())->
          registerFactor(TriangleFactorsDefines::Analysis::KognPwrUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Мощность когнитивных коррекций (анализ)"), QCoreApplication::tr("KPowAnal"), QCoreApplication::tr("мм/сек"), 4, 3, FactorsDefines::nsDual, 12);


    static_cast<AAnalyserApplication*>(QApplication::instance())->
            registerFactor(TriangleFactorsDefines::LatentMovingUid, TriangleFactorsDefines::GroupUid,
                           QCoreApplication::tr("Время начала движения после появления сигнала"), QCoreApplication::tr("LatMoving"), QCoreApplication::tr("сек"), 2, 3, FactorsDefines::nsDual, 12);

}

int TriangleFactors::freq() const
{
    if (m_resData)
        return m_resData->freq();
    return 128;
}

int TriangleFactors::diap() const
{
    if (m_resData)
        return m_resData->diap();
    return 128;
}

int TriangleFactors::trainingLength() const
{
    if (m_resData)
        return m_resData->trainingLength();
    return 0;
}

int TriangleFactors::signalLength() const
{
    return m_x.size();
}

TriangleDefines::Triangle TriangleFactors::triangleOriginal()
{
    if (m_resData)
        return TriangleDefines::Triangle(m_resData->topCorner(),
                                         m_resData->leftDownCorner(),
                                         m_resData->rightDownCorner());
    return TriangleDefines::Triangle(QPointF(), QPointF(), QPointF());
}

TriangleDefines::Triangle TriangleFactors::triangleTraining()
{
    return m_triangleAverageTraining;
}

TriangleDefines::Triangle TriangleFactors::triangleAnalysis()
{
    return m_triangleAverageAnalysis;
}

int TriangleFactors::trianglesCount() const
{
    return m_triangleSections.size();
}

BaseDefines::Section TriangleFactors::triangleSection(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_triangleSections.size());
    return m_triangleSections.at(idx);
}

TriangleDefines::Triangle TriangleFactors::triangle(const int idx) const
{
    Q_ASSERT(idx >= 0 && idx < m_triangles.size());
    return m_triangles.at(idx);
}

int TriangleFactors::getFactorsOfStagesCount()
{
    return m_factorsOfStages.size();
}

bool TriangleFactors::getFactorOfStages(const int idx, QString &uidT, QString &uidA)
{
    if (idx >= 0 && idx < m_factorsOfStages.size())
    {
        uidT = m_factorsOfStages.at(idx).first;
        uidA = m_factorsOfStages.at(idx).second;
        return true;
    }
    return false;
}

void TriangleFactors::readSignal()
{
    m_x.clear();
    m_xf.clear();
    m_y.clear();
    m_yf.clear();

    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStab, baStab))
    {
        Stabilogram stab(baStab);
        m_freq = stab.frequency();

        for (int i = 0; i < stab.size(); ++i)
        {
            double x = stab.value(0, i);
            double y = stab.value(1, i);
            m_x << x;
            m_y << y;
            m_xf << x;
            m_yf << y;
        }

        //! Фильтрация
        auto filtration = [&](QVector<double> &buffer)
        {
            BaseUtils::filterLowFreq(buffer, m_freq, 0.2, BaseUtils::fkChebyshev, 0, buffer.size() - 1);
            BaseUtils::swapVector(buffer);
            BaseUtils::filterLowFreq(buffer, m_freq, 0.2, BaseUtils::fkChebyshev, 0, buffer.size() - 1);
            BaseUtils::swapVector(buffer);
        };

        filtration(m_xf);
        filtration(m_yf);

//        BaseUtils::vectorToText(m_x, "c:/1/x.txt");
//        BaseUtils::vectorToText(m_xf, "c:/1/x_f.txt");
//        BaseUtils::vectorToText(m_y, "c:/1/y.txt");
//        BaseUtils::vectorToText(m_yf, "c:/1/y_f.txt");
    }
}

void TriangleFactors::computeTrianglesBounds()
{
    m_triangleSections.clear();

    int dir = -2;
    int begin = -1;
    int end = -1;
    for (int i = 0; i < m_yf.size(); ++i)
        if (i > 0)
        {
            bool isUpZero = ((i <= m_resData->trainingLength()) && (m_y.at(i) > 0) && (m_y.at(i - 1) > 0)) ||  //! Исключаем дребезг при начале
                            (i > m_resData->trainingLength());
            if ((m_yf.at(i) < m_yf.at(i - 1)) && (dir == 0 || dir == 1) && isUpZero)
            {
                end = i;
                if (begin > -1 && end > -1)
                    m_triangleSections << BaseDefines::Section(begin, end);
                //! Первый треугольник на этапе анализа
                if (begin < m_resData->trainingLength() && end >= m_resData->trainingLength())
                    m_firstAnalysisTriangle = m_triangleSections.size();
                if (begin == m_resData->trainingLength())
                    m_firstAnalysisTriangle = m_triangleSections.size() - 1;
                begin = i;
            }

            if (m_yf.at(i) > m_yf.at(i - 1))
                dir = 1;
            else
            if (m_yf.at(i) < m_yf.at(i - 1))
                dir = -1;
            else
                dir = 0;
        }
}

void TriangleFactors::computeTriangles()
{

    int n = 0;
    m_triangleAverageTraining = TriangleDefines::Triangle();
    m_triangleAverageAnalysis = TriangleDefines::Triangle();
    foreach (auto section, m_triangleSections)
    {
        //! Векторы: исходный и повернутые на +120 и -120 градусов
        QVector<QPointF> vn;
        QVector<QPointF> vrr;
        QVector<QPointF> vrl;
        double l = 0;
        double xp = 0;
        double yp = 0;
        for (int i = section.begin; i < section.end; ++i)
        {
            double x = m_x.at(i);
            double y = m_y.at(i);
            vn << QPointF(x, y);

            double xr = x;
            double yr = y;
            BaseUtils::rotatePoint(x, y, - 2 * M_PI / 3, xr, yr);
            vrr << QPointF(xr, yr);

            BaseUtils::rotatePoint(x, y, 2 * M_PI / 3, xr, yr);
            vrl << QPointF(xr, yr);

            if (i > section.begin)
                l = l + sqrt(pow(x - xp, 2) + pow(y - yp, 2));
            xp = x;
            yp = y;
        }

        //! Расчет координат вершин
        auto t = computeCorner(vn);
        auto ld = computeCorner(vrl);
        auto rd = computeCorner(vrr);

        //! Обратное вращение
        double x = ld.x();
        double y = ld.y();
        double xr = x;
        double yr = y;
        BaseUtils::rotatePoint(x, y, - 2 * M_PI / 3, xr, yr);
        ld.setX(xr);
        ld.setY(yr);
        x = rd.x();
        y = rd.y();
        BaseUtils::rotatePoint(x, y, 2 * M_PI / 3, xr, yr);
        rd.setX(xr);
        rd.setY(yr);

        //! Заполнение массива координат вершин
        TriangleDefines::Triangle trngl(t, ld, rd);
        double tm = static_cast<double>(vn.size()) / m_resData->freq();
        trngl.setTimeFactors(tm, l / tm);
        m_triangles << trngl;

        //! Расчет координат вершин усредненных треугольников
        auto averaging = [&](TriangleDefines::Triangle &triangle)
        {
            triangle.setTopCorner(QPointF(triangle.topCorner().x() + t.x(), triangle.topCorner().y() + t.y()));
            triangle.setLeftDownCorner(QPointF(triangle.leftDownCorner().x() + ld.x(), triangle.leftDownCorner().y() + ld.y()));
            triangle.setRightDownCorner(QPointF(triangle.rightDownCorner().x() + rd.x(), triangle.rightDownCorner().y() + rd.y()));
            triangle.setTimeFactors(triangle.time() + trngl.time(), triangle.speed() + trngl.speed());
        };
        if (n < m_firstAnalysisTriangle)
            averaging(m_triangleAverageTraining);
        else
            averaging(m_triangleAverageAnalysis);

        ++n;
    }

    //! Расчет координат вершин усредненных треугольников - деление на количество
    auto averaging = [&](TriangleDefines::Triangle  &triangle, const int count)
    {
        triangle.setTopCorner(QPointF(triangle.topCorner().x() / count, triangle.topCorner().y() / count));
        triangle.setLeftDownCorner(QPointF(triangle.leftDownCorner().x() / count, triangle.leftDownCorner().y() / count));
        triangle.setRightDownCorner(QPointF(triangle.rightDownCorner().x() / count, triangle.rightDownCorner().y() / count));
        triangle.calculate();
        triangle.setTimeFactors(triangle.time() / count, triangle.speed() / count);
    };
    averaging(m_triangleAverageTraining, m_firstAnalysisTriangle);
    averaging(m_triangleAverageAnalysis, m_triangles.size() - m_firstAnalysisTriangle);

}

bool coordinateTopLessThan(QPointF &p1, QPointF &p2)
{
    return p1.y() < p2.y();
}

QPointF TriangleFactors::computeCorner(QVector<QPointF> &stab)
{
    //! Сортируем массив
    std::sort(stab.begin(), stab.end(), coordinateTopLessThan);

    //! Кол-во точек
    int n = stab.size() / (100 / PercentPointInCorner);

    //! Выбор первых N% точек
    double mx = 0;
    double my = 0;
    for (int i = stab.size() - 1; i > stab.size() - 1 - n; --i)
    {
        mx = mx + stab.at(i).x();
        my = my + stab.at(i).y();
    }
    mx = mx / n;
    my = my / n;

    return QPointF(mx, my);
}

void TriangleFactors::computeSKOValues()
{
    double timeMoT = 0;
    double squareMoT = 0;
    double speedMoT = 0;
    double timeMoA = 0;
    double squareMoA = 0;
    double speedMoA = 0;
    for (int i = 0; i < m_triangles.size(); ++i)
    {
        if (i < m_firstAnalysisTriangle)
        {
            timeMoT += m_triangles.at(i).time();
            squareMoT += m_triangles.at(i).square();
            speedMoT += m_triangles.at(i).speed();
        }
        else
        {
            timeMoA += m_triangles.at(i).time();
            squareMoA += m_triangles.at(i).square();
            speedMoA += m_triangles.at(i).speed();
        }
    }
    timeMoT /= m_firstAnalysisTriangle;
    squareMoT /= m_firstAnalysisTriangle;
    speedMoT /= m_firstAnalysisTriangle;
    timeMoA /= (m_triangles.size() - m_firstAnalysisTriangle);
    squareMoA /= (m_triangles.size() - m_firstAnalysisTriangle);
    speedMoA /= (m_triangles.size() - m_firstAnalysisTriangle);

    for (int i = 0; i < m_triangles.size(); ++i)
    {
        if (i < m_firstAnalysisTriangle)
        {
            m_valuesTraining.timeQ += pow(m_triangles.at(i).time() - timeMoT, 2) / m_firstAnalysisTriangle;
            m_valuesTraining.squareQ += pow(m_triangles.at(i).square() - squareMoT, 2) / m_firstAnalysisTriangle;
            m_valuesTraining.speedQ += pow(m_triangles.at(i).speed() - speedMoT, 2) / m_firstAnalysisTriangle;
        }
        else
        {
            m_valuesAnalysis.timeQ += pow(m_triangles.at(i).time() - timeMoA, 2) / (m_triangles.size() - m_firstAnalysisTriangle);
            m_valuesAnalysis.squareQ += pow(m_triangles.at(i).square() - squareMoA, 2) / (m_triangles.size() - m_firstAnalysisTriangle);
            m_valuesAnalysis.speedQ += pow(m_triangles.at(i).speed() - speedMoA, 2) / (m_triangles.size() - m_firstAnalysisTriangle);
        }
    }
    m_valuesTraining.timeQ = sqrt(m_valuesTraining.timeQ);
    m_valuesTraining.squareQ = sqrt(m_valuesTraining.squareQ);
    m_valuesTraining.speedQ = sqrt(m_valuesTraining.speedQ);
    m_valuesAnalysis.timeQ = sqrt(m_valuesAnalysis.timeQ);
    m_valuesAnalysis.squareQ = sqrt(m_valuesAnalysis.squareQ);
    m_valuesAnalysis.speedQ = sqrt(m_valuesAnalysis.speedQ);
}

void TriangleFactors::computePosDeviations(const int begin, const int end,
                                           TriangleFactorsDefines::TranglePosDeviation &upDev,
                                           TriangleFactorsDefines::TranglePosDeviation &rtDev,
                                           TriangleFactorsDefines::TranglePosDeviation &lfDev,
                                           TriangleFactorsDefines::TranglePosDeviation &midDev)
{
    //! Системная ошибка - МО
    for (int i = begin; i <= end; ++i)
    {
        upDev.systX += fabs(m_triangles.at(i).topCorner().x() - m_resData->topCorner().x());
        upDev.systY += fabs(m_triangles.at(i).topCorner().y() - m_resData->topCorner().y());
        rtDev.systX += fabs(m_triangles.at(i).rightDownCorner().x() - m_resData->rightDownCorner().x());
        rtDev.systY += fabs(m_triangles.at(i).rightDownCorner().y() - m_resData->rightDownCorner().y());
        lfDev.systX += fabs(m_triangles.at(i).leftDownCorner().x() - m_resData->leftDownCorner().x());
        lfDev.systY += fabs(m_triangles.at(i).leftDownCorner().y() - m_resData->leftDownCorner().y());
        QPointF center = TriangleDefines::Triangle::center(QPointF(m_resData->topCorner().x(), m_resData->topCorner().y()),
                                                           QPointF(m_resData->rightDownCorner().x(), m_resData->rightDownCorner().y()),
                                                           QPointF(m_resData->leftDownCorner().x(), m_resData->leftDownCorner().y()));
        midDev.systX += fabs(m_triangles.at(i).mx() - center.x());
        midDev.systY += fabs(m_triangles.at(i).my() - center.y());
    }
    upDev.systX /= (end - begin + 1);
    upDev.systY /= (end - begin + 1);
    rtDev.systX /= (end - begin + 1);
    rtDev.systY /= (end - begin + 1);
    lfDev.systX /= (end - begin + 1);
    lfDev.systY /= (end - begin + 1);
    midDev.systX /= (end - begin + 1);
    midDev.systY /= (end - begin + 1);

    //! Случайная ошибка - СКО
    for (int i = begin; i <= end; ++i)
    {
        upDev.randX += pow(fabs(fabs(m_triangles.at(i).topCorner().x() - m_resData->topCorner().x()) - upDev.systX), 2);
        upDev.randY += pow(fabs(fabs(m_triangles.at(i).topCorner().y() - m_resData->topCorner().y()) - upDev.systY), 2);
        rtDev.randX += pow(fabs(fabs(m_triangles.at(i).rightDownCorner().x() - m_resData->rightDownCorner().x()) - rtDev.systX), 2);
        rtDev.randY += pow(fabs(fabs(m_triangles.at(i).rightDownCorner().y() - m_resData->rightDownCorner().y()) - rtDev.systY), 2);
        lfDev.randX += pow(fabs(fabs(m_triangles.at(i).leftDownCorner().x() - m_resData->leftDownCorner().x()) - lfDev.systX), 2);
        lfDev.randY += pow(fabs(fabs(m_triangles.at(i).leftDownCorner().y() - m_resData->leftDownCorner().y()) - lfDev.systY), 2);
        QPointF center = TriangleDefines::Triangle::center(QPointF(m_resData->topCorner().x(), m_resData->topCorner().y()),
                                                           QPointF(m_resData->rightDownCorner().x(), m_resData->rightDownCorner().y()),
                                                           QPointF(m_resData->leftDownCorner().x(), m_resData->leftDownCorner().y()));
        midDev.randX += pow(fabs(fabs(m_triangles.at(i).mx() - center.x()) - midDev.systX), 2);
        midDev.randY += pow(fabs(fabs
                                 (m_triangles.at(i).my() - center.y()) - midDev.systY), 2);
    }
    upDev.randX = sqrt(upDev.randX / (end - begin + 1));
    upDev.randY = sqrt(upDev.randY / (end - begin + 1));
    rtDev.randX = sqrt(rtDev.randX / (end - begin + 1));
    rtDev.randY = sqrt(rtDev.randY / (end - begin + 1));
    lfDev.randX = sqrt(lfDev.randX / (end - begin + 1));
    lfDev.randY = sqrt(lfDev.randY / (end - begin + 1));
    midDev.randX = sqrt(midDev.randX / (end - begin + 1));
    midDev.randY = sqrt(midDev.randY / (end - begin + 1));
}

QPointF TriangleFactors::getCornerTargetCoord(BaseDefines::TriangleCorner code)
{
    if (code == BaseDefines::tcTop)
        return m_resData->topCorner();
    else
    if (code == BaseDefines::tcLeftDown)
        return m_resData->leftDownCorner();
    else
    if (code == BaseDefines::tcRightDown)
        return m_resData->rightDownCorner();
    return QPointF(0, 0);
}

QList<QVector<double>> TriangleFactors::fillTransitionsBuffers()
{
    //! Первая точка цели
    BaseDefines::TriangleCorner n (BaseDefines::tcNone);
    if (m_resData->direction() == BaseDefines::dmClockwise)
        n = BaseDefines::tcRightDown;
    else
    if (m_resData->direction() == BaseDefines::dmCounterClockwise)
        n = BaseDefines::tcLeftDown;

    //! Создание буферов
    QVector<double> bufR;
    QList<QVector<double>> retval;
    //! По сигналу от начала первого треугольника до окончания этапа тренинга
    for (int i = m_triangleSections.at(0).begin; i < m_resData->trainingLength(); ++i)
    {
        //! Между точками смены этапа
        if (bufR.size() < m_resData->timeOffsetMarker() * m_resData->freq())
            bufR << sqrt(pow(m_x.at(i) - getCornerTargetCoord(n).x(), 2) +
                         pow(m_y.at(i) - getCornerTargetCoord(n).y(), 2));
        else
        {
            //! Точка смены этапа
            retval << bufR;  //! Буфер - в список
            bufR.clear();    //! Очистить буфер

            //! Следующая точка
            if (m_resData->direction() == BaseDefines::dmClockwise)
            {
                if (n > BaseDefines::tcTop)
                    n = static_cast<BaseDefines::TriangleCorner>(static_cast<int>(n) - 1);
                else
                    n = BaseDefines::tcRightDown;
            }
            else
            if (m_resData->direction() == BaseDefines::dmCounterClockwise)
            {
                if (n < BaseDefines::tcRightDown)
                    n = static_cast<BaseDefines::TriangleCorner>(static_cast<int>(n) + 1);
                else
                    n = BaseDefines::tcTop;
            }
        }
    }
    return retval;
}

void TriangleFactors::computeLatentMoving()
{
    //! Создание буферов, содержащих расстояния от маркера до цели
    auto buffers = fillTransitionsBuffers();

    double time = 0;
    int n = 0;
    foreach (auto buf, buffers)
    {
        double min = INT_MAX;
        double max = -INT_MAX;
        int iMax = -1;
        for (int i = 0; i < buf.size(); ++i)
        {
            if (buf[i] < min)
                min = buf[i];
            if (buf[i] > max)
            {
                max = buf[i];
                iMax = i;
            }
        }

        if (iMax > -1)
        {
            for (int i = iMax; i < buf.size(); ++i)
            {
                if (buf[i] < max - (max - min) * 0.2)
                {
                    time += static_cast<double>(i) / static_cast<double>(m_resData->freq());
                    ++n;
                    break;
                }
            }
        }
    }

    if (n > 0)
        time /= n;

    m_latentMoving = time;
}

bool TriangleFactors::getDistPointToLine(const int idx, int &iTrngl, double &dist)
{
    double x1 {0};
    double y1 {0};
    double x2 {0};
    double y2 {0};
    dist = 0;
    bool retval = false;

    if (iTrngl < m_triangleSections.size())
    {
        if ((idx >= m_triangleSections.at(iTrngl).begin) && (idx <= m_triangleSections.at(iTrngl).end))
        {
            //! Сторона треугольника
            double tm = static_cast<double>(idx - m_triangleSections.at(iTrngl).begin) /
                        static_cast<double>(m_triangleSections.at(iTrngl).end - m_triangleSections.at(iTrngl).begin);
            //! Точки для прямой
            if ((tm >= 0) and (tm < 0.333333))
            {
                x1 = m_triangles.at(iTrngl).topCorner().x();
                y1 = m_triangles.at(iTrngl).topCorner().y();
                if (m_resData->direction() == BaseDefines::dmClockwise)
                {
                    x2 = m_triangles.at(iTrngl).rightDownCorner().x();
                    y2 = m_triangles.at(iTrngl).rightDownCorner().y();
                }
                else
                {
                    x2 = m_triangles.at(iTrngl).leftDownCorner().x();
                    y2 = m_triangles.at(iTrngl).leftDownCorner().y();
                }
            }
            else
            if ((tm >= 0.333333) && (tm < 0.666666))
            {
                if (m_resData->direction() == BaseDefines::dmClockwise)
                {
                    x1 = m_triangles.at(iTrngl).rightDownCorner().x();
                    y1 = m_triangles.at(iTrngl).rightDownCorner().y();
                    x2 = m_triangles.at(iTrngl).leftDownCorner().x();
                    y2 = m_triangles.at(iTrngl).leftDownCorner().y();
                }
                else
                {
                    x1 = m_triangles.at(iTrngl).leftDownCorner().x();
                    y1 = m_triangles.at(iTrngl).leftDownCorner().y();
                    x2 = m_triangles.at(iTrngl).rightDownCorner().x();
                    y2 = m_triangles.at(iTrngl).rightDownCorner().y();
                }
            }
            else
            {
                if (m_resData->direction() == BaseDefines::dmClockwise)
                {
                    x1 = m_triangles.at(iTrngl).leftDownCorner().x();
                    y1 = m_triangles.at(iTrngl).leftDownCorner().y();
                }
                else
                {
                    x1 = m_triangles.at(iTrngl).rightDownCorner().x();
                    y1 = m_triangles.at(iTrngl).rightDownCorner().y();
                }
                x2 = m_triangles.at(iTrngl).topCorner().x();
                y2 = m_triangles.at(iTrngl).topCorner().y();
            }

            //! Расчет расстояния
            double a = y1 - y2;
            double b = x2 - x1;
            double c = x1 * y2 - x2 * y1;
            dist = pow(a, 2) + pow(b, 2);
            if (dist > 0)
              dist = fabs(a * m_x[idx] + b * m_y[idx] + c ) / sqrt(dist);
            retval = true;
        }
        else
        {
            if (idx >m_triangleSections.at(iTrngl).end)
                ++iTrngl;
        }
    }
    return retval;
}

void TriangleFactors::computeCorrectionsFactors()
{
    double timeDACTst {0}, amplDACTst {0};
    double timeDACAnl {0}, amplDACAnl {0};
    int extrCntDACTst {0}, extrCntDACAnl {0};
    int dirDAC {0};
    double extrDAC = {-1};
    int extrIDAC {0};
    int zoneLoTst {0}, zoneHiTst {0};
    int zoneLoAnl {0}, zoneHiAnl {0};
    double aOld {0}, da {0}, oDAC {0};
    double dist {0};            ///< Расстояние от точки до линии

    int iTst {0}, iAnl {0};

    for (int i = 0; i < qMin(m_x.size(), m_y.size()); ++i)
    {

        //! Перевод координат в полярную систему
        double x = m_x[i];
        double y = m_y[i];
        double r {0};
        double a {0};
        BaseUtils::convertDecartToPolar(x, y, r, a);

        if (i > 0)
        {
            if (aOld > 3 * M_PI_2 && a < M_PI_2)
              da = (2 * M_PI - aOld ) + a;
            else
            if (aOld < M_PI_2 && a > 3 * M_PI_2)
              da = (2 * M_PI - a) + aOld;
            else
              da = a - aOld;

            //! Экстремум по динамике опережения
            if (((da < oDAC) && (dirDAC == BaseDefines::PositiveValue)) ||
                ((da > oDAC) && (dirDAC == BaseDefines::NegativeValue)))
            {
                if ( extrDAC > -1 )
                {
                    double timeI = static_cast<double>(i - extrIDAC) / static_cast<double>(m_resData->freq());
                    double amplI = fabs(da - extrDAC);

                    //! Этап обучения
                    if (i <= m_resData->trainingLength())
                    {
                        timeDACTst += timeI;
                        amplDACTst += amplI;

                        //! Определение линии, по которой мы должны двигаться
                        getDistPointToLine(i, iTst, dist);

                        //! Расчет кол-ва экстремумов по зонам
                        if (timeI >= BaseDefines::ZoneMotorLo && timeI <= BaseDefines::ZoneMotorHi)
                        {
                            ++zoneLoTst;
                            m_motorMidATst += amplI;
                            m_motorSumTTst += timeI;
                            m_motorErrTst += dist;
                        }
                        else
                        if (timeI >= BaseDefines::ZoneKognLo && timeI <= BaseDefines::ZoneKognHi)
                        {
                            ++zoneHiTst;
                            m_kognMidATst += amplI;
                            m_kognSumTTst += timeI;
                            m_kognErrTst += dist;
                        }

                        ++extrCntDACTst;
                    }
                    //! Этап анализа
                    else
                    {
                        //! Определение линии, по которой мы должны двигаться
                        getDistPointToLine(i, iAnl, dist);

                        timeDACAnl += timeI;
                        amplDACAnl += amplI;
                        //! Расчет кол-ва экстремумов по зонам
                        if (timeI >= BaseDefines::ZoneMotorLo && timeI <= BaseDefines::ZoneMotorHi)
                        {
                            ++zoneLoAnl;
                            m_motorMidAAnl += amplI;
                            m_motorSumTAnl += timeI;
                            m_motorErrAnl += dist;
                        }
                        else
                        if (timeI >= BaseDefines::ZoneKognLo && timeI <= BaseDefines::ZoneKognHi)
                        {
                          ++zoneHiAnl;
                          m_kognMidAAnl += amplI;
                          m_kognSumTAnl += timeI;
                          m_kognErrAnl += dist;
                        }

                        ++extrCntDACAnl;
                    }
                }

                extrIDAC = i;
                extrDAC = da;
            }
        }
        dirDAC = BaseUtils::sign(da - oDAC);
        oDAC = da;
        aOld = a;
    }

    m_korrCntKognTst  = zoneHiTst;
    m_korrCntMotorTst = zoneLoTst;
    m_korrCntKognAnl  = zoneHiAnl;
    m_korrCntMotorAnl = zoneLoAnl;

    if (extrCntDACTst > 0)
    {
        m_semiWavLenDACTst = timeDACTst / extrCntDACTst;
        m_semiWavAmplDACTst = amplDACTst / extrCntDACTst;

        m_motorPercTst = static_cast<double>(zoneLoTst) / static_cast<double>(extrCntDACTst) * 100;
        m_kognPercTst = static_cast<double>(zoneHiTst) / static_cast<double>(extrCntDACTst) * 100;
    }

    if (extrCntDACAnl > 0)
    {
        m_semiWavLenDACAnl = timeDACAnl / extrCntDACAnl;
        m_semiWavAmplDACAnl = amplDACAnl / extrCntDACAnl;

        m_motorPercAnl = static_cast<double>(zoneLoAnl) / static_cast<double>(extrCntDACAnl) * 100;
        m_kognPercAnl = static_cast<double>(zoneHiAnl) / static_cast<double>(extrCntDACAnl) * 100;
    }

    if (zoneLoTst > 0)
    {
        m_motorMidATst /= zoneLoTst;
        m_motorMidTTst = m_motorSumTTst / zoneLoTst;
        m_motorErrTst /= zoneLoTst;
    }
    if (zoneHiTst > 0)
    {
        m_kognMidATst /= zoneHiTst;
        m_kognMidTTst = m_kognSumTTst / zoneHiTst;
        m_kognErrTst /= zoneHiTst;
    }

    if (zoneLoAnl > 0)
    {
        m_motorMidAAnl /= zoneLoAnl;
        m_motorMidTAnl = m_motorSumTAnl / zoneLoAnl;
        m_motorErrAnl /= zoneLoAnl;
    }
    if (zoneHiAnl > 0)
    {
        m_kognMidAAnl /= zoneHiAnl;
        m_kognMidTAnl = m_kognSumTAnl / zoneHiAnl;
        m_kognErrAnl /= zoneHiAnl;
    }

    m_motorPwrTst = (m_motorMidTTst - BaseDefines::ZoneMotorLo) * m_motorMidATst * zoneLoTst;
    m_kognPwrTst = (m_kognMidTTst - BaseDefines::ZoneKognLo) * m_kognMidATst  * zoneHiTst;

    m_motorPwrAnl = (m_motorMidTAnl - BaseDefines::ZoneMotorLo) * m_motorMidAAnl * zoneLoAnl;
    m_kognPwrAnl = (m_kognMidTAnl - BaseDefines::ZoneKognLo) * m_kognMidAAnl  * zoneHiAnl;
}

void TriangleFactors::getTriangleData()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanTriangleResult, baData))
        m_resData = new TriangleResultData(baData);
}

void TriangleFactors::addFactors()
{
    addFactorPair(TriangleFactorsDefines::Training::TimeUid, m_triangleAverageTraining.time(),
                  TriangleFactorsDefines::Analysis::TimeUid, m_triangleAverageAnalysis.time());
    addFactorPair(TriangleFactorsDefines::Training::TimeQUid, m_valuesTraining.timeQ,
                  TriangleFactorsDefines::Analysis::TimeQUid, m_valuesAnalysis.timeQ);
    addFactorPair(TriangleFactorsDefines::Training::SquareUid, m_triangleAverageTraining.square(),
                  TriangleFactorsDefines::Analysis::SquareUid, m_triangleAverageAnalysis.square());
    addFactorPair(TriangleFactorsDefines::Training::SquareQUid, m_valuesTraining.squareQ,
                  TriangleFactorsDefines::Analysis::SquareQUid, m_valuesAnalysis.squareQ);
    addFactorPair(TriangleFactorsDefines::Training::SpeedUid, m_triangleAverageTraining.speed(),
                  TriangleFactorsDefines::Analysis::SpeedUid, m_triangleAverageAnalysis.speed());
    addFactorPair(TriangleFactorsDefines::Training::SpeedQUid, m_valuesTraining.speedQ,
                  TriangleFactorsDefines::Analysis::SpeedQUid, m_valuesAnalysis.speedQ);
    addFactorPair(TriangleFactorsDefines::Training::MXUid, m_triangleAverageTraining.mx(),
                  TriangleFactorsDefines::Analysis::MXUid, m_triangleAverageAnalysis.mx());
    addFactorPair(TriangleFactorsDefines::Training::MYUid, m_triangleAverageTraining.my(),
                  TriangleFactorsDefines::Analysis::MYUid, m_triangleAverageAnalysis.my());
    addFactorPair(TriangleFactorsDefines::Training::AngleUid, m_triangleAverageTraining.angle(),
                  TriangleFactorsDefines::Analysis::AngleUid, m_triangleAverageAnalysis.angle());

    addFactorPair(TriangleFactorsDefines::Training::UpErrSysXUid, m_upDevTest.systX,
                  TriangleFactorsDefines::Analysis::UpErrSysXUid, m_upDevAnal.systX);
    addFactorPair(TriangleFactorsDefines::Training::UpErrSysYUid, m_upDevTest.systY,
                  TriangleFactorsDefines::Analysis::UpErrSysYUid, m_upDevAnal.systY);
    addFactorPair(TriangleFactorsDefines::Training::UpErrRndXUid, m_upDevTest.randX,
                  TriangleFactorsDefines::Analysis::UpErrRndXUid, m_upDevAnal.randX);
    addFactorPair(TriangleFactorsDefines::Training::UpErrRndYUid, m_upDevTest.randY,
                  TriangleFactorsDefines::Analysis::UpErrRndYUid, m_upDevAnal.randY);
    addFactorPair(TriangleFactorsDefines::Training::RightErrSysXUid, m_rtDevTest.systX,
                  TriangleFactorsDefines::Analysis::RightErrSysXUid, m_rtDevAnal.systX);
    addFactorPair(TriangleFactorsDefines::Training::RightErrSysYUid, m_rtDevTest.systY,
                  TriangleFactorsDefines::Analysis::RightErrSysYUid, m_rtDevAnal.systY);
    addFactorPair(TriangleFactorsDefines::Training::RightErrRndXUid, m_rtDevTest.randX,
                  TriangleFactorsDefines::Analysis::RightErrRndXUid, m_rtDevAnal.randX);
    addFactorPair(TriangleFactorsDefines::Training::RightErrRndYUid, m_rtDevTest.randY,
                  TriangleFactorsDefines::Analysis::RightErrRndYUid, m_rtDevAnal.randY);
    addFactorPair(TriangleFactorsDefines::Training::LeftErrSysXUid, m_lfDevTest.systX,
                  TriangleFactorsDefines::Analysis::LeftErrSysXUid, m_lfDevAnal.systX);
    addFactorPair(TriangleFactorsDefines::Training::LeftErrSysYUid, m_lfDevTest.systY,
                  TriangleFactorsDefines::Analysis::LeftErrSysYUid, m_lfDevAnal.systY);
    addFactorPair(TriangleFactorsDefines::Training::LeftErrRndXUid, m_lfDevTest.randX,
                  TriangleFactorsDefines::Analysis::LeftErrRndXUid, m_lfDevAnal.randX);
    addFactorPair(TriangleFactorsDefines::Training::LeftErrRndYUid, m_lfDevTest.randY,
                  TriangleFactorsDefines::Analysis::LeftErrRndYUid, m_lfDevAnal.randY);
    addFactorPair(TriangleFactorsDefines::Training::MidErrSysXUid, m_midDevTest.systX,
                  TriangleFactorsDefines::Analysis::MidErrSysXUid, m_midDevAnal.systX);
    addFactorPair(TriangleFactorsDefines::Training::MidErrSysYUid, m_midDevTest.systY,
                  TriangleFactorsDefines::Analysis::MidErrSysYUid, m_midDevAnal.systY);
    addFactorPair(TriangleFactorsDefines::Training::MidErrRndXUid, m_midDevTest.randX,
                  TriangleFactorsDefines::Analysis::MidErrRndXUid, m_midDevAnal.randX);
    addFactorPair(TriangleFactorsDefines::Training::MidErrRndYUid, m_midDevTest.randY,
                  TriangleFactorsDefines::Analysis::MidErrRndYUid, m_midDevAnal.randY);

    addFactorPair(TriangleFactorsDefines::Training::SemiWavLenDACUid, m_semiWavLenDACTst,
                  TriangleFactorsDefines::Analysis::SemiWavLenDACUid, m_semiWavLenDACAnl);
    addFactorPair(TriangleFactorsDefines::Training::SemiWavAmplDACUid, m_semiWavAmplDACTst,
                  TriangleFactorsDefines::Analysis::SemiWavAmplDACUid, m_semiWavAmplDACAnl);
    addFactorPair(TriangleFactorsDefines::Training::KorrCntKognUid, m_korrCntKognTst,
                  TriangleFactorsDefines::Analysis::KorrCntKognUid, m_korrCntKognAnl);
    addFactorPair(TriangleFactorsDefines::Training::KorrCntMotorUid, m_korrCntMotorTst,
                  TriangleFactorsDefines::Analysis::KorrCntMotorUid, m_korrCntMotorAnl);

    addFactorPair(TriangleFactorsDefines::Training::MotorPercUid, m_motorPercTst,
                  TriangleFactorsDefines::Analysis::MotorPercUid, m_motorPercAnl);
    addFactorPair(TriangleFactorsDefines::Training::KognPercUid, m_kognPercTst,
                  TriangleFactorsDefines::Analysis::KognPercUid, m_kognPercAnl);
    addFactorPair(TriangleFactorsDefines::Training::MotorErrUid, m_motorErrTst,
                  TriangleFactorsDefines::Analysis::MotorErrUid, m_motorErrAnl);
    addFactorPair(TriangleFactorsDefines::Training::KognErrUid, m_kognErrTst,
                  TriangleFactorsDefines::Analysis::KognErrUid, m_kognErrAnl);
    addFactorPair(TriangleFactorsDefines::Training::MotorMidAUid, m_motorMidATst,
                  TriangleFactorsDefines::Analysis::MotorMidAUid, m_motorMidAAnl);
    addFactorPair(TriangleFactorsDefines::Training::KognMidAUid, m_kognMidATst,
                  TriangleFactorsDefines::Analysis::KognMidAUid, m_kognMidAAnl);
    addFactorPair(TriangleFactorsDefines::Training::MotorSumTUid, m_motorSumTTst,
                  TriangleFactorsDefines::Analysis::MotorSumTUid, m_motorSumTAnl);
    addFactorPair(TriangleFactorsDefines::Training::KognSumTUid, m_kognSumTTst,
                  TriangleFactorsDefines::Analysis::KognSumTUid, m_kognSumTAnl);
    addFactorPair(TriangleFactorsDefines::Training::MotorMidTUid, m_motorMidTTst,
                  TriangleFactorsDefines::Analysis::MotorMidTUid, m_motorMidTAnl);
    addFactorPair(TriangleFactorsDefines::Training::KognMidTUid, m_kognMidTTst,
                  TriangleFactorsDefines::Analysis::KognMidTUid, m_kognMidTAnl);
    addFactorPair(TriangleFactorsDefines::Training::MotorPwrUid, m_motorPwrTst,
                  TriangleFactorsDefines::Analysis::MotorPwrUid, m_motorPwrAnl);
    addFactorPair(TriangleFactorsDefines::Training::KognPwrUid, m_kognPwrTst,
                  TriangleFactorsDefines::Analysis::KognPwrUid, m_kognPwrAnl);

    addFactor(TriangleFactorsDefines::LatentMovingUid, m_latentMoving);
}

void TriangleFactors::addFactorPair(const QString &uidT, const double valueT, const QString &uidA, const double valueA)
{
    m_factorsOfStages.append(BaseDefines::FctTblPair(uidT, uidA));
    addFactor(uidT, valueT);
    addFactor(uidA, valueA);
}

