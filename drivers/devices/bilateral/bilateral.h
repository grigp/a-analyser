#ifndef BILATERAL_H
#define BILATERAL_H

#include <QObject>

#include "driver.h"
#include "deviceprotocols.h"
#include "devicesdefines.h"

class Bilateral : public Driver
//        , public DeviceProtocols::StabControl
//        , public DeviceProtocols::TensoControl
//        , public DeviceProtocols::PulseControl
//        , public DeviceProtocols::MyoControl
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

//    void calibrate(const QString &channelUid) override;
//    void zeroing(const QString &channelUid) override;

//    void calibrateTenso(const QString &channelUid) override;
//    /*!
//     * \brief запрос / установка диапазонов значений для каналов
//     * \param chanNumber - номер канала 0 - ...
//     * \param min - минимальное значение
//     * \param max - максимальное значение
//     */
//    void getTensoValueDiapasone(const int chanNumber, double &min, double &max);
//    void getTensoValueDiapasone(const QString channelId, double &min, double &max);
//    void setTensoValueDiapasone(const int chanNumber, const double min, const double max);

//    void setBoundsDelArtifacts(const int low, const int high) override;

//    void zeroingMyo() override;
//    void zeroingMyo(const int channel) override;
//    double amplitudeMyo() override;

private:
    QPoint m_plate1Pos {QPoint(0, 500)};
    QSize m_plate1Size {QSize(500, 500)};
    QPoint m_plate2Pos {QPoint(500, 500)};
    QSize m_plate2Size {QSize(500, 500)};

};

#endif // BILATERAL_H
