#ifndef DRIVERSFACTORY_H
#define DRIVERSFACTORY_H

#include <QObject>
#include <QMap>

#include "connection.h"


class Driver;

/*!
 * \brief Класс фабрики классов драйверов DriversFactory class
 */
class DriversFactory : public QObject
{
    Q_OBJECT
public:
    explicit DriversFactory(QObject *parent = nullptr);

    /*!
     * \brief Возвращает список uid подключенных драйверов
     */
    QStringList getDrivers() const;

    /*!
     * \brief Возвращает название драйвера по uid
     * \param drvUid
     */
    QString getDriverName(const QString &drvUid) const;

    /*!
     * \brief Возвращает список подключений
     * Прежде всего для вывода в окне редактирования
     */
    QList<Connection> getConnections() const;

    /*!
     * \brief Возвращает экземпляр драйвера, соответствующего перечню протоколов
     * \param protocols - перечень протоколов
     * \param index - номер в списке
     */
    Driver* getDriver(const QStringList &protocols, const int index = 0) const;

signals:

public slots:

private:
    void assignDrivers();
    void assignConnections();

    /*!
     * \brief Возвращает true, если драйвер с заданным uid реализует указанные протоколы
     * \param driverUid - uid драйвера
     * \param protocols - список uid протоколов
     */
    bool isDriverSupportedProtocols(const QString &driverUid, const QStringList &protocols) const;

    QMap<QString, QString> m_drivers;
    QList<Connection> m_connections;
};

#endif // DRIVERSFACTORY_H
