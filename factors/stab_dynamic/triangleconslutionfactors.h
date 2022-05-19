#ifndef TRIANGLECONSLUTIONFACTORS_H
#define TRIANGLECONSLUTIONFACTORS_H

#include <QObject>

#include "basedefines.h"
#include "multifactor.h"

namespace TriangleConslutionFactorsDefines
{
static const QString GroupUid = "{5B3AEA92-818A-4537-B6A0-2E9AAF5E04FB}";
static const QString GroupName = QCoreApplication::tr("Показатели теста \"Треугольник\". Заключение");

static const QString AccRepeatUid = "{AD73562B-2E3C-4207-91D6-4F456BD4173A}";   ///< Точность повторения
static const QString AccMidXUid = "{4703A173-A3F9-4888-BE7C-14A6141F1517}";     ///< Точность позиции (фронталь)
static const QString AccMidYUid = "{2ED3DE29-9658-4585-A3C6-67AB8D89DB3E}";     ///< Точность позиции (сагитталь)
static const QString AccFormUid = "{BFC85C9B-E951-4C23-8F82-9E388A3EAF87}";         ///< Точность формы
static const QString KorrMotorTstCntUid = "{A83C172D-CE48-4635-AB00-0946E26044B5}"; ///< Кол-во моторных коррекций (обучение)
static const QString KorrKognTstCntUid = "{996CEDA6-F526-4EF5-99F4-0D0A69513795}";  ///< Кол-во когнитивных коррекций (обучение)
static const QString KorrMotorAnlCntUid = "{243FDD01-B4E9-47A5-8B7B-0A33C38BC439}"; ///< Кол-во моторных коррекций (анализ)
static const QString KorrKognAnlCntUid = "{CD545A7B-C69A-4078-B8A0-76AB06F2FB92}";  ///< Кол-во когнитивных коррекций (анализ)
static const QString KorrDominTstUid = "{38504A18-2CBE-49CC-8238-60D51F95B5AD}";    ///< Преобладание коррекций (обучение)
static const QString KorrDominAnlUid = "{E23ACA6E-0319-4140-80B2-C712F467D5FF}";    ///< Преобладание коррекций (анализ)'
static const QString MidPointErrTstUid = "{DE1F7892-DB25-4973-A738-2AA5915675EC}";  ///< Средняя ошибка точки (обучение)'
static const QString MidSquareErrTstUid = "{0052C97E-3382-4E51-9BC5-A807AEC949D3}"; ///< Средняя ошибка площади (обучение)
static const QString MidSideErrTstUid = "{EFAC53DA-E964-4E68-A7B8-0BFEC68AAE42}";   ///< Средняя ошибка длины сторон (обучение)

static const QString MidTimeErrAnlUid = "{6CDF729F-A9BF-4A96-AD37-4EF6C7C6A119}";   ///< Среднее изменение времени прохождения тр-ка (анализ)
static const QString MidSquareErrAnlUid = "{68C85F25-BCA6-4043-BEEE-754AA3AE7D44}"; ///< Среднее изменение площади тр-ка (анализ)
static const QString MidPosErrAnlUid = "{DFD66030-77DB-4CF8-9295-1B2C047B8913}";    ///< Изменение позиции треугольника
static const QString MidAmplErrAnlUid = "{3D4E5A78-C910-4ED4-B9F0-336D93C8C268}";   ///< Среднее изменение амплитуды тр-ка (анализ)

static const QString LatentMovingTstUid = "{2AB20C4B-7589-412D-8AF9-063EAA5BFEDB}"; ///< Время начала движения после появления сигнала (обучение)

static const QString ErrCntKognTstUid = "{4E48F4AB-90DF-42DF-B803-0DCCB117C87C}";  ///< Точность выполнения когнитивным сигналом (обучение)
static const QString ErrCntMotorTstUid = "{CCF1EF34-6E26-4052-8F8F-6F229CBDC074}"; ///< Точность моторной коррекции (обучение)
static const QString ErrCntKognAnlUid = "{E3E96ECD-12E9-4502-BB62-D079A179A87E}";  ///< Точность выполнения когнитивным сигналом (анализ)
static const QString ErrCntMotorAnlUid = "{55453C3B-ABC5-42CA-925D-D58FC28CBAA1}"; ///< Точность моторной коррекции (анализ)

}

class TriangleFactors;

class TriangleConslutionFactors : public ProbeMultifactor
{
public:
    explicit TriangleConslutionFactors(const QString &testUid,
                                       const QString &probeUid,
                                       TriangleFactors* factors = nullptr,
                                       QObject *parent = nullptr);
    ~TriangleConslutionFactors() override;

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

private:
    /*!
     * \brief Расчет показателей точности повторения
     */
    void compFctAccRepeat();

    /*!
     * \brief Расчет показателей точности позиции
     */
    void compFctAccPos();

    /*!
     * \brief Расчет показателей анализа формы треугольников
     */
    void compFctShapeAnalysis();

    /*!
     * \brief Расчет показателей коррекций
     */
    void compFctCorrections();

    /*!
     * \brief Расчет показателей средних ошибок
     */
    void compFctMidErrors();

    /*!
     * \brief Расчет показателей изменений
     */
    void compFctChangeFactors();

    /*!
     * \brief Расчет различных показателей
     */
    void compFctOther();

    void addFactors();

    double m_accRepeat {0};
    double m_accMidX {0};
    double m_accMidY {0};
    double m_accForm {0};
    double m_korrMotorTstCnt {0};
    double m_korrKognTstCnt {0};
    double m_korrMotorAnlCnt {0};
    double m_korrKognAnlCnt {0};
    double m_korrDominTst {0};
    double m_korrDominAnl {0};
    double m_midPointErrTst {0};
    double m_midSquareErrTst {0};
    double m_midSideErrTst {0};

    double m_midTimeErrAnl {0};
    double m_midSquareErrAnl {0};
    double m_midPosErrAnl {0};
    double m_midAmplErrAnl {0};

    double m_latentMovingTst {0};

    double m_errCntKognTst {0};
    double m_errCntMotorTst {0};
    double m_errCntKognAnl {0};
    double m_errCntMotorAnl {0};

    bool m_isAutoCalcTF {false};

    TriangleFactors* m_factors {nullptr};   ///< Исходные показатели треугольников
};

#endif // TRIANGLECONSLUTIONFACTORS_H
