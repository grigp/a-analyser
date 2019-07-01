#ifndef DRIVERSFACTORY_H
#define DRIVERSFACTORY_H

#include <QObject>
#include <QMap>

#include "connection.h"
#include "deviceprotocols.h"


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
     * \param drvUid - uid драйвера
     */
    QString getDriverName(const QString &drvUid) const;

    /*!
     * \brief Возвращает список допустимых портов для драйвера по его uid
     * \param drvUid - uid драйвера
     */
    QList<DeviceProtocols::Ports> getDriverPorts(const QString &drvUid) const;

    /*!
     * \brief Возвращает список подключений
     * Прежде всего для вывода в окне редактирования
     */
    QList<Connection> getConnections() const;

    /*!
     * \brief Добавляет подключение в список
     * \param active - флаг активности
     * \param drvUid - uid драйвера
     * \param params - параметры драйвера
     * \param port - порт
     * \param comment - комментарий
     */
    void addConnection(const bool active,
                       const QString &drvUid, const QJsonObject &params,
                       const DeviceProtocols::Ports port, const QString &comment);

    /*!
     * \brief Перемещает подключение вверх на одну позицию
     * \param connectIdx - перемещаемый индекс в массиве подключений
     */
    void moveConnectionUp(const int connectIdx);

    /*!
     * \brief Перемещает подключение вниз на одну позицию
     * \param connectIdx - перемещаемый индекс в массиве подключений
     */
    void moveConnectionDown(const int connectIdx);

    /*!
     * \brief Удаляет подключение
     * \param connectIdx - удаляемый индекс в массиве подключений
     */
    void deleteConnection(const int connectIdx);

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
    void saveConnections();

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
