#ifndef MWSTICK_H
#define MWSTICK_H

#include <QObject>
#include <QCoreApplication>
#include <QThread>
#include <QByteArray>

#include "driver.h"
#include "deviceprotocols.h"
#include "devicesdefines.h"



class SerialPort;
class Connection;


/*!
 * \brief Класс драйвера соревновательной палки The MWStick class
 */
class MWStick : public Driver
        , public DeviceProtocols::TensoControl
{
    Q_OBJECT
public:
    explicit MWStick(QObject *parent = nullptr);
    ~MWStick() override;

    static QString uid() {return DevicesDefines::uid_mw_stick;}
    static QString name() {return tr("Соревновательная палка");} //DevicesDefines::name_stabilan01);} Не локализуется через константу

    /*!
     * \brief Перекрытые методы, возвращающие uid и name драйверов
     * uid и name можно использовать, как методы класса, а эти - только с экземпляром
     */
    QString driverUid() const override {return uid();}
    QString driverName() const override {return name();}

    /*!
     * \brief Устанавливает параметры драйвера
     * для использования при работе
     * \param params - параметры
     * \param port - порт
     */
    void setParams(const DeviceProtocols::Ports port, const QJsonObject &params) override;

    /*!
     * \brief Вызывает диалог редактирования параметров
     * \param params - редактируемые параметры
     * \return true, если диалог закончился командой сохранить параметры
     */
    static bool editParams(QJsonObject &params);

    /*!
     * \brief Запуск передачи данных
     */
    void start() override;
    /*!
     * \brief Останов передачи данных
     */
    void stop() override;

    /*!
     * \brief Возвращает частоту дискретизации для канала по его id
     * \param channelId - идентификатор канала
     */
    int frequency(const QString &channelId) const override;

    /*!
     * \brief Возвращает список uid-ов каналов, передаваемых драйвером по uid протокола
     * \param protocolUid - uid протокола
     */
    QList<QString> getChannelsByProtocol(const QString &protocolUid) const override;

    /*!
     * \brief Возвращает список uid-ов каналов, передаваемых драйвером по uid формата данных
     * \param protocolUid - uid формата данных
     */
    QList<QString> getChannelsByFormat(const QString &formatUid) const override;

    /*!
     * \brief Возвращает список всех каналов, передаваемых драйвером
     */
    QList<QString> getChannels() const override;

    /*!
     * \brief Возвращает кол-во подканалов для канала с заданным uid
     * \param channelUid - uid канала
     */
    int getSubChannelsCount(const QString &channelUid) const override;

    /*!
     * \brief Возвращает true, если канал записывается по умолчанию (установлено в драйвере)
     * \param channelUid - uid канала
     * \param subChan - номер подканала
     */
    bool isChannelRecordingDefault(const QString &channelUid, const int subChan = -1) const override;

    /*!
     * \brief Возвращает интерфейс управления устройством для заданного канала
     * \param controlId - uid интерфейса управления устройством
     * \param channelId - идентификатор канала
     */
    DeviceProtocols::DeviceControl* getDeviceControl(const QString &controlId, const QString &channelId = "") override;

    /*!
     * \brief Возвращает список протоколов
     */
    static QStringList getProtocols();

    /*!
     * \brief Возвращает список допустимых портов для драйвера
     */
    static QList<DeviceProtocols::Ports> getPorts();

    void calibrateTenso(const QString &channelUid) override;
    /*!
     * \brief запрос / установка диапазонов значений для каналов
     * \param chanNumber - номер канала 0 - ...
     * \param min - минимальное значение
     * \param max - максимальное значение
     */
    void getTensoValueDiapasone(const int chanNumber, double &min, double &max) override;
    void getTensoValueDiapasone(const QString channelId, double &min, double &max) override;
    void setTensoValueDiapasone(const int chanNumber, const double min, const double max) override;
    bool isCalibrated() const override;


protected:
    /*!
     * \brief Возвращает настройки порта
     */
    SerialPortDefines::Settings getSerialPortSettings() override;


protected slots:
    void on_readData(const QByteArray data) override;
    void on_error(const QString &err) override;

private:
    /*!
     * \brief Этапы обмена данными с устройством The DataStages enum
     */
    enum DataStage
    {
          dsNone = 0    ///< Ожидание
        , dsInfoRequest ///< Запрос информационного пакета
        , dsSending     ///< Получение данных
    };

    /*!
     * \brief Обрабатывает принятый байт из пакета данных байт
     * \param b - текущий байт
     */
    void assignByteFromDevice(quint8 b);

    /*!
     * \brief Обработка принятого байта при приеме запроса
     * \param b - принятый байт
     */
    void assignByteOnInfoRequest(quint8 b);
    /*!
     * \brief Обработка принятого байта при чтении данных
     * \param b - принятый байт
     */
    void assignByteOnSending(quint8 b);
    /*!
     * \brief Обработка принятого байта в режиме установившейся синхронизации
     * \param b - принятый байт
     */
    void assignByte(quint8 b);

    /*!
     * \brief Устанавливает синхронизацию по данным массива m_synchroBuf
     * \return индекс байта, с которого установлена синхронизация или -1, если не установлена
     */
    int lookingForSynchronize();

    /*!
     * \brief Передача данных пакета
     */
    void sendDataBlock();

    /*!
     * \brief Передача данных тензоканалов
     */
    void sendTensoChannels();

    static QMap<QString, bool> getChanRecordingDefault(const QJsonObject &obj);
    static QJsonObject setChanRecordingDefault(const QMap<QString, bool>& recMap);

    /*!
     * \brief Команда запуска передачи данных
     */
    void startDataSending();
    /*!
     * \brief Команда останова передачи данных
     */
    void stopDataSending();

    /*!
     * \brief Запрос информационного пакета
     */
    void infoRequest();

    ///< Разбор принятых данных
    bool m_isMarker = false;      ///< Счетчик байтов маркера. При первом 0x80 становится true. При втором 0x80 начинается прием пакета. При true и не 0x80 сбрасывается
    bool m_isPackage = false;     ///< true - идет разбор пакета, false - нет разбора пакета
    int m_countBytePack {0};      ///< Счетчик байтов пакета
    int m_countChannels {11};     ///< Кол-во каналов. Должно инициализироваться как параметр драйвера
    quint8 m_prevB;               ///< Первый принятый байт
    double m_t1 {0}, m_t2 {0}, m_t3 {0};                  ///< Значения тензоканалов

    int m_channel {0};
    double m_rkp {0.8};
    int m_pn {500};

//    DeviceProtocols::TensoChannel m_tenso1 {DeviceProtocols::TensoChannel(DeviceProtocols::tdDynHand, 1.7, 100)};
//    DeviceProtocols::TensoChannel m_tenso2 {DeviceProtocols::TensoChannel(DeviceProtocols::tdDynStand, 1.7, 500)};
//    DeviceProtocols::TensoChannel m_tenso3 {DeviceProtocols::TensoChannel(DeviceProtocols::tdBreath, 1.7, 1)};
    double m_tensoPercMin[3] {0, 0, 0};
    double m_tensoPercMax[3] {100, 100, 100};
    bool m_isCalibrated {false};  ///< Было ли откалибровано

    QMap<QString, bool> m_chanRecordingDefault;

    DataStage m_dataStage {dsNone};
    int m_byteCount {0};
    quint8 m_dvcCode {0};      ///< Код устройства
    quint8 m_channelKind[4];   ///< Типы каналов
    double m_channelSK[4];     ///< Коэффициенты чувствительности каналов
    quint32 m_koefParts[4];    ///< Для сборки коэф-та чувствительности
    int m_blockSize {0};       ///< Размер пакета данных от устройства
    QVector<quint8> m_synchroBuf;  ///< Буфер для синхронизации, по длине равен трем длинам пакетов
    bool m_isSynchro {false};      ///< Признак синхронизации

    QVector<quint8> m_blockData;  ///< Байты данных в пакете
    double m_values[4];           ///< Значения сигнала по каналам
};

#endif // MWSTICK_H
