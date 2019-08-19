#ifndef DEVICEPROTOCOLS_H
#define DEVICEPROTOCOLS_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QVector>

class Driver;

namespace DeviceProtocols {

///<------------------------------------------------------------------------------------------
///< Порты

enum Ports {
      pcEmulation = -1
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


///<------------------------------------------------------------------------------------------
///< Данные от устройств

static const QString uid_StabDvcData = "{69941064-D628-4A6F-A415-0EA12156D237}";
static const QString uid_DynDvcData = "{EB0F8C35-048E-412C-B7B9-26C0479FEC6E}";
static const QString uid_JumpPlateDvcData = "{F7EF6F97-3502-4EC0-A798-68DDF40DFB7F}";
static const QString uid_JumpPlateBlockData = "{A3CE22EA-1D4D-4C3F-8E61-7FED58C7044B}";

static const QString name_StabDvcData = "Данные стабилографии";
static const QString name_DynDvcData = "Данные динамометрии";
static const QString name_JumpPlateDvcData = "Данные прыжковой платформы";
static const QString name_JumpPlateBlockData = "Отсчеты прыжковой платформы";


/*!
 * \brief Базовый класс данных от устройств DeviceData class
 */
class DeviceData
{
public:
    explicit DeviceData(Driver* sender)
        : m_sender(sender) {}

    virtual ~DeviceData(){}

    virtual QString uid() const = 0; // {return QString("");}
//    virtual QString name() const = 0; // {return QString("");}  Непонятно, как быть с локализацией

    Driver* sender() {return m_sender;}

private:
    Driver* m_sender;
};


/*!
 * \brief Класс данных стабилографии, получаемых от устройств StabDvcData class
 */
class StabDvcData : public DeviceData
{
public:
    StabDvcData(Driver *sender, double x, double y)
        : DeviceData(sender)
        , m_x(x), m_y(y) {m_z = 0; m_a = 0; m_b = 0; m_c = 0, m_d = 0;}

    StabDvcData(Driver *sender, double x, double y, double z)
        : DeviceData(sender)
        , m_x(x), m_y(y), m_z(z) {m_a = 0; m_b = 0; m_c = 0, m_d = 0;}

    StabDvcData(Driver *sender, double x, double y, double a, double b, double c)
        : DeviceData(sender)
        , m_x(x), m_y(y), m_a(a), m_b(b), m_c(c) {m_z = m_a + m_b + m_c; m_d = 0;}

    StabDvcData(Driver *sender, double x, double y, double a, double b, double c, double d)
        : DeviceData(sender)
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

    QString uid() const override {return uid_StabDvcData;}
//    QString name() const override {return name_StabDvcData;} Непонятно, как быть с локализацией

private:
  double m_x, m_y, m_z;
  double m_a, m_b, m_c, m_d;
};

/*!
 * \brief Класс данных динамометрии, получаемых от устройств DynamoDvcData class
 */
class DynamoDvcData : public DeviceData
{
public:
    DynamoDvcData(Driver *sender, const QVector<double> &data)
        : DeviceData(sender), m_data(data) {}

    int size() const {return m_data.size();}

    double value(const int i) const
    {
        Q_ASSERT(i >= 0 && i < m_data.size());
        return m_data.at(i);
    }

private:
    QVector<double> m_data;
};

/*!
 * \brief Класс данных прыжковой платформы, получаемых от устройств JumpPlateDvcData class
 */
class JumpPlateDvcData : public DeviceData
{
public:
    JumpPlateDvcData(Driver *sender, int plate, bool busy, double time)
        : DeviceData(sender), m_plate(plate), m_busy(busy), m_time(time) {}

    int plate() const {return m_plate;}
    bool busy() const {return m_busy;}
    double time() const {return m_time;}

    QString uid() const override {return uid_JumpPlateDvcData;}
//    QString name() const override {return name_JumpPlateDvcData;}  Непонятно, как быть с локализацией

private:
    int m_plate;
    bool m_busy;
    double m_time;
};

/*!
 * \brief Класс данных отсчетов прыжковой платформы, получаемых от устройств JumpPlateBlockData class
 */
class JumpPlateBlockData : public DeviceData
{
public:
    JumpPlateBlockData(Driver *sender, int blockCnt,
                       bool busy1, double counter1, double con1,
                       bool busy2, double counter2, double con2)
        : DeviceData(sender), m_blockCnt(blockCnt)
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
static const QString uid_DynControl = "{2D6BC458-8C8D-4125-AF3C-9F7352AED8F5}";
static const QString uid_JumpPlateControl = "{F7EF6F97-3502-4EC0-A798-68DDF40DFB7F}";

static const QString name_CommonControl = "Управление устройством общий";
static const QString name_StabControl = "Управление стабилоплатформой";
static const QString name_DynControl = "Управление динамометрами";
static const QString name_JumpPlateControl = "Управление прыжковой платформой";

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
 * \brief Класс управления устройствами данными стабилографии StabControl class
 */
class StabControl : public DeviceControl
{
public:
    virtual void calibrate() = 0;
    virtual void zeroing() = 0;

    static QString uid() {return uid_StabControl;}
//    static QString name() {return name_StabControl;}         Непонятно, как быть с локализацией

    ~StabControl() {}
};

/*!
 * \brief Класс управления устройствами данными динамометрии DynControl class
 */
class DynControl : public DeviceControl
{
public:
    virtual void calibrate(int chan) = 0;

    static QString uid() {return uid_DynControl;}
//    static QString name() {return name_DynControl;}        Непонятно, как быть с локализацией

    ~DynControl() {}
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
static const QString uid_JumpPlateProtocol = "{7E31AE8E-96AB-4E34-96DA-54E19B312377}";

static const QString name_StabProtocol = "Протокол стабилографии";
static const QString name_DynProtocol = "Протокол динамометрии";
static const QString name_JumpPlateProtocol = "Протокол прыжковой платформы";

static QList<DeviceProtocol> protocols = QList<DeviceProtocol>()
        << DeviceProtocol(std::make_tuple(uid_StabProtocol, name_StabProtocol, uid_StabControl, uid_StabDvcData))
        << DeviceProtocol(std::make_tuple(uid_DynProtocol, name_DynProtocol, uid_DynControl, uid_DynDvcData))
        << DeviceProtocol(std::make_tuple(uid_JumpPlateProtocol, name_JumpPlateProtocol, uid_JumpPlateControl, uid_JumpPlateDvcData));


///<------------------------------------------------------------------------------------------
///< Сервис

///< Получение названия протокола по uid
static QMap<QString, QString> protocolName {
    std::pair<QString, QString> (uid_StabProtocol, name_StabProtocol)
  , std::pair<QString, QString> (uid_DynProtocol, name_DynProtocol)
  , std::pair<QString, QString> (uid_JumpPlateProtocol, name_JumpPlateProtocol)
};

///< Получение названия интерфейса управления по uid
static QMap<QString, QString> controlName {
    std::pair<QString, QString> (uid_CommonControl, name_CommonControl)
  , std::pair<QString, QString> (uid_StabControl, name_StabControl)
  , std::pair<QString, QString> (uid_DynControl, name_DynControl)
  , std::pair<QString, QString> (uid_JumpPlateControl, name_JumpPlateControl)
};

///< Получение названия данных от устройства по uid
static QMap<QString, QString> deviceDataName {
    std::pair<QString, QString> (uid_StabDvcData, name_StabDvcData)
  , std::pair<QString, QString> (uid_DynDvcData, name_DynDvcData)
  , std::pair<QString, QString> (uid_JumpPlateDvcData, name_JumpPlateDvcData)
  , std::pair<QString, QString> (uid_JumpPlateBlockData, name_JumpPlateBlockData)
};


}

Q_DECLARE_METATYPE(QList<DeviceProtocols::Ports>)



#endif // DEVICEPROTOCOLS_H
