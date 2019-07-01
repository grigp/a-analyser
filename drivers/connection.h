#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QJsonObject>

#include "deviceprotocols.h"

/*!
 * \brief Класс данных для подключения к устройству Connection class
 */
class Connection
{
public:
    explicit Connection() {}
    explicit Connection(const QString &drvUid,
                        const QString &drvName,
                        const DeviceProtocols::Ports port,
                        const QJsonObject &params,
                        const bool active,
                        const QString &comment)
        : m_driverUid(drvUid)
        , m_driverName(drvName)
        , m_port(port)
        , m_params(params)
        , m_active(active)
        , m_comment(comment) {}

    QString driverUid() const {return m_driverUid;}
    QString driverName() const {return m_driverName;}

    DeviceProtocols::Ports port() const {return m_port;}
    void setPort(const DeviceProtocols::Ports port) {m_port = port;}

    QJsonObject params() const {return m_params;}
    void setParams(const QJsonObject &params) {m_params = params;}

    QString comment() const {return m_comment;}
    void setComment(const QString &comment) {m_comment = comment;}

    bool active() const {return m_active;}
    void setActive(const bool active) {m_active = active;}

private:
    QString m_driverUid;
    QString m_driverName;
    DeviceProtocols::Ports m_port;
    QJsonObject m_params;
    bool m_active;
    QString m_comment;

};

#endif // CONNECTION_H
