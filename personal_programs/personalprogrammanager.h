#ifndef PERSONALPROGRAMMANAGER_H
#define PERSONALPROGRAMMANAGER_H

#include <QObject>
#include <QStandardItemModel>

/*!
 * \brief Класс, работающий с индивидуальными программами The PersonalProgramManager class
 */
class PersonalProgramManager : public QObject
{
    Q_OBJECT
public:
    explicit PersonalProgramManager(QObject *parent = nullptr);

    /*!
     * \brief Читает файл списка дневных программ и заполняет модель данных
     * \param model - заполняемая модель данных
     * \param uids - uid-ы дневных программ
     */
    void readDailyProgramList(QStandardItemModel& model, QStringList uids = QStringList());

    /*!
     * \brief Читает файл списка дневных программ и возвращает их в виде массива json
     * \param uids - uid-ы дневных программ
     * \return массив дневных программ
     */
    QJsonArray readDailyProgramList(QStringList uids = QStringList());

    /*!
     * \brief Сохраняет список дневных программ из модели данных
     * \param model - модель данных с дневными программами
     */
    void saveDailyProgramList(const QStandardItemModel& model);


    /*!
     * \brief Читает файл списка индивидуальных программ и заполняет модель данных
     * \param model - заполняемая модель данных
     */
    void readPersonalProgramList(QStandardItemModel &model);

    /*!
     * \brief Сохраняет список индивидуальных программ из модели данных
     * \param model - модель данных с индивидуальными программами
     */
    void savePersonalProgramList(const QStandardItemModel& model);


    /*!
     * \brief Связывает дневные программы с индивидуальной
     * \param uidPP - индивидуальная программа
     * \param uidDPs - список дневных программ
     */
    void assignDailyProgramsForPersonal(QString& uidPP, QStringList& uidDPs);

    /*!
     * \brief Изменяет связку дневных программ с индивидуальной
     * Старые дневные программы удаляются
     * \param uidPP - индивидуальная программа
     * \param uidDPs - новый список дневных программ
     */
    void editDailyProgramsForPersonal(QString& uidPP, QStringList& uidDPs);

    /*!
     * \brief Очищает список дневных программ для индивидуальной
     * \param uidPP - uid индивидуальной программы
     */
    void clearListDailyProgramsForPersonal(QString& uidPP);

    /*!
     * \brief Вовзращает список дневных программ для индивидуальной
     * \param uidPP - uid индивидуальной программы
     */
    QStringList getListDailyProgramsForPersonal(QString& uidPP);

    /*!
     * \brief Связывает индивидуальную программу с пациентом и возвращает ее в виде json объекта
     * \param patientUid - uid пациента
     * \param ppUid - uid индивидуальной программы
     * \param ppUidAssigned - uid назначенной индивидуальной программы
     * \return объект программы для пациента
     */
    QJsonObject assignPersonalProgramForPatient(const QString& patientUid, const QString& ppUid, QString& ppUidAssigned);

    /*!
     * \brief Возвращает более светлый цвет для базового
     * \param color - базовый цвет
     * \param divider - коэффициен освеления
     */
    static QColor lightColor(const QColor& color, const int divider);

    /*!
     * \brief Возвращает цвет по значению показателя от 0 до 100 схеме:
     * 0 - красный
     * 50 - желтый
     * 100 - зеленый
     * \param valSuccess - значение показателя от 0 до 100
     */
    static QColor successColor(const double valSuccess);


private:
    QString createDir();
};

#endif // PERSONALPROGRAMMANAGER_H
