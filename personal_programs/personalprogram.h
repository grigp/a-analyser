#ifndef PERSONALPROGRAM_H
#define PERSONALPROGRAM_H

#include <QObject>
#include <QStandardItemModel>
#include <QUuid>
#include <QDate>

/*!
 * \brief Класс модели данных индивидуальной программы The PersonalProgram class
 * Строки - дневные программы, столбцы - тесты. Кол-во тестов в различных дневных программах может быть различным
 */
class PersonalProgram : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit PersonalProgram(QObject *parent = Q_NULLPTR);

    /*!
     * \brief Загружает модель данными из файла индивидуальной программы
     * \param fileName - имя файла
     */
    void load(const QString& fileName);

    /*!
     * \brief Загружает модель данными из модели индивидуальной программы
     * \param objPPAll - модель индивидуальной программы
     */
    void load(const QJsonObject& objPPAll);

    /*!
     * \brief Создает объект, содержащий программу.
     * Противовес load(...)
     * \return созданный объект
     */
    QJsonObject save();

    /*!
     * \brief Добавляет объект дневной программы к модели
     * \param objDP - объект дневной программы
     */
    void addDailyProgram(const QJsonObject& objDP);

    /*!
     * \brief Дублирует дневную программу
     * \param number - номер дублируемой программы
     */
    void doubleDailyProgram(const int number);

    /*!
     * \brief Добавляет объект теста к дневной программы к модели
     * \param numDP - номер дневной программы
     * \param metUid - uid методики
     * \param params - параметры методики
     */
    void addTest(int numDP, QString& metUid, QJsonObject& params);

    /*!
     * \brief Дублирует тест в дневной программе
     * \param numDP - номер дневной программы
     * \param numTest - номер теста
     */
    void doubleTest(const int numDP, const int numTest);

    QString uid() const {return m_uid;}
    QString name() const {return m_name;}
    QString logoFileName() const {return m_logoFileName;}
    QString uidPatient() const {return m_uidPatient;}

    int minTimeBetweenDP() const {return m_minTimeBetweenDP;}
    void setMinTimeBetweenDP(const int minTime) {m_minTimeBetweenDP = minTime;}

    int maxTimeBetweenDP() const {return m_maxTimeBetweenDP;}
    void setMaxTimeBetweenDP(const int maxTime) {m_maxTimeBetweenDP = maxTime;}

    QDate dateBegin() const {return m_dateBegin;}
    QDate dateEnd() const {return m_dateEnd;}
    bool isActive() const {return m_isActive;}

    /*!
     * \brief Проверяет, проведены ли тесты в индивидуальной программе
     * \param pp - индивидуальная программа
     * \return true, если есть хотя бы один тест
     */
    static bool isTestByPPExists(QJsonObject& pp);

    /*!
     * \brief Проверяет, проведены ли тесты в индивидуальной программе
     * \return true, если есть хотя бы один тест
     */
    bool isTestByPPExists();

private:
    //!--- Параметры индивидуальной программы ---------------
    QString m_uid {QUuid().toString()};          ///< uid
    QString m_uidAssigned {""};                  ///< uid назначенной ИП
    QString m_name {""};                         ///< Название
    QString m_logoFileName {""};                 ///< Имя файла с логотипом
    QString m_uidPatient {QUuid().toString()};   ///< uid пациента
    int m_minTimeBetweenDP {0};                  ///< Минимальное время между дневными программами
    int m_maxTimeBetweenDP {0};                  ///< Макснимальное время между дневными программами

    QDate m_dateBegin {QDate()};                 ///< Дата начала программы
    QDate m_dateEnd {QDate()};                   ///< Дата окончания программы
    bool m_isActive {false};                     ///< Активна ли программа
};

#endif // PERSONALPROGRAM_H
