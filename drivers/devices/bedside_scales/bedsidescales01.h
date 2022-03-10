#ifndef BEDSIDESCALES01_H
#define BEDSIDESCALES01_H

#include <QObject>
#include <QCoreApplication>
#include <QThread>
#include <QByteArray>

#include "driver.h"
#include "deviceprotocols.h"
#include "devicesdefines.h"


/*!  Протокол передачи данных
 * [ 01 | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 10 | 11 | 12 | 13 | 14 ]
 * [ ff | 03 | cc | Lo1| Hi1| Lo2| Hi2| Lo3| Hi3| Lo4| Hi4| CRC| ff | ff ]
 * ff - байт разделитель
 * 03 - сетевой адрес устройства
 * cc - код операции
 * CRC - контрольная сумма
 */


/*!
 * \brief Класс драйвера прикроватных весов The BedsideScales01 class
 */
class BedsideScales01 :
          public Driver
        , public DeviceProtocols::TensoControl

{
    Q_OBJECT
public:
    explicit BedsideScales01(QObject *parent = nullptr);

    static QString uid() {return DevicesDefines::uid_bedsidescales01;}
    static QString name() {return tr("Прикроватные весы");} //DevicesDefines::name_bedsidescales01);} Не локализуется через константу

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

    /*!
     * \brief Передача данных пакета
     */
    void sendDataBlock();

    int m_synchro {5};           ///< Счетчик байтов синхронизации 5 -> 0 0xff 0xff 0xff 0x03 0xcc
    int m_dataByteCount {0};     ///< Счетчик байтов пакета. Должно быть 8
    quint8 m_lo {0};             ///< Младший байт
    double m_values[4];          ///< Принятый пакет данных
    quint8 m_bPrev {0};          ///< Предыдущий байт
    quint8 m_circleCnt {0xFF};   ///< Кольцевой счетчик пакетов
    quint8 m_crc {0};            ///< Контрольная сумма
    int m_errorCount {0};        ///< Количество ошибок по результатам анализа колцевого счетчика
};

#endif // BEDSIDESCALES01_H
