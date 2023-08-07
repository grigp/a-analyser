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
     * \brief Добавляет объект дневной программы к модели
     * \param objDP - объект дневной программы
     */
    void addDailyProgram(const QJsonObject& objDP);

//    /*!
//     * \brief Добавляет объект теста к дневной программы к модели
//     * \param objTest - объект теста
//     */
//    void addTest(const QJsonObject& objTest);

    QString uid() const {return m_uid;}
    QString name() const {return m_name;}
    QString logoFileName() const {return m_logoFileName;}
    QString uidPatient() const {return m_uidPatient;}
    int minTimeBetweenDP() const {return m_minTimeBetweenDP;}
    int maxTimeBetweenDP() const {return m_maxTimeBetweenDP;}

    QDate dateBegin() const {return m_dateBegin;}
    QDate dateEnd() const {return m_dateEnd;}
    bool isActive() const {return m_isActive;}


private:
    //!--- Параметры индивидуальной программы ---------------
    QString m_uid {QUuid().toString()};          ///< uid
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
