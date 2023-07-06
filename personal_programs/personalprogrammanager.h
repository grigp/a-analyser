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
};

#endif // PERSONALPROGRAMMANAGER_H
