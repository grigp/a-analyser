#ifndef TRIANGLEFACTORS_H
#define TRIANGLEFACTORS_H

#include <QObject>

#include "baseutils.h"
#include "multifactor.h"
#include "signalsdefines.h"

namespace TriangleFactorsDefines
{
static const QString GroupUid = "{262C7BC3-2EFA-40CA-8D08-D770E76F57FC}";
static const QString LUpUid = "{E16E7852-9E22-4385-8A3A-B949C27505BB}";
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
     * \brief Верхняя вершина треугольника
     */
    QPointF topCorner() const;

    /*!
     * \brief Левая нижняя вершина треугольника
     */
    QPointF leftDownCorner() const;

    /*!
     * \brief Правая нижняя вершина треугольника
     */
    QPointF rightDownCorner() const;

    /*!
     * \brief Возвращает кол-во треугольников
     */
    int trianglesCount() const;

    /*!
     * \brief Возвращает границы треугольника по номеру
     * \param idx - номер треугольника
     */
    BaseUtils::Section triangle(const int idx) const;

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
     * \brief Читает данные пробы
     */
    void getTriangleData();

    TriangleResultData* m_resData;

    QList<BaseUtils::Section> m_triangles;   ///< Список треугольников

    QVector<double> m_x, m_y;                ///< Исходная стабилограмма
    QVector<double> m_xf, m_yf;              ///< Фильтраванная стабилограмма
    int m_freq {50};
    int m_firstAnalysisTriangle {0};         ///< Первый треугольник на этапе анализа
};

#endif // TRIANGLEFACTORS_H
