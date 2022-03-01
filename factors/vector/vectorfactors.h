#ifndef VECTORFACTORS_H
#define VECTORFACTORS_H

#include <QObject>
#include <QVector>
#include "multifactor.h"

namespace VectorFactorsDefines
{
static const QString GroupUid = "{EE47A8A0-2C55-4B8B-AF44-B642932ACC90}";
static const QString KFRUid = "{1A32C0D6-7A87-4E92-B1EE-ED0BB506EABA}";
static const QString NPVUid = "{A3EE312D-630B-423C-B21B-811FE8E6AD04}";
static const QString KRINDUid = "{92790B4F-DAAA-4666-8ECC-61017B6E0095}";

static const QString VMidUid = "{A3D24C31-7330-4AE7-A0B3-3F1CEED27361}";
static const QString AmplVUid = "{C798791A-AEBB-41F7-9DFC-DA829105EE7E}";
static const QString TVUid = "{B8BB2479-295D-4E4D-91D3-140765E9A48A}";

static const QString WMidUid = "{D42D78E5-55FB-47AE-B8A0-CCFEDC9D0319}";
static const QString AmplWUid = "{552F3C8C-D647-4EA1-BD65-DCE77C92EA6B}";
static const QString TWUid = "{E996D938-DF03-4E04-8094-BC13EAA6B57F}";

static const QString KAUSUid = "{322CA2B5-B016-4EBF-AE22-3EC3B12DC018}";
static const QString NUSUid = "{E2AA941A-A124-40CD-BCB4-1F87BD9A6E25}";

static const QString LSSFUid = "{D82C60C6-D09D-470D-A28B-78EC83D7D841}";
static const QString LSSSUid = "{58C9D5ED-22D3-48C3-A2FD-BCACE16600A6}";
static const QString KALSFUid = "{C0AD66A6-2FB2-4E2D-8453-B6B8D8880D89}";
static const QString KALSSUid = "{0B3D724A-DADC-4432-9BFC-64087D55398D}";

static const QString PwVgrUid = "{BED2A473-10D9-4719-8CC5-10E3BEC1DCE5}";
static const QString VWUid = "{85984CC0-E541-4AF4-B6BE-3DF9E05840A1}";

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

/*!
 * \brief Класс модуля расчета векторных показателей  VectorFactors class
 */
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

    /*!
     * \brief Доступ к данным функции накопления значений
     * \param idx - номер диапазона
     * \return Значение функции накопления в данном номере
     */
    double accumulationFuncValue(const int idx) const;

    /*!
     * \brief Доступ к данным границ диапазонов
     * \param idx - номер диапазона
     * \return Значение границы
     */
    double diapRangeLimit(const int idx) const;

    /*!
     * \brief Возвращает кол-во векторов
     */
    int vectorCount() const;

    /*!
     * \brief Доступ к векторам скоростей
     * \param idx - номер вектора
     * \return составляющие скорости по фронтали и сагиттали
     */
    QPointF vector(const int idx) const;

private:
    /*!
     * \brief Расчет средней величины отклонений
     * \param stab - указатель на стабилограмму
     */
    double deviation(Stabilogram *stab);

    /*!
     * \brief Расчет скоростей
     * \param stab - Исходная стабилограмма
     * \param spdX, spdY - Скорости по фронтали и сагиттали
     * \param accelX, accelY - Ускорения по фронтали и сагиттали
     */
    void computeSpeed(Stabilogram *stab,
                      QVector<double> &spdX, QVector<double> &spdY,
                      QVector<double> &accelX, QVector<double> &accelY);

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

    /*!
     * \brief Расчет НУС
     * \param wSpeed - массив угловой скорости
     */
    void computeNUS(const QVector<double> &wSpeed);

    /*!
     * \brief Расчет показателей вариативности скорости
     * \param spd - вектор значений скорости
     * \param frequency - частота дискретизации
     * \param amplAv - амплитуда вариации скорости
     * \param timeAv - период вариации скорости
     */
    void computeVariationFactors(const QVector<double> &spd, const int frequency,
                                 double &amplAv, double &timeAv) const;

    /*!
     * \brief Расчет коэффициента асимметрии для массива скорости
     * \param spd - массив скорости
     * \return коэффициент асимметрии
     */
    double computeAsymmetry(const QVector<double> &spd);

    /*!
     * \brief Расчет мощности векторограммы
     * \param stab - указатель на стабилограмму
     */
    void computePowerVector(Stabilogram *stab);

    double m_kfr {0};
    double m_npv {0};
    double m_krind {0};

    double m_vMid {0};
    double m_amplV {0};
    double m_tV {0};

    double m_wMid {0};
    double m_amplW {0};
    double m_tW {0};

    double m_kaus {0};
    double m_nus {0};

    double m_lss_f {0};
    double m_lss_s {0};
    double m_kals_f {0};
    double m_kals_s {0};

    double m_pv {0};
    double m_vw {0};

    double m_rotLf {0};
    double m_rotRt {0};

    QVector<double> m_spdX, m_spdY;

    ///< Массив диапазонов
    VectorFactorsDefines::Diapazone m_diapazones[VectorFactorsDefines::DiapsCount];
};


#endif // VECTORFACTORS_H
