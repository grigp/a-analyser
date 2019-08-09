#ifndef JUMPPLATE_H
#define JUMPPLATE_H

#include <QObject>
#include "driver.h"
#include "deviceprotocols.h"

namespace  {
    static const QString uid_jumpPlate = "{ACC7467D-6600-4808-B372-EDC2069EBF5E}";
    static const QString name_jumpPlate = "Прыжковая платформа";
}

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

    static QString uid() {return uid_jumpPlate;}
    static QString name() {return name_jumpPlate;}

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
     * \brief Возвращает список протоколов
     * \return
     */
    static QStringList getProtocols();

    /*!
     * \brief Возвращает список допустимых портов для драйвера
     */
    static QList<DeviceProtocols::Ports> getPorts();

    void calibrate() override;

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
