#ifndef VECTORFACTORS_H
#define VECTORFACTORS_H

#include <QObject>
#include "multifactor.h"

namespace VectorFactorsDefines
{
static const QString GroupUid = "{EE47A8A0-2C55-4B8B-AF44-B642932ACC90}";
static const QString KFRUid = "{1A32C0D6-7A87-4E92-B1EE-ED0BB506EABA}";
static const QString NPVUid = "{A3EE312D-630B-423C-B21B-811FE8E6AD04}";
static const QString KRINDUid = "{92790B4F-DAAA-4666-8ECC-61017B6E0095}";
static const QString VMidUid = "{A3D24C31-7330-4AE7-A0B3-3F1CEED27361}";
static const QString AmplVUid = "{C798791A-AEBB-41F7-9DFC-DA829105EE7E}";

//! Количество диапазонов
static const int DiapsCount = 50;

//! Диапазоны векторов для КФР Элемент таблицы диапазонов (КФР)
struct Diapazone
{
    double limitLow;   ///< Нижняя граница диапазона
    double limitHigh;  ///< Верхняя граница диапазона
    int vectorsCnt;    ///< Кол-во векторов
    int vectorsAcc;    ///< Накопленное кол-во векторов
    double freq;       ///< Относительная частота
    double freqAcc;    ///< Накопленная частота
    double logarifm;   ///< Логарифм накопленной частоты
    int s;             ///< Параметр
    double lambda;     ///< Параметр аппроксимации
    Diapazone() {}
};

}

class Stabilogram;

class VectorFactors : public ChannelMultifactor
{
    Q_OBJECT
public:
    VectorFactors(const QString &testUid,
                  const QString &probeUid,
                  const QString &channelId,
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
                        const QString &probeUid,
                        const QString &channelId);

    /*!
     * \brief Рассчитывает показатели для теста с заданным uid
     * uid теста задается в конструкторе
     */
    void calculate() override;

    /*!
     * \brief Регистрирует факторсгруппу и показатели
     */
    static void registerFactors();

private:
    /*!
     * \brief Расчет средней величины отклонений
     * \param stab - указатель на стабилограмму
     */
    double deviation(Stabilogram *stab);

    /*!
     * \brief Расчет скоростей
     * \param stab - Исходная стабилограмма
     * \param spdX - Скорость по фронтали
     * \param spdY - Скорость по сагиттали
     */
    void computeSpeed(Stabilogram *stab, QVector<double> &spdX, QVector<double> &spdY);

    /*!
     * \brief Расчет векторов линейной скорости, углов и угловой скорости
     * \param spdX - Скорость по фронтали (входной)
     * \param spdY - Скорость по сагиттали (входной)
     * \param spd - Модуль вектора скорости (рассчитываемый)
     * \param angles - углы (рассчитываемый)
     * \param wSpeed - угловые скорости (рассчитываемый)
     */
    void vectorSpeed(const QVector<double> &spdX, const QVector<double> &spdY,
                     QVector<double> &spd, QVector<double> &angles, QVector<double> &wSpeed);

    /*!
     * \brief Расчет КФР
     * \param spd - модули скоростей
     */
    void computeKFR(const QVector<double> &spd);

    /*!
     * \brief Возвращает индекс диапазона в массиве диапазонов по значению
     */
    int getDiapasoneIndex(const double value) const;

    /*!
     * \brief Расчет покаателя НПВ
     * \param spdX, spdY - векторы мгновенных скоростей по координатам
     */
    void computeNPV(const QVector<double> &spdX,
                    const QVector<double> &spdY,
                    const int frequency);

    double m_kfr {0};
    double m_npv {0};
    double m_krind {0};
    double m_vMid {0};
    double m_amplV {0};

    double m_rotLf {0};
    double m_rotRt {0};

    ///< Массив диапазонов
    VectorFactorsDefines::Diapazone m_diapazones[VectorFactorsDefines::DiapsCount];
};


#endif // VECTORFACTORS_H
