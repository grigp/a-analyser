#ifndef DRIVER_H
#define DRIVER_H

#include "deviceprotocols.h"

#include "serialportdefines.h"

#include <QObject>
#include <QStringList>
#include <QThread>

class SerialPort;

/*!
 * \brief Базовый класс драйвера Driver class
 */
class Driver : public QObject
{
    Q_OBJECT
public:
    explicit Driver(QObject *parent = nullptr);
    ~Driver() override;

    enum ErrorCodes
    {
          EC_NoError = 0
        , EC_NoData = 1
        , EC_User = 100
        , EC_MarkerIinsidePackage = 101
    };

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
    virtual void start();
    /*!
     * \brief Останов передачи данных
     */
    virtual void stop();

    /*!
     * \brief Возвращает частоту дискретизации для канала по его id
     * \param channelId - идентификатор канала
     */
    virtual int frequency(const QString &channelId) const = 0;

    /*!
     * \brief Возвращает список uid-ов каналов, передаваемых драйвером по uid протокола
     * \param protocolUid - uid протокола
     */
    virtual QList<QString> getChannelsByProtocol(const QString &protocolUid) const = 0;

    /*!
     * \brief Возвращает список uid-ов каналов, передаваемых драйвером по uid формата данных
     * \param protocolUid - uid формата данных
     */
    virtual QList<QString> getChannelsByFormat(const QString &formatUid) const = 0;

    /*!
     * \brief Возвращает список всех каналов, передаваемых драйвером
     */
    virtual QList<QString> getChannels() const = 0;

    /*!
     * \brief Возвращает кол-во подканалов для канала с заданным uid
     * \param channelUid - uid канала
     */
    virtual int getSubChannelsCount(const QString &channelUid) const = 0;

    /*!
     * \brief Возвращает true, если канал записывается по умолчанию (установлено в драйвере)
     * \param channelUid - uid канала
     * \param subChan - номер подканала
     */
    virtual bool isChannelRecordingDefault(const QString &channelUid, const int subChan = -1) const = 0;

    /*!
     * \brief Возвращает интерфейс управления устройством для заданного канала
     * \param controlId - uid интерфейса управления устройством
     * \param channelId - идентификатор канала
     */
    virtual DeviceProtocols::DeviceControl* getDeviceControl(const QString &controlId, const QString &channelId = "") = 0;

    DeviceProtocols::Ports portName() const {return m_portName;}


signals:
    void started();
    void stopped();

    void sendData(DeviceProtocols::DeviceData *data);
    void communicationError(const QString &drvName, const QString &port, const int errorCode);

    void connectPort();
    void disconnectPort();
    void portSettings(const QString &name,
                      const int baudrate, const int DataBits, const int Parity,
                      const int StopBits, const int FlowControl);
    void writeData(const QByteArray data);
    void error(const int errorCode);

public slots:

protected slots:
    virtual void on_readData(const QByteArray data);
    virtual void on_error(const QString &err);

protected:
//    DeviceProtocols::Ports portName() const {return m_portName;}
    void setPortName(const DeviceProtocols::Ports portName) {m_portName = portName;}

    SerialPort* port() const {return m_port;}
    QThread* trdInput() const {return m_trdInput;}

    /*!
     * \brief Возвращает настройки порта
     */
    virtual SerialPortDefines::Settings getSerialPortSettings();

    void incBlockCount() {++m_blockCount;}
    int blockCount() const {return m_blockCount;}

    void timerEvent(QTimerEvent *event) override;

private:
    int m_blockCount {0};                    ///< Счетчик пакетов
    int m_blockCountPrev {0};
    int m_tmCommError {-1};                  ///< id таймера ошибки связи
    bool m_isCommunicationError {false};     ///< признак ошибки связи

    DeviceProtocols::Ports m_portName;
    SerialPort *m_port {nullptr};
    QThread *m_trdInput {nullptr};
};

#endif // DRIVER_H
