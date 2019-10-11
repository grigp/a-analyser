#ifndef PERSONALNORMMANAGER_H
#define PERSONALNORMMANAGER_H

#include <QObject>

#include "datadefines.h"
#include "factorsdefines.h"

/*!
 * \brief Класс диспетчера индивидуальных норм PersonalNormManager class
 */
class PersonalNormManager : public QObject
{
    Q_OBJECT
public:
    explicit PersonalNormManager(QObject *parent = nullptr);

public:
    /*!
     * \brief Возвращает список условий проведения в виде списка uid
     */
    QStringList getTestConditions() const;

    /*!
     * \brief Возвращает данные об условиях проведения
     * \param uid - uid условий проведения
     * \param ci - информация об условиях проведения
     * \return true, если удалось вернуть
     */
    bool getTestConditionInfo(const QString &uid, DataDefines::TestConditionInfo &ci) const;

    /*!
     * \brief Получение индивидуальных норм для всех показателей теста
     * \param testUid - uid теста
     * \param pnil - возвращаемый список индивидуальных норм для теста
     * \return true, если нормы получены
     */
    bool getPersonalNorm(const QString &testUid, QList<DataDefines::PersonalNormInfo> &pnil) const;

    /*!
     * \brief Возвращает количество нормообразующих обследований для нормы, которую будет использовать тест
     * \param testUid - uid теста
     */
    int getPersonalNormContainedTestCount(const QString &testUid) const;

    /*!
     * \brief Устанавливает для теста признак включения в нормообразующие в true или false
     * \param testUid - uid теста
     * \param isNormContained - будет ли тест нормообразующим
     */
    void setTestNormContained(const QString &testUid, const bool isNormContained) const;

    /*!
     * \brief Расчет всех индивидуальных норм
     */
    void calculateAll();

signals:

public slots:

private:
    void loadConditions();

    /*!
     * \brief Расчет индивидуальной нормы для пациента по методике и условиям проведения
     * \param patientUid - uid пациента
     * \param methodUid - uid методики
     * \param conditionUid - uid условий проведения
     */
    void calculate(const QString &patientUid, const QString &methodUid, const QString &conditionUid) const;

    void fillFactorsTable(const QString &patientUid, const QString &methodUid, const QString &conditionUid,
                          QList<QList<FactorsDefines::FactorValueAdvanced>> &factors) const;

    void calculateNormsForTable(const QList<QList<FactorsDefines::FactorValueAdvanced>> &factors,
                                QList<FactorsDefines::Norm> &norms) const;

    void saveNormsToDatabase(const QString &patientUid, const QString &methodUid, const QString &conditionUid,
                             const QList<QList<FactorsDefines::FactorValueAdvanced>> &factors,
                             const QList<FactorsDefines::Norm> &norms) const;

    /*!
     * \brief Возвращает true, если тест является нормообразующим
     * Учитывает флаг от пользователя и устаревание
     * \param testId - uid теста
     */
    bool isTestNormContained(const QString &testUid) const;

    QList<DataDefines::TestConditionInfo> m_tcList;   ///< Список условий проведения

};

#endif // PERSONALNORMMANAGER_H
