#ifndef BOXERDODGINGMULTIFACTOR_H
#define BOXERDODGINGMULTIFACTOR_H

#include <QObject>

#include "multifactor.h"

namespace BoxerDodgingFactorsDefines
{
static const QString GroupUid = "{6FA3650A-8E4E-436F-8EED-F99EF62B15E8}";
static const QString AverageLatUid = "{5E562BAD-2C79-4F90-8CBA-63708E8C98A8}";
static const QString AverageTimeUid = "{A70152D5-FEC1-4891-9C3A-15507C24D321}";
static const QString AverageAmplUid = "{1E363F1F-2EF3-4B13-87C3-CB92BD91E227}";
static const QString AllErrorsUid = "{A9316F5B-6D16-4414-A54E-BDA182770622}";

static const QString LeftLatUid = "{FC1413E6-33E9-43F9-BE4A-A91BACD43756}";
static const QString LeftTimeUid = "{96F32471-AA83-4BE9-8061-14F570C4A789}";
static const QString LeftAmplUid = "{774DC444-CECC-4A38-BDC5-1939D5CABE8E}";
static const QString LeftErrorsUid = "{36ED3A98-8C90-433C-906C-7B5629B00447}";

static const QString RightLatUid = "{6B846F87-5701-4744-8F25-8BC05EB0E5D5}";
static const QString RightTimeUid = "{91035F91-978C-431E-BAFB-2F9013A2AA62}";
static const QString RightAmplUid = "{7F3F750A-195A-458C-9AB3-BC95F3E100B1}";
static const QString RightErrorsUid = "{AB768D87-8E63-4A1C-9550-0FFB4FBDCB8B}";

static const QString AheadLatUid = "{014F4F3B-B405-470D-B3C3-6690AA190357}";
static const QString AheadTimeUid = "{FE7CA0C8-3E97-4953-A180-8312350E1EA8}";
static const QString AheadAmplUid = "{B1F55138-8622-4040-821E-1788E774DD99}";
static const QString AheadErrorsUid = "{9886189E-3771-45BC-948B-6A803A5FC155}";

static const QString BackLatUid = "{0C025ED3-53B5-4738-824A-27465B952987}";
static const QString BackTimeUid = "{865526EE-49DD-4400-ACD3-68D38A24A46A}";
static const QString BackAmplUid = "{DD4923D8-A30E-4C19-87C3-246D5613196A}";
static const QString BackErrorsUid = "{DCC2BAE5-6E01-4F82-9BE7-3E5D7F69F2EC}";
}

class BoxerDodgingResultData;
class Stabilogram;


class BoxerDodgingMultifactor : public ProbeMultifactor
{
public:
    BoxerDodgingMultifactor(const QString &testUid,
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

    int deviationThreshold() const;
    int freq() const;
    int diap() const;

    int tryesCount() const {return m_fctAverage.count;}
    int leftCount() const {return m_fctLeft.count;}
    int rightCount() const {return m_fctRight.count;}
    int aheadCount() const {return m_fctAhead.count;}
    int backCount() const {return m_fctBack.count;}

private:

    ///< Структура, группирующая показатели по направлениям
    struct FactorsDirection
    {
        double latent;  ///< Латентный период
        double time;    ///< Время реакции
        double ampl;    ///< Амплитуда
        int count;      ///< Кол-во попыток
        int errors;     ///< Кол-во ошибок
    };

    ///< Границы участка сигнала
    struct Section
    {
        int begin;   ///< Начало
        int end;     ///< Конец
        Section(int b, int e) : begin(b), end(e) {}
    };

    /*!
     * \brief Читает метки этапов
     */
    void getEventLablels();

    /*!
     * \brief Составляет списки секций
     */
    void assignSections(Stabilogram* stab);

    /*!
     * \brief Рассчитывает показатели
     */
    void calculateFactors(Stabilogram* stab);

    /*!
     * \brief Рассчитывает показатели для направления
     * \param sections - список участков сигнала для направления
     * \param factors - показатели
     * \param stab - указатель на сигнал
     */
    void calculateFactorsForDirection(const QList<Section> sections,
                                      FactorsDirection& factors,
                                      Stabilogram* stab);

    BoxerDodgingResultData* m_resData {nullptr};

    QList<Section> m_secLeft;    ///< Список участков сигнала при движении влево
    QList<Section> m_secRight;   ///< Список участков сигнала при движении вправо
    QList<Section> m_secAhead;   ///< Список участков сигнала при движении вперед
    QList<Section> m_secBack;    ///< Список участков сигнала при движении назад

    FactorsDirection m_fctAverage;   ///< Усредненные показатели
    FactorsDirection m_fctLeft;      ///< Движение влево
    FactorsDirection m_fctRight;     ///< Движение вправо
    FactorsDirection m_fctAhead;     ///< Движение вперед
    FactorsDirection m_fctBack;      ///< Движение назад
};

#endif // BOXERDODGINGMULTIFACTOR_H
