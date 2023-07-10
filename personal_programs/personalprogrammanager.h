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
     */
    void readDailyProgramList(QStandardItemModel& model);

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
};

#endif // PERSONALPROGRAMMANAGER_H
