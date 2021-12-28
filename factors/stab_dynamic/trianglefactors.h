#ifndef TRIANGLEFACTORS_H
#define TRIANGLEFACTORS_H

#include <QObject>

#include "baseutils.h"
#include "multifactor.h"
#include "signalsdefines.h"
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
}

/*!
 * \brief Значения некоторых показателей FactorValues struct
 */
struct FactorValues
{
    double timeQ {0};    ///< Разброс длительности проходов
    double squareQ {0};  ///< Разброс площади треугольников
    double speedQ {0};   ///< Разброс скорости прохождения
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
    BaseUtils::Section triangleSection(const int idx) const;

    /*!
     * \brief Возвращает треугольник по номеру
     * \param idx - номер треугольника
     */
    TriangleDefines::Triangle triangle(const int idx) const;

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
     * \brief Читает данные пробы
     */
    void getTriangleData();

    void addFactors();

    TriangleResultData* m_resData;

    QList<BaseUtils::Section> m_triangleSections;   ///< Список участков сигнала треугольников
    QList<TriangleDefines::Triangle> m_triangles;   ///< Список треугольников
    TriangleDefines::Triangle m_triangleAverageTraining; ///< Усредненный треугольник на этапе обучения
    TriangleDefines::Triangle m_triangleAverageAnalysis; ///< Усредненный треугольник на этапе анализа

    QVector<double> m_x, m_y;                ///< Исходная стабилограмма
    QVector<double> m_xf, m_yf;              ///< Фильтраванная стабилограмма
    int m_freq {50};
    int m_firstAnalysisTriangle {0};         ///< Первый треугольник на этапе анализа

    TriangleFactorsDefines::FactorValues m_valuesTraining;
    TriangleFactorsDefines::FactorValues m_valuesAnalysis;
};

#endif // TRIANGLEFACTORS_H
