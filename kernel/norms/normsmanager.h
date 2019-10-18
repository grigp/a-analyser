#ifndef NORMSMANAGER_H
#define NORMSMANAGER_H

#include <QObject>

#include "datadefines.h"
#include "factorsdefines.h"

/*!
 * \brief Класс диспетчера индивидуальных норм NormsManager class
 */
class NormsManager : public QObject
{
    Q_OBJECT
public:
    explicit NormsManager(QObject *parent = nullptr);

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
    void setTestNormContained(const QString &testUid, const bool isNormContained);

    /*!
     * \brief Расчет индивидуальной нормы для пациента по методике и условиям проведения
     * \param patientUid - uid пациента
     * \param methodUid - uid методики
     * \param conditionUid - uid условий проведения
     */
    void calculate(const QString &patientUid, const QString &methodUid, const QString &conditionUid);

    /*!
     * \brief Расчет всех индивидуальных норм
     */
    void calculateAll();

    /*!
     * \brief Возвращает список групповых норм для заданной методике по заданным условиям проведения
     * \param methodicUid - uid методики
     * \param consitionUid - uid условий проведения
     * \param gni - список индивидуальных норм
     * \return true, если удалось вернуть
     */
    bool getGroupNorms(const QString &methodicUid, const QString &conditionUid,
                       QList<DataDefines::GroupNormInfo> &gni) const;


signals:
    /*!
     * \brief Сигнал о том, что индивидуальная норма пересчитана
     * \param patientUid - uid пациента
     * \param methodUid - uid методики
     * \param conditionUid - uid условий проведения
     */
    void personalNormRecalculated(const QString &patientUid, const QString &methodUid, const QString &conditionUid);

    /*!
     * \brief Сигнал о том, что индивидуальная норма удалена
     * \param patientUid - uid пациента
     * \param methodUid - uid методики
     * \param conditionUid - uid условий проведения
     */
    void personalNormDeleted(const QString &patientUid, const QString &methodUid, const QString &conditionUid);

    /*!
     * \brief Сигнал о том, что у индивидуальной нормы изменено кол-во нормообразующих обследований
     * \param patientUid - uid пациента
     * \param methodUid - uid методики
     * \param conditionUid - uid условий проведения
     */
    void personalNormContainedChange(const QString &patientUid, const QString &methodUid, const QString &conditionUid);

public slots:

private:
    void loadConditions();
    void loadDopuskGroupNorms();

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
    QList<DataDefines::GroupNormInfo> m_groupNorms;   ///< Список групповых норм
};

#endif // NORMSMANAGER_H
