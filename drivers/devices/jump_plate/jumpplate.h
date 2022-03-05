#ifndef JUMPPLATE_H
#define JUMPPLATE_H

#include <QObject>
#include <QCoreApplication>
#include "driver.h"
#include "deviceprotocols.h"
#include "devicesdefines.h"

/*!
 * \brief Класс драйвера прыжковой платформы JumpPlate class
 */
class JumpPlate : public Driver, public DeviceProtocols::JumpPlateControl
{
    Q_OBJECT
public:
    explicit JumpPlate(QObject *parent = nullptr);

    enum ErrorCodes
    {
        EC_MarkerIinsidePackage = Driver::EC_User + 1

    };

    static QString uid() {return DevicesDefines::uid_jumpPlate;}
    static QString name() {return tr("Прыжковая платформа");} //DevicesDefines::name_jumpPlate;}  Не локализуется через константу

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
     */
    bool isChannelRecordingDefault(const QString &channelUid) const override;

    /*!
     * \brief Возвращает список протоколов
     * \return
     */
    static QStringList getProtocols();

    /*!
     * \brief Возвращает список допустимых портов для драйвера
     */
    static QList<DeviceProtocols::Ports> getPorts();

    void calibrate() override;

    /*!
     * \brief Возвращает кол-во платформ
     */
    int platformsCount() override;

    /*!
     * \brief Возвращает состояние платформы (true - загружена, false - пустая)
     * \param pltNum - номер платформы
     */
    bool platformState(const int pltNum) const override;

    /*!
     * \brief Возвращает время платформы
     * \param pltNum - номер платформы
     */
    double platformTime(const int pltNum) const override;


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
     * \brief Обрабатывает принятый байт из пакета данных байт
     * \param b - текущий байт
     */
    void assignByteFromDevice(quint8 b);

    int m_platformCount = 1;      ///< Кол-во платформ

    ///< разбор принятых данных
    bool m_isMarker {false};      ///< Счетчик байтов маркера. При первом 0x80 становится true. При втором 0x80 начинается прием пакета. При true и не 0x80 сбрасывается
    bool m_isPackage {false};     ///< true - идет разбор пакета, false - нет разбора пакета
    int m_countBytePack {0};      ///< Счетчик байтов пакета
    int m_countChannels {4};      ///< Кол-во каналов. Должно инициализироваться как параметр драйвера
    quint8 m_byte0 {0};           ///< Первый принятый байт
    quint8 m_byte1 {0};           ///< Второй принятый байт
    quint8 m_byte2 {0};           ///< Третий принятый байт

    double m_conPlate1 {0};       ///< Констаната пересчета для платформы 1
    double m_conPlate2 {0};       ///< Констаната пересчета для платформы 2
    bool m_flagPlate1 {0};        ///< Флаг нажатия первой платформы
    unsigned int m_counterPlate1 {0};  ///< Счетчик времени первой платформы
    bool m_flagPlate2 {0};        ///< Флаг нажатия второй платформы
    unsigned int m_counterPlate2 {0};  ///< Счетчик времени второй платформы

    bool m_busyPlate1 {false};    ///< Флаг "Занятия" платформы 1
    double m_timePlate1 {0};      ///< Время занятия / свободного состояния платформы 1
    bool m_busyPlate2 {false};    ///< Флаг "Занятия" платформы 2
    double m_timePlate2 {0};      ///< Время занятия / свободного состояния платформы 2
};

#endif // JUMPPLATE_H
