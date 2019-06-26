#ifndef DEVICEPROTOCOLS_H
#define DEVICEPROTOCOLS_H

#include <QObject>

namespace DeviceProtocols {

static const QString uid_StabDvcData = "{69941064-D628-4A6F-A415-0EA12156D237}";

static const QString name_StabDvcData = "Данные стабилографии";


/*!
 * \brief Базовый класс данных от устройств DeviceData class
 */
class DeviceData
{
public:
    explicit DeviceData() {}

    virtual QString uid() = 0;
    virtual QString name() = 0;
};


/*!
 * \brief Класс данных стабилографии, получаемых от устройств StabDvcData class
 */
class StabDvcData : public DeviceData
{
public:
    StabDvcData(double x, double y)
        : m_x(x), m_y(y) {m_z = 0; m_a = 0; m_b = 0; m_c = 0, m_d = 0;}

    StabDvcData(double x, double y, double z)
        : m_x(x), m_y(y), m_z(z) {m_a = 0; m_b = 0; m_c = 0, m_d = 0;}

    StabDvcData(double x, double y, double a, double b, double c)
        : m_x(x), m_y(y), m_a(a), m_b(b), m_c(c) {m_z = m_a + m_b + m_c; m_d = 0;}

    StabDvcData(double x, double y, double a, double b, double c, double d)
        : m_x(x), m_y(y), m_a(a), m_b(b), m_c(c), m_d(d) {m_z = m_a + m_b + m_c + m_d;}

    double x() {return m_x;}
    double y() {return m_y;}
    double z() {return m_z;}

    double a() {return m_a;}
    double b() {return m_b;}
    double c() {return m_c;}
    double d() {return m_d;}

    QString uid() override {return uid_StabDvcData;}
    QString name() override {return name_StabDvcData;}

private:
  double m_x, m_y, m_z;
  double m_a, m_b, m_c, m_d;
};


///<------------------------------------------------------------------------------------------

static const QString uid_CommonControl = "{4EBD7247-9F63-4722-82C3-0FE86ADB276F}";
static const QString uid_StabControl = "{D9498794-C63E-426D-8BB5-09B3CE1EADD8}";

static const QString name_CommonControl = "Управление устройством общий";
static const QString name_StabControl = "Управление передачей стабилограмы";

/*!
 * \brief Базовый класс интерфейсов управления устройствами DeviceControl class
 */
class DeviceControl
{
public:
    explicit DeviceControl() {}
    ~DeviceControl() {}

    virtual QString uid() = 0;
    virtual QString name() = 0;
};


/*!
 * \brief Класс общего интерфейса управления устройствами CommonControl class
 */
class CommonControl :public DeviceControl
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;

    QString uid() {return uid_CommonControl;}
    QString name() {return name_CommonControl;}

    ~CommonControl() {}
};

/*!
 * \brief Класс управления устройствами данными стабилографии StabControl class
 */
class StabControl :public DeviceControl
{
public:
    virtual void calibrate() = 0;
    virtual void zeroing() = 0;

    QString uid() {return uid_StabControl;}
    QString name() {return name_StabControl;}

    ~StabControl() {}
};


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

///<------------------------------------------------------------------------------------------

static const QString uid_StabProtocol = "{3F67F2E4-CA04-4934-A942-CBB827DF004F}";

static const QString name_StabProtocol = "Протокол стабилографии";

QList<DeviceProtocol> protocols = QList<DeviceProtocol>()
        << DeviceProtocol(std::make_tuple(uid_StabProtocol, name_StabProtocol, uid_StabControl, uid_StabDvcData));

}

#endif // DEVICEPROTOCOLS_H
