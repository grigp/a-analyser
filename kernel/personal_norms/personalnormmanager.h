#ifndef PERSONALNORMMANAGER_H
#define PERSONALNORMMANAGER_H

#include <QObject>

#include "datadefines.h"

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
    QStringList getTestConditions();

    /*!
     * \brief Возвращает данные об условиях проведения
     * \param uid - uid условий проведения
     * \param ci - информация об условиях проведения
     * \return true, если удалось вернуть
     */
    bool getTestConditionInfo(const QString &uid, DataDefines::TestConditionInfo &ci);

signals:

public slots:

private:
    void loadConditions();

    QList<DataDefines::TestConditionInfo> m_tcList;   ///< Список условий проведения
};

#endif // PERSONALNORMMANAGER_H
