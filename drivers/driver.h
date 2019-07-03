#ifndef DRIVER_H
#define DRIVER_H

#include "deviceprotocols.h"

#include <QObject>
#include <QStringList>

/*!
 * \brief Базовый класс драйвера Driver class
 */
class Driver : public QObject
{
    Q_OBJECT
public:
    explicit Driver(QObject *parent = nullptr);

    /*!
     * \brief Виртуальные методы, возвращающие uid и name драйверов
     * Есть методы в подклассах uid и name, но они статические и невиртуальные
     */
    virtual QString driverUid() const = 0;
    virtual QString driverName() const = 0;

    /*!
     * \brief Устанавливает параметры драйвера
     * для использования при работе
     * \param params - параметры
     * \param port - порт
     */
    virtual void setParams(const DeviceProtocols::Ports port, const QJsonObject &params) = 0;

    /*!
     * \brief Запуск передачи данных
     */
    virtual void start() = 0;
    /*!
     * \brief Останов передачи данных
     */
    virtual void stop() = 0;


signals:
    void sendData(DeviceProtocols::DeviceData *data);
    void communicationError(const int errorCode);

public slots:
};

#endif // DRIVER_H
