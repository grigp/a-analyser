#ifndef STABILAN01_H
#define STABILAN01_H

#include <QObject>
#include <QCoreApplication>
#include <QThread>
#include <QByteArray>

#include "driver.h"
#include "deviceprotocols.h"
#include "stabilan01defines.h"
#include "devicesdefines.h"



class SerialPort;


/*!
 * \brief Класс драйвера стабилоанализатора Стабилан - 01 Stabilan01 class
 */
class Stabilan01 :
          public Driver
        , public DeviceProtocols::StabControl
        , public DeviceProtocols::TensoControl
        , public DeviceProtocols::PulseControl
        , public DeviceProtocols::MyoControl
{
    Q_OBJECT
public:
    explicit Stabilan01(QObject *parent = nullptr);
    ~Stabilan01() override;

    enum ErrorCodes
    {
        EC_MarkerIinsidePackage = Driver::EC_User + 1

    };

    static QString uid() {return DevicesDefines::uid_stabilan01;}
    static QString name() {return tr("Стабилан - 01");} //DevicesDefines::name_stabilan01);} Не локализуется через константу

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

    void setBoundsDelArtifacts(const int low, const int high) override;

    void zeroingMyo() override;
    void zeroingMyo(const int channel) override;
    double amplitudeMyo() override;


    /*!
     * \brief Возвращает название модели стабилоанализатора по коду
     * \param mdl - код модели
     */
    static QString modelName(const Stabilan01Defines::Model mdlCode);
    static QList<Stabilan01Defines::Model> models();

    static QString zeroingTypeName(const Stabilan01Defines::ZeroingType ztCode);
    static QList<Stabilan01Defines::ZeroingType> zeroingTypes();

    DeviceProtocols::TensoChannel tenso1Params() const {return m_tenso1;}
    DeviceProtocols::TensoChannel tenso2Params() const {return m_tenso2;}
    DeviceProtocols::TensoChannel tenso3Params() const {return m_tenso3;}

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

    /*!
     * \brief Передача данных стабилограммы
     */
    void sendStab();

    /*!
     * \brief Передача данных вариационной пульсометрии
     */
    void sendPulse();

    /*!
     * \brief Передача данных миограммы
     */
    void sendMyogram();

    /*!
     * \brief Передача данных тензоканалов
     */
    void sendTensoChannels();

    static QMap<QString, bool> getChanRecordingDefault(const QJsonObject &obj);
    static QJsonObject setChanRecordingDefault(const QMap<QString, bool>& recMap);

    ///< Разбор принятых данных
    bool m_isMarker = false;      ///< Счетчик байтов маркера. При первом 0x80 становится true. При втором 0x80 начинается прием пакета. При true и не 0x80 сбрасывается
    bool m_isPackage = false;     ///< true - идет разбор пакета, false - нет разбора пакета
    int m_countBytePack {0};      ///< Счетчик байтов пакета
    int m_countChannels {11};     ///< Кол-во каналов. Должно инициализироваться как параметр драйвера
    quint8 m_prevB;               ///< Первый принятый байт
    double m_X, m_Y, m_A, m_B, m_C, m_D, m_Z; ///< Принятые и разобранные данные
    double m_t1, m_t2, m_t3;                  ///< Значения тензоканалов

    ///< Пульс
    quint8 m_rrPor {0};
    bool m_rrMark {false};
    bool m_rrOk {false};
    double m_valPulse {0};

    Stabilan01Defines::Model m_model;
    Stabilan01Defines::ZeroingType m_zt;

    DeviceProtocols::TensoChannel m_tenso1 {DeviceProtocols::TensoChannel(DeviceProtocols::tdDynHand, 1.7, 100)};
    DeviceProtocols::TensoChannel m_tenso2 {DeviceProtocols::TensoChannel(DeviceProtocols::tdDynStand, 1.7, 500)};
    DeviceProtocols::TensoChannel m_tenso3 {DeviceProtocols::TensoChannel(DeviceProtocols::tdBreath, 1.7, 1)};
    double m_tensoPercMin[3] {0, 0, 0};
    double m_tensoPercMax[3] {100, 100, 100};
    bool m_isCalibrated {false};  ///< Было ли откалибровано

    ///< Миограмма
    double m_myoValue[4][4];   ///< Данные миограммы. Первый индекс - номер записи, второй - номер подканала.
    double m_myoAmplitude {2}; ///< Амплитуда миограммы
    double m_myoOffset[4] {0, 0, 0, 0};     ///< Смещения по каналам миограмм

    ///< Точки центровки
    double m_offsetX {0};
    double m_offsetY {0};

    QMap<QString, bool> m_chanRecordingDefault;
};

#endif // STABILAN01_H
