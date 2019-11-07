#include "driversfactory.h"

#include "aanalyserapplication.h"
#include "stabilan01.h"
#include "jumpplate.h"
#include "datadefines.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>

DriversFactory::DriversFactory(QObject *parent) : QObject(parent)
{
    QTimer::singleShot(0, [=]()
    {
        assignDrivers();
        assignConnections();
    });
}

QStringList DriversFactory::getDrivers() const
{
    return m_drivers.keys();
}

QString DriversFactory::getDriverName(const QString &drvUid) const
{
    return m_drivers.value(drvUid);
}

QList<DeviceProtocols::Ports> DriversFactory::getDriverPorts(const QString &drvUid) const
{
    //! Надо хардкодить все драйвера
    if (drvUid == Stabilan01::uid())
        return Stabilan01::getPorts();
    else
    if (drvUid == JumpPlate::uid())
        return JumpPlate::getPorts();

    return QList<DeviceProtocols::Ports>();
}

QList<Connection> DriversFactory::getConnections() const
{
    return m_connections;
}

void DriversFactory::addConnection(const bool active,
                                   const QString &drvUid,
                                   const QJsonObject &params,
                                   const DeviceProtocols::Ports port,
                                   const QString &comment)
{
    Connection connection(drvUid, m_drivers.value(drvUid), port, params, active, comment);
    m_connections << connection;
    saveConnections();
}

void DriversFactory::moveConnectionUp(const int connectIdx)
{
    if (connectIdx > 0)
    {
        m_connections.move(connectIdx, connectIdx - 1);
        saveConnections();
    }
}

void DriversFactory::moveConnectionDown(const int connectIdx)
{
    if (connectIdx < m_connections.size() - 1)
    {
        m_connections.move(connectIdx, connectIdx + 1);
        saveConnections();
    }
}

void DriversFactory::deleteConnection(const int connectIdx)
{
    if (connectIdx >= 0 && connectIdx < m_connections.size())
    {
        m_connections.removeAt(connectIdx);
        saveConnections();
    }
}

void DriversFactory::dataChangedConnection(const int connectIdx, const int paramIdx, const QVariant value)
{
    bool isChanged = false;

    Connection connection = m_connections.at(connectIdx);
    switch (paramIdx) {
    case 0:
        connection.setActive(value.toBool());
        isChanged = true;
        break;
    case 2:
        connection.setPort(static_cast<DeviceProtocols::Ports>(value.toInt()));
        isChanged = true;
        break;
    case 3:
        connection.setComment(value.toString());
        isChanged = true;
        break;
    default:
        break;
    }

    if (isChanged)
    {
        m_connections.replace(connectIdx, connection);
        saveConnections();
    }
}

bool DriversFactory::editParamsConnecton(const int connectIdx, const QString &drvUid, QJsonObject &params)
{
    bool resval = false;
    //! Надо хардкодить все драйвера
    if (drvUid == Stabilan01::uid())
        resval = Stabilan01::editParams(params);
    else
    if (drvUid == JumpPlate::uid())
        resval = JumpPlate::editParams(params);

    if (resval && connectIdx > -1)
    {
        Connection connection = m_connections.at(connectIdx);
        connection.setParams(params);
        m_connections.replace(connectIdx, connection);
        saveConnections();
    }
    return resval;
}

Driver *DriversFactory::getDriverByProtocols(const QStringList &protocols, const int index) const
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
                    drv->setParams(connection.port(), connection.params());
                    return drv;
                }
                ++num;
            }

    return nullptr;
}

Driver *DriversFactory::getDriverByFormats(const QStringList &formats, const int index) const
{
    int num = 0;
    foreach (auto connection, m_connections)
        if (connection.active())
            if (isDriverSupportedFormats(connection.driverUid(), formats))
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
                    drv->setParams(connection.port(), connection.params());
                    return drv;
                }
                ++num;
            }

    return nullptr;
}

void DriversFactory::assignDrivers()
{
    //! Надо хардкодить все драйвера
    m_drivers.insert(Stabilan01::uid(), Stabilan01::name());
    m_drivers.insert(JumpPlate::uid(), JumpPlate::name());
}

void DriversFactory::assignConnections()
{
    //! Файл со списком подключений берем из ресурса, создаем его на диске,
    //! после чего читаем и редактируем уже с диска.
    //! Это позволяет не инсталить подключения, а создавать все в
    //! в процессе работы
    if (!QFile::exists(DataDefines::appCopyPath() + "connections.json"))
    {
        QDir dir(DataDefines::appCopyPath());
        if (!dir.exists())
            dir.mkpath(DataDefines::appCopyPath());
        if (static_cast<AAnalyserApplication*>(QApplication::instance())->languargeCode() == DataDefines::LANG_CODE_RUS)
            QFile::copy(":/pre_settings/connections.json",
                        DataDefines::appCopyPath() + "connections.json");
        else
            if (static_cast<AAnalyserApplication*>(QApplication::instance())->languargeCode() == DataDefines::LANG_CODE_ENGUSA)
                QFile::copy(":/pre_settings/connections_en_US.json",
                            DataDefines::appCopyPath() + "connections.json");
    }

    QFile fConnect(DataDefines::appCopyPath() + "connections.json");
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
            auto active = objConnect["active"].toBool();
            auto port = objConnect["port"].toInt();
            auto params = objConnect["params"].toObject();

            Connection connection(drvUid, drvName, static_cast<DeviceProtocols::Ports>(port), params, active, comment);
            m_connections << connection;
        }
    }
}

void DriversFactory::saveConnections()
{
    QFile fConnect(DataDefines::appCopyPath() + "connections.json");
    if (fConnect.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonArray connections;
        foreach (auto connection, m_connections)
        {
            QJsonObject obj;
            obj["active"] = connection.active();
            obj["driver_uid"] = connection.driverUid();
            obj["port"] = connection.port();
            obj["comment"] = connection.comment();
            obj["params"] = connection.params();

            connections.append(obj);
        }

        QJsonObject root;
        root["connections"] = connections;

        QJsonDocument doc(root);
        QByteArray ba = doc.toJson();
        fConnect.write(ba);
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

bool DriversFactory::isDriverSupportedFormats(const QString &driverUid, const QStringList &formats) const
{
    QStringList protocolsDrv = QStringList();

    //! Получаем протоколы от драйвера
    //! Надо хардкодить все драйвера
    if (driverUid == Stabilan01::uid())
        protocolsDrv = Stabilan01::getProtocols();
    else
    if (driverUid == JumpPlate::uid())
        protocolsDrv = JumpPlate::getProtocols();

    //! Переводим протоколы в форматы. В протоколе всегда данные одного формата.
    //! Но драйвер может реализовывать данные разных протоколов и поставлять данные разных форматов
    QStringList formatsDrv = QStringList();
    foreach (auto protocol, protocolsDrv)
        formatsDrv.append(DeviceProtocols::protocolFormat.value(protocol));

    //! Ищем все переданные форматы в полученном списке
    if (formatsDrv != QStringList())
    {
        foreach (auto format, formats)
            if (!formatsDrv.contains(format))
                return false;
        return true;
    }
    return false;
}

