#ifndef STABILAN01_H
#define STABILAN01_H

#include <QObject>
#include <QCoreApplication>
#include <QThread>
#include <QByteArray>

#include "driver.h"
#include "deviceprotocols.h"
#include "stabilan01defines.h"

namespace  {
    static const QString uid_stabilan01 = "{CEAD1669-1531-4B8E-9220-590C49BB310D}";
    static const QString name_stabilan01 = QCoreApplication::tr("Стабилан - 01");
}


class SerialPort;


/*!
 * \brief Класс драйвера стабилоанализатора Стабилан - 01 Stabilan01 class
 */
class Stabilan01 : public Driver, public DeviceProtocols::StabControl
{
    Q_OBJECT
public:
    explicit Stabilan01(QObject *parent = nullptr);

    enum ErrorCodes
    {
        EC_MarkerIinsidePackage = Driver::EC_User + 1

    };

    static QString uid() {return uid_stabilan01;}
    static QString name() {return tr("Стабилан - 01");} //name_stabilan01);} Не локализуется через константу

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
     */
    static QStringList getProtocols();

    /*!
     * \brief Возвращает список допустимых портов для драйвера
     */
    static QList<DeviceProtocols::Ports> getPorts();

    void calibrate() override;
    void zeroing() override;

    /*!
     * \brief Возвращает название модели стабилоанализатора по коду
     * \param mdl - код модели
     */
    static QString modelName(const Stabilan01Defines::Model mdlCode);
    static QList<Stabilan01Defines::Model> models();

    static QString zeroingTypeName(const Stabilan01Defines::ZeroingType ztCode);
    static QList<Stabilan01Defines::ZeroingType> zeroingTypes();

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

    // разбор принятых данных
    bool m_isMarker = false;      // Счетчик байтов маркера. При первом 0x80 становится true. При втором 0x80 начинается прием пакета. При true и не 0x80 сбрасывается
    bool m_isPackage = false;     // true - идет разбор пакета, false - нет разбора пакета
    int m_countBytePack = 0;      // Счетчик байтов пакета
    int m_countChannels = 11;     // Кол-во каналов. Должно инициализироваться как параметр драйвера
    quint8 m_prevB;               // Первый принятый байт
    double m_X, m_Y, m_A, m_B, m_C, m_D, m_Z; // Принятые и разобранные данные

    Stabilan01Defines::Model m_model;
    Stabilan01Defines::ZeroingType m_zt;

    ///< Точки центровки
    double m_offsetX {0};
    double m_offsetY {0};

};

#endif // STABILAN01_H
