#ifndef TRIANGLEFACTORS_H
#define TRIANGLEFACTORS_H

#include <QObject>
#include <QVector>

#include "basedefines.h"
#include "multifactor.h"
#include "triangledefines.h"

namespace TriangleFactorsDefines
{
static const QString GroupUid = "{262C7BC3-2EFA-40CA-8D08-D770E76F57FC}";

namespace Training {
static const QString TimeUid = "{E16E7852-9E22-4385-8A3A-B949C27505BB}";
static const QString TimeQUid = "{1B36487D-F314-46F3-8199-CE889F388ED5}";
static const QString SquareUid = "{6006B71E-D7E5-4FDF-8B73-77B8B966F330}";
static const QString SquareQUid = "{B231419B-E3E2-4049-BFFB-3D8EFB2E379B}";
static const QString SpeedUid = "{07B9825D-3D8C-4079-B859-1A0fCA288777}";
static const QString SpeedQUid = "{368CD3FB-7162-43E4-B35B-7737942D40A9}";
static const QString MXUid = "{8B36C68A-7CF3-4C1D-AC5B-7E86595020F5}";
static const QString MYUid = "{BE74D224-2C8C-42B2-8AF0-5C0F24D79B8E}";

static const QString UpErrSysXUid = "{B5BA8F9B-AE33-482C-A10E-DF7C18D3B8F3}";
static const QString UpErrSysYUid = "{82544872-1E52-4EB0-A426-32C864378173}";
static const QString UpErrRndXUid = "{5D364B9D-4A06-4DD1-B83A-E6A6ECF8D099}";
static const QString UpErrRndYUid = "{DF827196-4D5A-4268-9D98-A924CD3740B5}";
static const QString RightErrSysXUid = "{F5B31BAC-451E-4DAC-9D16-A34BA1722932}";
static const QString RightErrSysYUid = "{77EBEE63-5C3D-404C-AF33-683C4D1E0EF6}";
static const QString RightErrRndXUid = "{5BD19A9C-4D16-4B65-97A5-E61AEC2B6936}";
static const QString RightErrRndYUid = "{3EEAA965-DDB1-4137-9471-4B7F5691FD37}";
static const QString LeftErrSysXUid = "{D26744AE-A408-48A4-A536-9F72C1F963EB}";
static const QString LeftErrSysYUid = "{C27B7BBD-CA91-46E8-9202-592898B02EA0}";
static const QString LeftErrRndXUid = "{67C59527-333A-4210-8130-F0C32853EAD2}";
static const QString LeftErrRndYUid = "{A59B2584-55F8-4C10-99D8-F339B9DEB0B0}";
static const QString MidErrSysXUid = "{D688E596-D983-414C-9B35-01B9369B0B24}";
static const QString MidErrSysYUid = "{BB898E6D-FA52-4457-8A22-AF1FDCAB5866}";
static const QString MidErrRndXUid = "{1160F91F-7EF3-46E8-8016-D56677A2CD6D}";
static const QString MidErrRndYUid = "{F489741C-0B82-48B0-8203-F8E4A600BEC3}";

static const QString AngleUid = "{05ABA283-C887-4882-A6CB-CDFA9501C2C5}";

static const QString SemiWavLenDACUid = "{86086F6F-0F9B-4331-9BE0-EA59FDDFEE6D}";  // Средняя длительность коррекций (обучение)
static const QString SemiWavAmplDACUid = "{E8317E29-EE1A-4E49-BB72-309E2C98EE53}"; // Средняя амплитуда коррекций (обучение)
static const QString KorrCntKognUid = "{A467AB9B-EE8A-4FD4-8820-5D1D3432DC0F}";    // Кол-во когнитивных коррекций (обучение)
static const QString KorrCntMotorUid = "{9217F70A-AB7E-4009-9917-AB8B6C139247}";   // Кол-во моторных коррекций (обучение)

static const QString MotorPercUid = "{CF64A08F-50AB-4B72-BEF6-2BA4D25B8FA9}";    // Процент моторных коррекций (обучение)
static const QString KognPercUid = "{FFFD77D2-B48C-4FE4-BE8A-805037C7A14E}";    // Процент когнитивных коррекций (обучение)
static const QString MotorErrUid = "{4045F765-820E-495D-9A3F-A8F94B6457AD}";     // Средняя ошибка моторных коррекций (обучение)
static const QString KognErrUid = "{C303AEDE-4308-4210-B136-C5E10493B42E}";     // Средняя ошибка когнитивных коррекций (обучение)
static const QString MotorMidAUid = "{019ACED9-4E76-4FE6-BBC5-EE97AC11EE1C}";    // Средняя амплитуда моторных коорекций (обучение)
static const QString KognMidAUid = "{7016ACEB-1136-4D22-978C-AE7A1CD8C32B}";    // Средняя амплитуда когнитивных коорекций (обучение)
static const QString MotorSumTUid = "{352B9EF5-A080-4DDF-A3B1-88524B024592}";    // Суммарная длительность моторных коррекций (обучение)
static const QString KognSumTUid = "{6A818A23-7C04-4249-9FF0-8801CA79FCAB}";    // Суммарная длительность когнитивных коррекций (обучение)
static const QString MotorMidTUid = "{DF955DEA-8F15-4747-816F-679FADBDDE6D}";    // Средняя длительность моторных коррекций (обучение)
static const QString KognMidTUid = "{DD3EBFDD-61D5-4239-8419-9367B5D0503B}";    // Средняя длительность когнитивных коррекций (обучение)
static const QString MotorPwrUid = "{EAAB7A8E-9186-47D9-B8AA-395ABDA08784}";     // Мощность моторных коррекций (обучение)
static const QString KognPwrUid = "{489354B5-2362-4841-8DF1-B7B558D8AB35}";     // Мощность когнитивных коррекций (обучение)

}


namespace Analysis {
static const QString TimeUid = "{0EB7D41E-3708-4370-8BFD-B3008DDCD63B}";
static const QString TimeQUid = "{6FA79832-0FAF-4C1D-AFE6-A0EEBAD54674}";
static const QString SquareUid = "{2D3F41C5-E32B-493E-847E-41CD5B4340BB}";
static const QString SquareQUid = "{7E173708-3BC6-4828-9E50-73A68711E13D}";
static const QString SpeedUid = "{6367BA59-A934-45D3-8342-FFFB3D1445E2}";
static const QString SpeedQUid = "{5B049643-D08A-4D6E-B0A0-CCCA8E1DABFE}";
static const QString MXUid = "{A243B86F-9AA9-4FE8-9B23-657AE43BCDCF}";
static const QString MYUid = "{E0C30597-E75C-4136-A775-504BF02B5B9C}";

static const QString UpErrSysXUid = "{17CDCF87-A839-4BDE-9F37-032B4D20600C}";
static const QString UpErrSysYUid = "{D871C553-2A25-470A-B732-FFC6B90B6E66}";
static const QString UpErrRndXUid = "{7A99A923-9631-42AF-AB03-404152F7DEAB}";
static const QString UpErrRndYUid = "{962505B4-7B70-49D6-B602-3A4BBDE6472F}";
static const QString RightErrSysXUid = "{5443F24D-7762-4F37-A1C2-31033E3AF16D}";
static const QString RightErrSysYUid = "{9BE4E3C4-5549-463C-95F3-D0E507B7F515}";
static const QString RightErrRndXUid = "{7F95EEAA-D6DD-4D45-BBF2-8349E70A964E}";
static const QString RightErrRndYUid = "{CB933D57-3D60-4E2A-906E-6727320C21E0}";
static const QString LeftErrSysXUid = "{B70C3C41-12DB-4290-B11D-68D8C3F1EB15}";
static const QString LeftErrSysYUid = "{6AF26249-F86E-48EF-BCE1-83EC8E490557}";
static const QString LeftErrRndXUid = "{D89E44F8-3680-459D-A6CE-6A8A1A386FDF}";
static const QString LeftErrRndYUid = "{0B890317-2B07-40D6-BD6A-F136C148B892}";
static const QString MidErrSysXUid = "{08D3DB85-C776-4FE9-9B1E-70F87D69DBCF}";
static const QString MidErrSysYUid = "{0F00ED53-BBD6-42C9-99A0-8614B0DEC559}";
static const QString MidErrRndXUid = "{A5EA0128-73FB-4FFC-B098-E7C436C869F4}";
static const QString MidErrRndYUid = "{E2F87C06-F61B-4AF0-8090-D41F99476741}";

static const QString AngleUid = "{06C30D29-D4CA-4EEA-938B-E40E9461A428}";

static const QString SemiWavLenDACUid = "{2944DDF6-8295-4479-9C36-2F190F9B2717}";  // Средняя длительность коррекций (анализ)
static const QString SemiWavAmplDACUid = "{82EEE61E-BFE7-4AA4-B322-44C4D6748D68}"; // Средняя амплитуда коррекций (анализ)
static const QString KorrCntKognUid = "{EA891CBD-4201-4592-AF57-B5012CEF53CB}";    // Кол-во когнитивных коррекций (анализ)
static const QString KorrCntMotorUid = "{7781018F-50AF-482E-A6CD-2BCD3191FE9B}";   // Кол-во моторных коррекций (анализ)

static const QString MotorPercUid = "{5A9432E6-AD53-4E23-9438-94D599DD904D}";    // Процент моторных коррекций (анализ)
static const QString KognPercUid = "{1D5EE90E-399E-45F6-AAA9-6E33D0E909D1}";    // Процент когнитивных коррекций (анализ)
static const QString MotorErrUid = "{AE63E6F5-A692-41E6-90E9-376FEAECAF88}";     // Средняя ошибка моторных коррекций (анализ)
static const QString KognErrUid = "{1C38E0E0-3B56-490F-B0A1-B386A35171BC}";     // Средняя ошибка когнитивных коррекций (анализ)
static const QString MotorMidAUid = "{3FF6B9CF-6C73-4D58-82FC-DF7ED0839AD1}";    // Средняя амплитуда моторных коорекций (анализ)
static const QString KognMidAUid = "{6CAD1A2A-27DC-46DF-9A55-61B526A75391}";    // Средняя амплитуда когнитивных коорекций (анализ)
static const QString MotorSumTUid = "{E466FA2B-37C0-45F8-AB56-273F87479135}";    // Суммарная длительность моторных коррекций (анализ)
static const QString KognSumTUid = "{DD123FE2-F8CC-40BE-9306-C9903785B2B9}";    // Суммарная длительность когнитивных коррекций (анализ)
static const QString MotorMidTUid = "{FBA86CFA-C57B-40B8-BF0F-59836A95EC03}";    // Средняя длительность моторных коррекций (анализ)
static const QString KognMidTUid = "{5AF4603E-A2E9-4B34-A02A-8467C412A5F6}";    // Средняя длительность когнитивных коррекций (анализ)
static const QString MotorPwrUid = "{A8F2C3EC-D932-4B72-8635-6B6356E845F7}";     // Мощность моторных коррекций (анализ)
static const QString KognPwrUid = "{0C47DDF0-5B9B-4000-834C-93FAA3D5BFAE}";     // Мощность когнитивных коррекций (анализ)
}


static const QString LatentMovingUid = "{1750CF55-457E-4E1C-80C7-4EA5528DCB91}";

/*!
 * \brief Значения некоторых показателей FactorValues struct
 */
struct FactorValues
{
    double timeQ {0};    ///< Разброс длительности проходов
    double squareQ {0};  ///< Разброс площади треугольников
    double speedQ {0};   ///< Разброс скорости прохождения
};

/*!
 * \brief Структура параметров отклонения треугольника от шаблона TrangleTopDeviation struct
 */
struct TranglePosDeviation
{
  double systX {0};
  double systY {0};
  double randX {0};
  double randY {0};
};


}

class TriangleResultData;

class TriangleFactors : public ProbeMultifactor
{
public:
    explicit TriangleFactors(const QString &testUid,
                             const QString &probeUid,
                             QObject *parent = nullptr);

    /*!
     * \brief Возвращает uid группы показателей
     */
    QString uid() const override;

    /*!
     * \brief Возвращает название группы показателей
     */
    QString name() const override;

    /*!
     * \brief Возвращает название группы показателей для использования, как константу
     */
    static QString nameAsConst();

    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     * uid теста задается в конструкторе
     */
    bool isValid()  const override;
    /*!
     * \brief Возвращает true, если показатели можно рассчитать для теста с заданным uid
     * Аналог для вызова без создания экземпляра объекта
     */
    static bool isValid(const QString &testUid,
                        const QString &probeUid);

    /*!
     * \brief Рассчитывает показатели для теста с заданным uid
     * uid теста задается в конструкторе
     */
    void calculate() override;

    /*!
     * \brief Регистрирует факторсгруппу и показатели
     */
    static void registerFactors();

    int freq() const;
    int diap() const;

    /*!
     * \brief Длительность этапа обучения в пакетах
     */
    int trainingLength() const;

    /*!
     * \brief Возвращает индекс первого треугольника на этапе анализа
     */
    int firstAnalysisTriangle() const {return  m_firstAnalysisTriangle;}

    /*!
     * \brief Длительность сигнала в пакетах
     */
    int signalLength() const;

    /*!
     * \brief Возвращает оригигальный треугольник
     */
    TriangleDefines::Triangle triangleOriginal();

    /*!
     * \brief Возвращает усредненный треугольник для этапа обучения
     */
    TriangleDefines::Triangle triangleTraining();

    /*!
     * \brief Возвращает усредненный треугольник для этапа анализа
     */
    TriangleDefines::Triangle triangleAnalysis();

    /*!
     * \brief Возвращает кол-во треугольников
     */
    int trianglesCount() const;

    /*!
     * \brief Возвращает границы треугольника по номеру
     * \param idx - номер треугольника
     */
    BaseDefines::Section triangleSection(const int idx) const;

    /*!
     * \brief Возвращает треугольник по номеру
     * \param idx - номер треугольника
     */
    TriangleDefines::Triangle triangle(const int idx) const;

    /*!
     * \brief Возвращает количество показателей, распределенных по этапам в таблице
     */
    int getFactorsOfStagesCount();

    /*!
     * \brief Возвращает uid показателей, распределенных по этапам в таблице
     * \param idx - индекс показателя
     * \param uidT - uid показателя для этапа обучения
     * \param uidA - uid показателя для этапа анализа
     * \return true, если удачно
     */
    bool getFactorOfStages(const int idx, QString &uidT, QString &uidA);

private:
    /*!
     * \brief Чтение сигналов, фильтрация
     */
    void readSignal();

    /*!
     * \brief Расчет списка границ треугольников
     */
    void computeTrianglesBounds();

    /*!
     * \brief Расчет треугольников
     */
    void computeTriangles();   

    /*!
     * \brief Рассчитывает точку вершины треугольника
     * \param stab - массив точек треугольника (стабилограмма)
     * \return точку вершины
     */
    QPointF computeCorner(QVector<QPointF> &stab);

    /*!
     * \brief Расчет значений СКО
     */
    void computeSKOValues();

    /*!
     * \brief computePosDeviations
     * \param begin, end - номера первого и последнего треугольников
     * \param upDev - параметры верхнего угла
     * \param rtDev - параметры правого угла
     * \param lfDev - параметры левого угла
     * \param midDev - параметры центра треугольника
     */
    void computePosDeviations(const int begin, const int end,
                              TriangleFactorsDefines::TranglePosDeviation &upDev,
                              TriangleFactorsDefines::TranglePosDeviation &rtDev,
                              TriangleFactorsDefines::TranglePosDeviation &lfDev,
                              TriangleFactorsDefines::TranglePosDeviation &midDev);

    QPointF getCornerTargetCoord(BaseDefines::TriangleCorner code);

    //! Создание буферов, содержащих расстояния от маркера до цели
    //! Используется при расчете показателя "Время начала движения после появления сигнала"
    QList<QVector<double>> fillTransitionsBuffers();

    /*!
     * \brief Расчет показателя "Время начала движения после появления сигнала"
     */
    void computeLatentMoving();

    /*!
     * \brief Определение линии, по которой мы должны двигаться
     * \param idx - индекс отсчета
     * \param iTrngl - номер треугольника
     * \param dist - рассчитанное расстояние
     * \return true при удаче
     */
    bool getDistPointToLine ( const int idx, int &iTrngl, double &dist);

    /*!
     * \brief Расчет показателей коррекций
     */
    void computeCorrectionsFactors();

    /*!
     * \brief Читает данные пробы
     */
    void getTriangleData();

    void addFactors();

    /*!
     * \brief Добавляет пару показателей для этапов
     * \param uidT - uid показателя для этапа тренинга
     * \param valueT - значение показателя для этапа тренинга
     * \param uidA - uid показателя для этапа анализа
     * \param valueA - значение показателя для этапа анализа
     */
    void addFactorPair(const QString &uidT, const double valueT, const QString &uidA, const double valueA);

    TriangleResultData* m_resData;

    QList<BaseDefines::Section> m_triangleSections;   ///< Список участков сигнала треугольников
    QList<TriangleDefines::Triangle> m_triangles;   ///< Список треугольников
    TriangleDefines::Triangle m_triangleAverageTraining; ///< Усредненный треугольник на этапе обучения
    TriangleDefines::Triangle m_triangleAverageAnalysis; ///< Усредненный треугольник на этапе анализа

    QVector<double> m_x, m_y;                ///< Исходная стабилограмма
    QVector<double> m_xf, m_yf;              ///< Фильтраванная стабилограмма
    int m_freq {50};
    int m_firstAnalysisTriangle {0};         ///< Первый треугольник на этапе анализа

    TriangleFactorsDefines::FactorValues m_valuesTraining;
    TriangleFactorsDefines::FactorValues m_valuesAnalysis;

    ///< Параметры отклонения треугольника от шаблона
    TriangleFactorsDefines::TranglePosDeviation m_upDevTest, m_rtDevTest, m_lfDevTest, m_midDevTest;
    TriangleFactorsDefines::TranglePosDeviation m_upDevAnal, m_rtDevAnal, m_lfDevAnal, m_midDevAnal;

    ///< uid-ы показателей для этапов
    QList<BaseDefines::FctTblPair> m_factorsOfStages;

    ///< Время начала движения после появления сигнала
    double m_latentMoving {0};

    ///< Средние длительность и амплитуда между локальными экстремумами
    double m_semiWavLenDACTst {0}, m_semiWavAmplDACTst {0};
    double m_semiWavLenDACAnl {0}, m_semiWavAmplDACAnl {0};
    double m_korrCntKognTst {0}, m_korrCntMotorTst {0};
    double m_korrCntKognAnl {0}, m_korrCntMotorAnl {0};
    ///< Распределение локальных экстремумов по зонам
    double m_motorPercTst {0}, m_kognPercTst {0};
    double m_motorErrTst {0}, m_kognErrTst {0};
    double m_motorMidATst {0}, m_kognMidATst {0};
    double m_motorSumTTst {0}, m_kognSumTTst {0};
    double m_motorMidTTst {0}, m_kognMidTTst {0};
    double m_motorPwrTst {0}, m_kognPwrTst {0};
    double m_motorPercAnl {0}, m_kognPercAnl {0};
    double m_motorErrAnl {0}, m_kognErrAnl {0};
    double m_motorMidAAnl {0}, m_kognMidAAnl {0};
    double m_motorSumTAnl {0}, m_kognSumTAnl {0};
    double m_motorMidTAnl {0}, m_kognMidTAnl {0};
    double m_motorPwrAnl {0}, m_kognPwrAnl {0};

};

#endif // TRIANGLEFACTORS_H
