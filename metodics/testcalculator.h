#ifndef TESTCALCULATOR_H
#define TESTCALCULATOR_H

#include <QObject>
#include <QStandardItemModel>

#include "factorsdefines.h"

/*!
 * \brief Базовый класс для модулей расчета и доступа к результатам теста TestCalculator class
 * Содержит:
 * - метод расчета результатов
 * - список указателей к показателям для автодинамики
 */
class TestCalculator : public QObject
{
    Q_OBJECT
public:
    explicit TestCalculator(const QString &testUid, QObject *parent = nullptr);

    /*!
     * \brief Уровни структуры теста TestLevel enum
     */
    enum TestLevel
    {
          LevelTest = 0
        , LevelProbe
        , LevelChannel
    };

    /*!
     * \brief Роли в модели данных структуры теста TestTreeRoles enum
     */
    enum TestTreeRoles
    {
          LevelRole = Qt::UserRole + 1     ///<  Уровень. TestLevel. В каждом узле свое значение
        , TestUidRole                      ///<  uid теста. QString. В узле теста
        , TestDateTimeRole                 ///<  дата и время проведения теста. QDateTime. В узле теста
        , PatientUidRole                   ///<  uid пациента. QString. В узле теста
        , MetodicUidRole                   ///<  uid методики. QString. В узле теста
        , ParamsRole                       ///<  параметры методики. QJsonObject. В узле теста
        , ProbeUidRole                     ///<  uid пробы. QString. В узле пробы
        , ProbeStepRole                    ///<  номер этапа пробы. int. В узле пробы
        , ChannelUidRole                   ///<  uid канала. QString. В узле канала
        , ChannelIdRole                    ///<  id канала. QString. В узле канала
        , SignalRole                       ///<  Записанный сигнал в виде QByteArray. В узле канала
        , FactorsgroupRole                 ///<  Указатель на рассчитанную факторсгруппу. В дополнительных столбцах
        , FactorsgroupUidRole              ///<  uid рассчитанной факторсгруппы
    };

    /*!
     * \brief Полный расчет данных теста с записью значений первичных показателей в БД
     */
    virtual void calculate();

    /*!
     * \brief Быстрый расчет данных теста
     * Чтение первичных показателей из БД
     */
    virtual void fastCalculate();

    /*!
     * \brief Возвращает кол-во первичных показателей
     */
    int primaryFactorsCount() const {return m_primaryFactors.size();}

    /*!
     * \brief Возвращает первичный показатель по номеру в списке
     * \param num - номер в списке
     */
    FactorsDefines::FactorValueAdvanced* primaryFactor(const int num) const;

    QString testUid() const {return m_testUid;}

protected:
    /*!
     * \brief Построение дерева теста
     */
    //virtual void computeTestTree();

    /*!
     * \brief Добавляет первичный показатель в список
     * Добавлять будут подклассы
     * \param uid - uid показателя
     * \param value - значение
     * \param probeNum - номер пробы
     * \param channelId - идентификатор канала
     * \param description - описание
     */
    void addPrimaryFactor(const QString &testUid,
                          const QString &uid,
                          const double value,
                          const int probeNum,
                          const QString &channelId,
                          const QString &description);

    /*!
     * \brief Добавляет первичный показатель в список
     * Без данных о пробе. Для показателей уровня теста
     */
    void addPrimaryFactor(const QString &testUid,
                          const QString &uid,
                          const double value,
                          const QString &description);

    QStandardItemModel m_mdlTest;

private:
    QString m_testUid;
    QList<FactorsDefines::FactorValueAdvanced*> m_primaryFactors;
};

///<------------------------------------------------------------------------------------------------
/*!
 * \brief Класс модуля расчета результатов стабилографического теста StabTestCalculator class
 */
class StabTestCalculator : public TestCalculator
{
    Q_OBJECT
public:
    StabTestCalculator(const QString &testUid, QObject *parent = nullptr)
        : TestCalculator(testUid, parent)
    {}
};

#endif // TESTCALCULATOR_H
