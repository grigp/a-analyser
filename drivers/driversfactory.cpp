#include "driversfactory.h"

#include "stabilan01.h"
#include "jumpplate.h"
#include "datadefines.h"
#include "deviceprotocols.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

DriversFactory::DriversFactory(QObject *parent) : QObject(parent)
{
    assignDrivers();
    assignConnections();
}

QStringList DriversFactory::getDrivers() const
{
    return m_drivers.keys();
}

QString DriversFactory::getDriverName(const QString &drvUid) const
{
    return m_drivers.value(drvUid);
}

QList<Connection> DriversFactory::getConnections() const
{
    return m_connections;
}

Driver *DriversFactory::getDriver(const QStringList &protocols, const int index) const
{
    int num = 0;
    foreach (auto connection, m_connections)
        if (connection.active())
            if (isDriverSupportedProtocols(connection.driverUid(), protocols))
            {
                if (num == index)
                {
                    //! Создаем и возвращаем экземпляры драйверов
                    //! Надо хардкодить все драйвера
                    Driver *drv{nullptr} ;
                    if (connection.driverUid() == Stabilan01::uid())
                        drv = new Stabilan01();
                    else
                    if (connection.driverUid() == JumpPlate::uid())
                        drv = new JumpPlate();
                    //! Параметры драйверу!
                    drv->setParams(connection.params());
                    return drv;
                }
                ++num;
            }

    return nullptr;
}

void DriversFactory::assignDrivers()
{
    //! Надо хардкодить все драйвера
    auto uid = Stabilan01::uid();
    auto name = Stabilan01::name();
    m_drivers.insert(uid, name);

    uid = JumpPlate::uid();
    name = JumpPlate::name();
    m_drivers.insert(uid, name);
}

void DriversFactory::assignConnections()
{
    //! Файл со списком подключений берем из ресурса, создаем его на диске,
    //! после чего читаем и редактируем уже с диска.
    //! Это позволяет не инсталить подключения, а создавать все в
    //! в процессе работы
    if (!QFile::exists(DataDefines::appDataPath() + "connections.json"))
        QFile::copy(":/connections.json",
                    DataDefines::appDataPath() + "connections.json");

    QFile fConnect(DataDefines::appDataPath() + "connections.json");
    fConnect.setPermissions((((fConnect.permissions() |= QFile::WriteOwner) |= QFile::WriteUser) |= QFile::WriteGroup) |= QFile::WriteOther);
    if (fConnect.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray ba = fConnect.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(ba));
        QJsonObject root = doc.object();
        QJsonArray connects = root["connections"].toArray();

        for (int i = 0; i < connects.size(); ++i)
        {
            auto objConnect = connects[i].toObject();
            auto drvUid = objConnect["driver_uid"].toString();
            auto drvName = m_drivers.value(drvUid);
            auto comment = objConnect["comment"].toString();
            auto active = objConnect["active"].toInt();
            auto port = objConnect["port"].toString();
            auto params = objConnect["params"].toObject();

            Connection connection(drvUid, drvName, port, params, active, comment);
            m_connections << connection;
        }
    }
}

bool DriversFactory::isDriverSupportedProtocols(const QString &driverUid, const QStringList &protocols) const
{
    QStringList protocolsDrv = QStringList();

    //! Получаем протоколы от драйвера
    //! Надо хардкодить все драйвера
    if (driverUid == Stabilan01::uid())
        protocolsDrv = Stabilan01::getProtocols();
    else
    if (driverUid == JumpPlate::uid())
        protocolsDrv = JumpPlate::getProtocols();

    //! Ищем все переданные протоколы в полученном списке
    if (protocolsDrv != QStringList())
    {
        foreach (auto protocol, protocols)
            if (!protocolsDrv.contains(protocol))
                return false;
        return true;
    }
    return false;
}
