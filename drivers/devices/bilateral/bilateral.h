#ifndef BILATERAL_H
#define BILATERAL_H

#include <QObject>

#include "driver.h"
#include "deviceprotocols.h"
#include "devicesdefines.h"

class Bilateral : public Driver
        , public DeviceProtocols::StabControl
        , public DeviceProtocols::MultiPlatformControl
{
    Q_OBJECT
public:
    explicit Bilateral(QObject *parent = nullptr);
    ~Bilateral() override;

    static QString uid() {return DevicesDefines::uid_bilateral;}
    static QString name() {return tr("Билатеральный стабилограф");} //DevicesDefines::name_bilateral);} Не локализуется через константу

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

    void calibrate(const QString &channelUid) override;
    void zeroing(const QString &channelUid) override;
    QSize stabSize() override;


    /*!
     * \brief Возвращает кол-во платформ
     */
    int platformCount() override {return 2;}

    /*!
     * \brief Возвращает положение и размеры стабилоплатформы num
     */
    QRect platform(const int num) override;

private slots:
    void getData(DeviceProtocols::DeviceData *data);


private:

    /*!
     * \brief Обработка данных стабилограмм
     */
    void assignStabData();

    QRect m_plate1 {QRect(0, 500, 500, 500)};
    QRect m_plate2 {QRect(500, 500, 500, 500)};
    QPoint m_center {QPoint(500, 250)};

    Driver* m_drivers[2];       ///< Драйверы передающие данные
    QString m_chanUid[2];       ///< UID каналов, по которым будет рассчитываться общая СКГ
    DeviceProtocols::StabControl* m_stabControl[2];   ///< Протоколы управления стабилоанализаторами

    ///< Координаты и масса по каналам
    double m_x {0}, m_y{0}, m_a {0}, m_b {0}, m_c {0}, m_d {0};
    double m_x1 {0}, m_y1{0}, m_a1 {0}, m_b1 {0}, m_c1 {0}, m_d1 {0};
    double m_x2 {0}, m_y2{0}, m_a2 {0}, m_b2 {0}, m_c2 {0}, m_d2 {0};
    bool m_isData1 {false}, m_isData2 {false};

    double m_offsetX {0}, m_offsetY {0};
};

#endif // BILATERAL_H