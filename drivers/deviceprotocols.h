#ifndef DEVICEPROTOCOLS_H
#define DEVICEPROTOCOLS_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QVector>

#include "channelsdefines.h"

class Driver;

namespace DeviceProtocols {

///<------------------------------------------------------------------------------------------
///< Порты

enum Ports {
      pcNone = -1
    , pcUsb = 0
    , pcCom1, pcCom2, pcCom3, pcCom4, pcCom5, pcCom6, pcCom7, pcCom8, pcCom9, pcCom10
    , pcCom11, pcCom12, pcCom13, pcCom14, pcCom15, pcCom16, pcCom17, pcCom18, pcCom19, pcCom20
    , pcCom21, pcCom22, pcCom23, pcCom24, pcCom25, pcCom26, pcCom27, pcCom28, pcCom29, pcCom30
    , pcCom31, pcCom32, pcCom33, pcCom34, pcCom35, pcCom36, pcCom37, pcCom38, pcCom39, pcCom40
    , pcCom41, pcCom42, pcCom43, pcCom44, pcCom45, pcCom46, pcCom47, pcCom48, pcCom49, pcCom50
    , pcCom51, pcCom52, pcCom53, pcCom54, pcCom55, pcCom56, pcCom57, pcCom58, pcCom59, pcCom60
    , pcCom61, pcCom62, pcCom63, pcCom64, pcCom65, pcCom66, pcCom67, pcCom68, pcCom69, pcCom70
    , pcCom71, pcCom72, pcCom73, pcCom74, pcCom75, pcCom76, pcCom77, pcCom78, pcCom79, pcCom80
    , pcCom81, pcCom82, pcCom83, pcCom84, pcCom85, pcCom86, pcCom87, pcCom88, pcCom89, pcCom90
    , pcCom91, pcCom92, pcCom93, pcCom94, pcCom95, pcCom96, pcCom97, pcCom98, pcCom99, pcCom100
    , pcCom101, pcCom102, pcCom103, pcCom104, pcCom105, pcCom106, pcCom107, pcCom108, pcCom109, pcCom110
    , pcCom111, pcCom112, pcCom113, pcCom114, pcCom115, pcCom116, pcCom117, pcCom118, pcCom119, pcCom120
    , pcCom121, pcCom122, pcCom123, pcCom124, pcCom125, pcCom126, pcCom127
};

/*!
 * \brief Возвращает имя порта для показа в списке в формате COM <номер>
 */
QString portName(const Ports port);

/*!
 * \brief Возвращает имя порта для использования в QSerialPort в формате COM<номер>
 */
QString serialPortName(const Ports port);

QList<Ports> comPorts();
QList<Ports> comUsbPorts();
QList<Ports> allPorts();


/*!
 * \brief Тензометрические устройства TensoDevice enum
 */
enum TensoDevice
{
      tdDynHand = 0 ///< Кистевой силомер
    , tdDynStand    ///< Становой силомер
    , tdBreath      ///< Периметрическое дыхание
    , tdPushDevice  ///< Толкатель
};

/*!
 * \brief Параметры тензометрического канала TensoChannel struct
 */
struct TensoChannel
{
    TensoDevice device;
    double rkp;
    double pn;
    TensoChannel() {}
    TensoChannel(TensoDevice dvc, double r, int p)
        : device(dvc), rkp(r), pn(p) {}
};

///<------------------------------------------------------------------------------------------
///< Данные от устройств

static const QString uid_StabDvcData = "{69941064-D628-4A6F-A415-0EA12156D237}";
static const QString uid_DynDvcData = "{EB0F8C35-048E-412C-B7B9-26C0479FEC6E}";
static const QString uid_BreathDvcData = "{C030543B-A417-4FDB-94E8-547557373D22}";
static const QString uid_PulseDvcData = "{7311EDCB-9E7D-4D87-9A5C-F382C85C4F1B}";
static const QString uid_MyoDvcData = "{4F170074-1F17-4214-9BA9-BA53C732BF5F}";
static const QString uid_JumpPlateDvcData = "{F7EF6F97-3502-4EC0-A798-68DDF40DFB7F}";
static const QString uid_JumpPlateBlockData = "{A3CE22EA-1D4D-4C3F-8E61-7FED58C7044B}";
static const QString uid_WeightPlateDvcData = "{CB8129C1-D44A-4116-8FA3-824AC8C11E67}";
static const QString uid_ADCValuesDvcData = "{FFBBD740-245C-4FE4-B9B1-D3E63DE4F3DB}";

static const QString name_StabDvcData = "Данные стабилографии";
static const QString name_DynDvcData = "Данные динамометрии";
static const QString name_BreathDvcData = "Данные периметрического дыхания";
static const QString name_PulseDvcData = "Данные вариационной пульсометрии";
static const QString name_MyoDvcData = "Данные миографии";
static const QString name_JumpPlateDvcData = "Данные прыжковой платформы";
static const QString name_JumpPlateBlockData = "Отсчеты прыжковой платформы";
static const QString name_WeightPlateDvcData = "Данные весовой платформы";
static const QString name_ADCValuesDvcData = "Данные АЦП";


/*!
 * \brief Базовый класс данных от устройств DeviceData class
 */
class DeviceData
{
public:
    explicit DeviceData(Driver* sender, const QString &channelId)
        : m_sender(sender), m_channelId(channelId) {}

    virtual ~DeviceData(){}

    virtual QString uid() const = 0; // {return QString("");}
//    virtual QString name() const = 0; // {return QString("");}  Непонятно, как быть с локализацией

    Driver* sender() {return m_sender;}
    QString channelId() {return m_channelId;}

    //!
    //! \brief Изменение идентификатора канала.
    //! Использовать в билатеральном режиме для изменения id каналов латеральных платформ
    //! \param channelID - новый id канала
    //!
    void changeCahnnelId(const QString channelID) {m_channelId = channelID;}

private:
    Driver* m_sender;
    QString m_channelId;
};

/*!
 * \brief Класс данных от устройств с универсальным доступом к подканалам MultiData class
 * Предполагается для использования в тестах, которые получают данные по формату и не знают о сути каналов
 */
class MultiData : public DeviceData
{
public:
    explicit MultiData(Driver* sender, const QString &channelId)
        : DeviceData(sender, channelId){}

    /*!
     * \brief Возвращает кол-во подканалов
     */
    virtual int subChanCount() const = 0;

    /*!
     * \brief Возвращает значение подканала по индексу
     */
    virtual QVariant value(const int idx) const = 0;
};

/*!
 * \brief Класс данных стабилографии, получаемых от устройств StabDvcData class
 */
class StabDvcData : public MultiData
{
public:
    StabDvcData(Driver *sender, const QString &channelId, double x, double y)
        : MultiData(sender, channelId)
        , m_x(x), m_y(y) {m_z = 0; m_a = 0; m_b = 0; m_c = 0, m_d = 0;}

    StabDvcData(Driver *sender, const QString &channelId, double x, double y, double z)
        : MultiData(sender, channelId)
        , m_x(x), m_y(y), m_z(z) {m_a = 0; m_b = 0; m_c = 0, m_d = 0;}

    StabDvcData(Driver *sender, const QString &channelId, double x, double y, double a, double b, double c)
        : MultiData(sender, channelId)
        , m_x(x), m_y(y), m_a(a), m_b(b), m_c(c) {m_z = m_a + m_b + m_c; m_d = 0;}

    StabDvcData(Driver *sender, const QString &channelId, double x, double y, double a, double b, double c, double d)
        : MultiData(sender, channelId)
        , m_x(x), m_y(y), m_a(a), m_b(b), m_c(c), m_d(d) {m_z = m_a + m_b + m_c + m_d;}

//    StabDvcData(const StabDvcData &obj)
//        : DeviceData(obj.sender())
//        , m_x(obj.x()), m_y(obj.y()), m_z(obj.z()),
//          m_a(obj.a()), m_b(obj.b()), m_c(obj.c()), m_d(obj.d())
//    {}
    ~StabDvcData(){}

    double x() const {return m_x;}
    double y() const {return m_y;}
    double z() const {return m_z;}

    double a() const {return m_a;}
    double b() const {return m_b;}
    double c() const {return m_c;}
    double d() const {return m_d;}

    int subChanCount() const override {return 1;} //3;}

    QVariant value(const int idx) const override
    {
        Q_UNUSED(idx);
        QPointF rec(m_x, m_y);
        return QVariant::fromValue(rec);

//        switch (idx) {
//        case 0: return m_x;
//        case 1: return m_y;
//        case 3: return m_z;
//        default: return 0;
//        }
    }

    QString uid() const override {return uid_StabDvcData;}
//    QString name() const override {return name_StabDvcData;} Непонятно, как быть с локализацией

private:
  double m_x, m_y, m_z;
  double m_a, m_b, m_c, m_d;
};

/*!
 * \brief Класс данных тензоканала (динамометрии или дыхания), получаемых от устройств TensoDvcData class
 */
class TensoDvcData : public MultiData
{
public:
    TensoDvcData(Driver *sender, const QString &channelId, const double &value)
        : MultiData(sender, channelId), m_value(value) {}

    int subChanCount() const override {return 1;}
    QVariant value(const int idx) const override {Q_UNUSED(idx); return m_value;}

    QString uid() const override {return uid_DynDvcData;}

private:
    double m_value;
};

/*!
 * \brief Класс данных весовой платформы, получаемых от устройств TensoMultiDvcData class
 */
class WeightPlateDvcData : public MultiData
{
public:
    WeightPlateDvcData(Driver *sender, const QString &channelId, const QVector<double> &value)
        : MultiData(sender, channelId), m_value(value) {}

    int subChanCount() const override {return m_value.size();}
    QVariant value(const int idx) const override {return m_value.at(idx);}

    QString uid() const override {return uid_WeightPlateDvcData;}

private:
    QVector<double> m_value;
};

/*!
 * \brief Класс данных отсчетов АЦП, получаемых от устройств ADCValuesDvcData class
 */
class ADCValuesDvcData : public MultiData
{
public:
    ADCValuesDvcData(Driver *sender, const QString &channelId, const QVector<quint16> &value)
        : MultiData(sender, channelId), m_value(value) {}

    int subChanCount() const override {return m_value.size();}
    QVariant value(const int idx) const override {return m_value.at(idx);}

    QString uid() const override {return uid_ADCValuesDvcData;}

private:
    QVector<quint16> m_value;
};
/*!
 * \brief Класс данных динамометрии по нескольким каналам, получаемых от устройств DynamoComboDvcData class
 */
//class DynamoComboDvcData : public MultiData
//{
//public:
//    DynamoComboDvcData(Driver *sender, const QString &channelId, const QVector<double> &data)
//        : MultiData(sender, channelId), m_data(data) {}

//    int subChanCount() const override {return m_data.size();}
//    QVariant value(const int idx) const override
//    {
//        Q_ASSERT(idx >= 0 && idx < m_data.size());
//        return m_data.at(idx);
//    }

//private:
//    QVector<double> m_data;
//};

/*!
 * \brief Класс данных вариационной пульсометрии, получаемых от устройств PulseDvcData class
 */
class PulseDvcData : public MultiData
{
public:
    PulseDvcData(Driver *sender, const QString &channelId, const double &value)
        : MultiData(sender, channelId), m_value(value) {}

    int subChanCount() const override {return 1;}
    QVariant value(const int idx) const override {Q_UNUSED(idx); return m_value;}

    QString uid() const override {return uid_PulseDvcData;}

private:
    double m_value;
};

/*!
 * \brief Класс данных миограммы, получаемых от устройств MyogramDvcData class
 */
class MyogramDvcData : public MultiData
{
public:
    MyogramDvcData(Driver *sender, const QString &channelId, const QVector<double> &data)
        : MultiData(sender, channelId), m_data(data) {}

    int subChanCount() const override {return m_data.size();}
    QVariant value(const int idx) const override
    {
        Q_ASSERT(idx >= 0 && idx < m_data.size());
        return m_data.at(idx);
    }

    QString uid() const override {return uid_MyoDvcData;}

private:
    QVector<double> m_data;
};

/*!
 * \brief Класс данных прыжковой платформы, получаемых от устройств JumpPlateDvcData class
 */
class JumpPlateDvcData : public MultiData
{
public:
    JumpPlateDvcData(Driver *sender, const QString &channelId, int plate, bool busy, double time)
        : MultiData(sender, channelId), m_plate(plate), m_busy(busy), m_time(time) {}

    int plate() const {return m_plate;}
    bool busy() const {return m_busy;}
    double time() const {return m_time;}

    QString uid() const override {return uid_JumpPlateDvcData;}
//    QString name() const override {return name_JumpPlateDvcData;}  Непонятно, как быть с локализацией

    int subChanCount() const override {return 0;}
    QVariant value(const int idx) const override {Q_UNUSED(idx); return 0;}

private:
    int m_plate;
    bool m_busy;
    double m_time;
};

/*!
 * \brief Класс данных отсчетов прыжковой платформы, получаемых от устройств JumpPlateBlockData class
 */
class JumpPlateBlockData : public MultiData
{
public:
    JumpPlateBlockData(Driver *sender, const QString &channelId, int blockCnt,
                       bool busy1, double counter1, double con1,
                       bool busy2, double counter2, double con2)
        : MultiData(sender, channelId), m_blockCnt(blockCnt)
        , m_busy1(busy1), m_counter1(counter1), m_con1(con1)
        , m_busy2(busy2), m_counter2(counter2), m_con2(con2)
    {}

    QString uid() const override {return uid_JumpPlateBlockData;}
//    QString name() const override {return name_JumpPlateBlockData;}

    int blockCnt() const {return m_blockCnt;}

    bool busy1() const {return m_busy1;}
    double counter1() const {return m_counter1;}
    double con1() const {return m_con1;}

    bool busy2() const {return m_busy2;}
    double counter2() const {return m_counter2;}
    double con2() const {return m_con2;}

    int subChanCount() const override {return 0;}
    QVariant value(const int idx) const override {Q_UNUSED(idx); return 0;}

private:
    int m_blockCnt;
    bool m_busy1;
    double m_counter1;
    double m_con1;
    bool m_busy2;
    double m_counter2;
    double m_con2;

};

///<------------------------------------------------------------------------------------------
///< Интерфейсы управления устройствами

static const QString uid_CommonControl = "{4EBD7247-9F63-4722-82C3-0FE86ADB276F}";
static const QString uid_StabControl = "{D9498794-C63E-426D-8BB5-09B3CE1EADD8}";
static const QString uid_TensoControl = "{2D6BC458-8C8D-4125-AF3C-9F7352AED8F5}";
//static const QString uid_BreathControl = "{CE69FF3F-5A95-43E6-A3A2-2505CD39A636}";
static const QString uid_PulseControl = "{8A6607B9-42FB-4832-AAE8-B83506A08B18}";
static const QString uid_MyoControl = "{5C26B27F-0FA6-478B-9384-E8B6FA403F84}";
static const QString uid_JumpPlateControl = "{F7EF6F97-3502-4EC0-A798-68DDF40DFB7F}";
static const QString uid_WeightPlateControl = "{7B49EF56-B831-401D-AB89-73337FF0EC4B}";
static const QString uid_MultiPlatformControl = "{15A2CE55-F43D-43FA-8116-35A1A988E21F}";


static const QString name_CommonControl = "Управление устройством общий";
static const QString name_StabControl = "Управление стабилоплатформой";
static const QString name_TensoControl = "Управление тензоканалами";
//static const QString name_BreathControl = "Управление периметрическим дыханием";
static const QString name_PulseControl = "Управление пульсометрией";
static const QString name_MyoControl = "Управление миограммой";
static const QString name_JumpPlateControl = "Управление прыжковой платформой";
static const QString name_WeightPlateControl = "Управление весовой платформой";
static const QString name_MultiPlatformControl = "Управление несколькими платформами";

/*!
 * \brief Базовый класс интерфейсов управления устройствами DeviceControl class
 */
class DeviceControl
{
public:
    explicit DeviceControl() {}
    ~DeviceControl() {}

//    virtual QString uid() const = 0;
//    virtual QString name() const = 0;
};


/*!
 * \brief Класс общего интерфейса управления устройствами CommonControl class
 */
class CommonControl : public DeviceControl
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;

    static QString uid() {return uid_CommonControl;}

//    static QString name() {return name_CommonControl;}   Непонятно, как быть с локализацией

    ~CommonControl() {}
};

/*!
 * \brief Класс управления данными, представляющими собой декартовы координаты DecartCoordControl class
 */
class DecartCoordControl : public DeviceControl
{
public:
    virtual void zeroing(const QString &channelUid) = 0;

    /*!
     * \brief Возвращает размеры стабилоплатформы
     */
    virtual QSize stabSize() = 0;

    ~DecartCoordControl() {}
};

/*!
 * \brief Класс управления данными, представляющими собой линейный канал LinearChanControl class
 */
class LinearChanControl : public DeviceControl
{
public:
    ~LinearChanControl() {}
};

/*!
 * \brief Класс управления устройствами данными стабилографии StabControl class
 */
class StabControl : public DecartCoordControl
{
public:
    virtual void calibrate(const QString &channelUid) = 0;

    static QString uid() {return uid_StabControl;}
//    static QString name() {return name_StabControl;}         Непонятно, как быть с локализацией

    ~StabControl() {}
};

/*!
 * \brief Класс управления устройствами данными динамометрии TensoControl class
 */
class TensoControl : public LinearChanControl
{
public:
    virtual void calibrateTenso(const QString &channelUid) = 0;

    /*!
     * \brief запрос / установка диапазонов значений для каналов
     * \param chanNumber - номер канала 0 - ...
     * \param min - минимальное значение
     * \param max - максимальное значение
     */
    virtual void getTensoValueDiapasone(const int chanNumber, double &min, double &max) = 0;
    virtual void getTensoValueDiapasone(const QString channelId, double &min, double &max) = 0;
    virtual void setTensoValueDiapasone(const int chanNumber, const double min, const double max) = 0;

    static QString uid() {return uid_TensoControl;}

    ~TensoControl() {}
};

///*!
// * \brief Класс управления устройствами данными динамометрии BreathControl class
// */
//class BreathControl : public LinearChanControl
//{
//public:
//    virtual void calibrate(const QString &channelUid) = 0;

//    static QString uid() {return uid_BreathControl;}

//    ~BreathControl() {}
//};

/*!
 * \brief Класс управления устройствами данными динамометрии PulseControl class
 */
class PulseControl : public LinearChanControl
{
public:
    virtual void setBoundsDelArtifacts(const int low, const int high) = 0;

    static QString uid() {return uid_PulseControl;}

    ~PulseControl() {}
};

/*!
 * \brief Класс управления устройствами данными миографии MyoControl class
 */
class MyoControl : public LinearChanControl
{
public:
    virtual void zeroingMyo() = 0;
    virtual void zeroingMyo(const int channel) = 0;
    virtual double amplitudeMyo() = 0;

    static QString uid() {return uid_MyoControl;}

    ~MyoControl() {}
};

/*!
 * \brief Класс управления устройствами данными прыжковой платформы JumpPlateControl class
 */
class JumpPlateControl : public DeviceControl
{
public:
    virtual void calibrate() = 0;

    static QString uid() {return uid_JumpPlateControl;}
//    static QString name() {return name_JumpPlateControl;}          Непонятно, как быть с локализацией

    /*!
     * \brief Возвращает кол-во платформ
     */
    virtual int platformsCount() = 0;

    /*!
     * \brief Возвращает состояние платформы (true - загружена, false - пустая)
     * \param pltNum - номер платформы
     */
    virtual bool platformState(const int pltNum) const = 0;

    /*!
     * \brief Возвращает время платформы
     * \param pltNum - номер платформы
     */
    virtual double platformTime(const int pltNum) const = 0;

    ~JumpPlateControl() {}
};

/*!
 * \brief Класс управления драйвером из нескольких платформ MultiPlatformControl class
 */
class MultiPlatformControl : public DeviceControl
{
public:

    /*!
     * \brief Возвращает кол-во платформ
     */
    virtual int platformCount() = 0;

    /*!
     * \brief Возвращает размеры стабилоплатформы num
     */
    virtual QRect platform(const int num) = 0;

    ~MultiPlatformControl() {}
};



///<------------------------------------------------------------------------------------------
///< Протоколы

struct DeviceProtocol
{
    QString uid;
    QString name;
    QString uidDeviceControl;
    QString uidDeviceData;
    DeviceProtocol() {}
    DeviceProtocol(std::tuple<QString, QString, QString, QString> params)
        : uid(std::get<0>(params))
        , name(std::get<1>(params))
        , uidDeviceControl(std::get<2>(params))
        , uidDeviceData(std::get<3>(params))
    {}
};

static const QString uid_StabProtocol = "{3F67F2E4-CA04-4934-A942-CBB827DF004F}";
static const QString uid_DynProtocol = "{36C4809F-AB3F-4C98-9737-7561757A3994}";
static const QString uid_PulseProtocol = "{B15928FE-E0F9-491C-9B82-1F73B8A7493D}";
//static const QString uid_BreathProtocol = "{B2C83763-15ED-426E-B6AF-1ADD79AC28FF}";
static const QString uid_MyoProtocol = "{DE208F93-8515-49BA-83F1-916EC215E99C}";
static const QString uid_JumpPlateProtocol = "{7E31AE8E-96AB-4E34-96DA-54E19B312377}";
static const QString uid_WeightPlateProtocol = "{8D6F194F-2E21-4ADF-9989-AEFF69374D4B}";
static const QString uid_ADCValuesProtocol = "{ED55ED10-BD5E-4BB6-9CA8-59A04FAE8167}";

static const QString name_StabProtocol = "Протокол стабилографии";
static const QString name_DynProtocol = "Протокол динамометрии";
static const QString name_PulseProtocol = "Протокол пульсометрии";
static const QString name_BreathProtocol = "Протокол периметрического дыхания";
static const QString name_MyoProtocol = "Протокол миографии";
static const QString name_JumpPlateProtocol = "Протокол прыжковой платформы";
static const QString name_WeightPlateProtocol = "Протокол весовой платформы";
static const QString name_ADCValuesProtocol = "Протокол дискрет АЦП";

static QList<DeviceProtocol> protocols = QList<DeviceProtocol>()
        << DeviceProtocol(std::make_tuple(uid_StabProtocol, name_StabProtocol, uid_StabControl, uid_StabDvcData))
        << DeviceProtocol(std::make_tuple(uid_DynProtocol, name_DynProtocol, uid_TensoControl, uid_DynDvcData))
//        << DeviceProtocol(std::make_tuple(uid_BreathProtocol, name_BreathProtocol, uid_BreathControl, uid_BreathDvcData))
        << DeviceProtocol(std::make_tuple(uid_PulseProtocol, name_PulseProtocol, uid_PulseControl, uid_PulseDvcData))
        << DeviceProtocol(std::make_tuple(uid_MyoProtocol, name_MyoProtocol, uid_MyoControl, uid_MyoDvcData))
        << DeviceProtocol(std::make_tuple(uid_JumpPlateProtocol, name_JumpPlateProtocol, uid_JumpPlateControl, uid_JumpPlateDvcData))
        << DeviceProtocol(std::make_tuple(uid_WeightPlateProtocol, name_WeightPlateProtocol, uid_WeightPlateControl, uid_WeightPlateDvcData))
        << DeviceProtocol(std::make_tuple(uid_ADCValuesProtocol, name_ADCValuesProtocol, uid_WeightPlateControl, uid_ADCValuesDvcData));


///<------------------------------------------------------------------------------------------
///< Сервис

///< Получение названия протокола по uid
static QMap<QString, QString> protocolName {
    std::pair<QString, QString> (uid_StabProtocol, name_StabProtocol)
  , std::pair<QString, QString> (uid_DynProtocol, name_DynProtocol)
//  , std::pair<QString, QString> (uid_BreathProtocol, name_BreathProtocol)
  , std::pair<QString, QString> (uid_PulseProtocol, name_PulseProtocol)
  , std::pair<QString, QString> (uid_MyoProtocol, name_MyoProtocol)
  , std::pair<QString, QString> (uid_JumpPlateProtocol, name_JumpPlateProtocol)
  , std::pair<QString, QString> (uid_WeightPlateProtocol, name_WeightPlateProtocol)
};

///< Получение названия интерфейса управления по uid
static QMap<QString, QString> controlName {
    std::pair<QString, QString> (uid_CommonControl, name_CommonControl)
  , std::pair<QString, QString> (uid_StabControl, name_StabControl)
  , std::pair<QString, QString> (uid_TensoControl, name_TensoControl)
//  , std::pair<QString, QString> (uid_BreathControl, name_BreathControl)
  , std::pair<QString, QString> (uid_PulseProtocol, name_PulseControl)
  , std::pair<QString, QString> (uid_MyoControl, name_MyoControl)
  , std::pair<QString, QString> (uid_JumpPlateControl, name_JumpPlateControl)
  , std::pair<QString, QString> (uid_WeightPlateControl, name_WeightPlateControl)
};

///< Получение названия данных от устройства по uid
static QMap<QString, QString> deviceDataName {
    std::pair<QString, QString> (uid_StabDvcData, name_StabDvcData)
  , std::pair<QString, QString> (uid_DynDvcData, name_DynDvcData)
  , std::pair<QString, QString> (uid_BreathDvcData, name_BreathDvcData)
  , std::pair<QString, QString> (uid_PulseDvcData, name_PulseDvcData)
  , std::pair<QString, QString> (uid_MyoDvcData, name_MyoDvcData)
  , std::pair<QString, QString> (uid_JumpPlateDvcData, name_JumpPlateDvcData)
  , std::pair<QString, QString> (uid_JumpPlateBlockData, name_JumpPlateBlockData)
  , std::pair<QString, QString> (uid_WeightPlateDvcData, name_WeightPlateDvcData)
  , std::pair<QString, QString> (uid_ADCValuesDvcData, name_ADCValuesDvcData)
};

///< Соответствие протокола и формата канала по uid
static QMap<QString, QString> protocolFormat {
    std::pair<QString, QString> (uid_StabProtocol, ChannelsDefines::cfDecartCoordinates)
  , std::pair<QString, QString> (uid_DynProtocol, ChannelsDefines::cfSinglePositive)
//  , std::pair<QString, QString> (uid_BreathProtocol, ChannelsDefines::cfSingleDual)
  , std::pair<QString, QString> (uid_PulseProtocol, ChannelsDefines::cfSinglePositive)
  , std::pair<QString, QString> (uid_MyoProtocol, ChannelsDefines::cfSinglePositive)
  , std::pair<QString, QString> (uid_JumpPlateProtocol, ChannelsDefines::cfNoSignal)
  , std::pair<QString, QString> (uid_WeightPlateProtocol, ChannelsDefines::cfSinglePositive)
};

///< Устройства тензометрические
static const QString name_DynHand = "Кистевой силомер";
static const QString name_DynStand = "Становой силомер";
static const QString name_Breath = "Периметрическое дыхание";
static const QString name_PushDevice = "Толкатель";

static QMap<TensoDevice, QString> tensoDevices {
    std::pair<TensoDevice, QString> (tdDynHand, name_DynHand)
  , std::pair<TensoDevice, QString> (tdDynStand, name_DynStand)
  , std::pair<TensoDevice, QString> (tdBreath, name_Breath)
  , std::pair<TensoDevice, QString> (tdPushDevice, name_PushDevice)
};

}

Q_DECLARE_METATYPE(QList<DeviceProtocols::Ports>)



#endif // DEVICEPROTOCOLS_H
