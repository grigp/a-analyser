#ifndef AMEDPLATFORM01_H
#define AMEDPLATFORM01_H

#include <QObject>
#include <QCoreApplication>
#include <QThread>
#include <QByteArray>

#include "driver.h"
#include "deviceprotocols.h"
#include "devicesdefines.h"

/*!
 * \brief Класс драйвера стабилоплатформы А-Мед The AMedPlatform01 class
 */
class AMedPlatform01 : public Driver
        , public DeviceProtocols::StabControl
        , public DeviceProtocols::TensoControl
{
public:
    explicit AMedPlatform01(QObject *parent = nullptr);
    ~AMedPlatform01() override;

    static QString uid() {return DevicesDefines::uid_amed_platform01;}
    static QString name() {return tr("Стабилоанализатор А-Мед");} //DevicesDefines::name_amed_platform01);} Не локализуется через константу

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

    void calibrate(const QString &channelUid) override;
    void zeroing(const QString &channelUid) override;

    /*!
     * \brief Возвращает размеры стабилоплатформы
     */
    QSize stabSize() override;

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

    DeviceProtocols::TensoChannel tenso1Params() const {return m_tenso1;}
    DeviceProtocols::TensoChannel tenso2Params() const {return m_tenso2;}
    DeviceProtocols::TensoChannel tenso3Params() const {return m_tenso3;}

private:
    static QMap<QString, bool> getChanRecordingDefault(const QJsonObject &obj);
    static QJsonObject setChanRecordingDefault(const QMap<QString, bool>& recMap);

    int m_countChannels {8};     ///< Кол-во каналов. Должно инициализироваться как параметр драйвера

    DeviceProtocols::TensoChannel m_tenso1 {DeviceProtocols::TensoChannel(DeviceProtocols::tdDynHand, 1.7, 100)};
    DeviceProtocols::TensoChannel m_tenso2 {DeviceProtocols::TensoChannel(DeviceProtocols::tdDynStand, 1.7, 500)};
    DeviceProtocols::TensoChannel m_tenso3 {DeviceProtocols::TensoChannel(DeviceProtocols::tdBreath, 1.7, 1)};

    QMap<QString, bool> m_chanRecordingDefault;
};

#endif // AMEDPLATFORM01_H
