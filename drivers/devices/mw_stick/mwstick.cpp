#include "mwstick.h"

#include "mwstickparamsdialog.h"
#include "aanalyserapplication.h"
#include "serialport.h"
#include "channelsutils.h"
#include "settingsprovider.h"
#include "driverdefines.h"
#include "initialsetupstabilandialog.h"
#include "connection.h"

#include <QApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QtMath>
#include <QDebug>

namespace
{
///< Идентификаторы тензоканалов по типу для разъемов
static QMap<DeviceProtocols::TensoDevice, QStringList> td1ChannelsByDevice{
    std::pair<DeviceProtocols::TensoDevice, QStringList> (DeviceProtocols::tdDynHand,
                                                          QStringList() << ChannelsDefines::chanDynHand1
                                                                        << ChannelsDefines::FirstPlatform::chanDynHand1
                                                                        << ChannelsDefines::SecondPlatform::chanDynHand1)
  , std::pair<DeviceProtocols::TensoDevice, QStringList> (DeviceProtocols::tdDynStand,
                                                          QStringList() << ChannelsDefines::chanDynStand1
                                                                        << ChannelsDefines::FirstPlatform::chanDynStand1
                                                                        << ChannelsDefines::SecondPlatform::chanDynStand1)
  , std::pair<DeviceProtocols::TensoDevice, QStringList> (DeviceProtocols::tdBreath,
                                                          QStringList() << ChannelsDefines::chanBreath1
                                                                        << ChannelsDefines::FirstPlatform::chanBreath1
                                                                        << ChannelsDefines::SecondPlatform::chanBreath1)
  , std::pair<DeviceProtocols::TensoDevice, QStringList> (DeviceProtocols::tdPushDevice,
                                                          QStringList() << ChannelsDefines::chanDynPush1
                                                                        << ChannelsDefines::FirstPlatform::chanDynPush1
                                                                        << ChannelsDefines::SecondPlatform::chanDynPush1)
};
static QMap<DeviceProtocols::TensoDevice, QStringList> td2ChannelsByDevice{
    std::pair<DeviceProtocols::TensoDevice, QStringList> (DeviceProtocols::tdDynHand,
                                                          QStringList() << ChannelsDefines::chanDynHand2
                                                                        << ChannelsDefines::FirstPlatform::chanDynHand2
                                                                        << ChannelsDefines::SecondPlatform::chanDynHand2)
  , std::pair<DeviceProtocols::TensoDevice, QStringList> (DeviceProtocols::tdDynStand,
                                                          QStringList() << ChannelsDefines::chanDynStand2
                                                                        << ChannelsDefines::FirstPlatform::chanDynStand2
                                                                        << ChannelsDefines::SecondPlatform::chanDynStand2)
  , std::pair<DeviceProtocols::TensoDevice, QStringList> (DeviceProtocols::tdBreath,
                                                          QStringList() << ChannelsDefines::chanBreath2
                                                          << ChannelsDefines::FirstPlatform::chanBreath2
                                                          << ChannelsDefines::SecondPlatform::chanBreath2)
  , std::pair<DeviceProtocols::TensoDevice, QStringList> (DeviceProtocols::tdPushDevice,
                                                          QStringList() << ChannelsDefines::chanDynPush2
                                                          << ChannelsDefines::FirstPlatform::chanDynPush2
                                                          << ChannelsDefines::SecondPlatform::chanDynPush2)
};
static QMap<DeviceProtocols::TensoDevice, QStringList> td3ChannelsByDevice{
    std::pair<DeviceProtocols::TensoDevice, QStringList> (DeviceProtocols::tdDynHand,
                                                          QStringList() << ChannelsDefines::chanDynHand3
                                                                        << ChannelsDefines::FirstPlatform::chanDynHand3
                                                                        << ChannelsDefines::SecondPlatform::chanDynHand3)
  , std::pair<DeviceProtocols::TensoDevice, QStringList> (DeviceProtocols::tdDynStand,
                                                          QStringList() << ChannelsDefines::chanDynStand3
                                                                        << ChannelsDefines::FirstPlatform::chanDynStand3
                                                                        << ChannelsDefines::SecondPlatform::chanDynStand3)
  , std::pair<DeviceProtocols::TensoDevice, QStringList> (DeviceProtocols::tdBreath,
                                                          QStringList() << ChannelsDefines::chanBreath3
                                                                        << ChannelsDefines::FirstPlatform::chanBreath3
                                                                        << ChannelsDefines::SecondPlatform::chanBreath3)
  , std::pair<DeviceProtocols::TensoDevice, QStringList> (DeviceProtocols::tdPushDevice,
                                                          QStringList() << ChannelsDefines::chanDynPush3
                                                                        << ChannelsDefines::FirstPlatform::chanDynPush3
                                                                        << ChannelsDefines::SecondPlatform::chanDynPush3)
};

const quint8 MarkerValue = 0x80;

///< Поддерживаемые протоколы управления устройствами
QSet<QString> Controls =
{
    DeviceProtocols::uid_CommonControl,
    DeviceProtocols::uid_TensoControl,
};

}

MWStick::MWStick(QObject *parent)
    : Driver (parent)
{

}

MWStick::~MWStick()
{

}

void MWStick::setParams(const DeviceProtocols::Ports port, const QJsonObject &params)
{
    setPortName(port);

    m_chanRecordingDefault = MWStick::getChanRecordingDefault(params["chan_recording_default"].toObject());

    m_tenso1.device = static_cast<DeviceProtocols::TensoDevice>(params["tenso1"].toInt(0));
    m_tenso2.device = static_cast<DeviceProtocols::TensoDevice>(params["tenso2"].toInt(1));
    m_tenso3.device = static_cast<DeviceProtocols::TensoDevice>(params["tenso3"].toInt(2));
    m_tenso1.rkp = params["rkp1"].toDouble(1.7);
    m_tenso2.rkp = params["rkp2"].toDouble(1.7);
    m_tenso3.rkp = params["rkp3"].toDouble(1.7);
    m_tenso1.pn = params["pn1"].toDouble(100);
    m_tenso2.pn = params["pn2"].toDouble(500);
    m_tenso3.pn = params["pn3"].toDouble(1);

    m_countChannels = 27;
}

bool MWStick::editParams(QJsonObject &params)
{
    MWStickParamsDialog dlg(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow());
    dlg.setChannel(params["channel"].toInt(0));
    dlg.setRkp(params["rkp"].toDouble(1.7));
    dlg.setPn(params["pn"].toInt(100));

    if (dlg.exec() == QDialog::Accepted)
    {
        params["channel"] = dlg.channel();
        params["rkp"] = dlg.rkp();
        params["pn"] = dlg.pn();

        return true;
    }
    return false;
}

void MWStick::start()
{
    Driver::start();
}

void MWStick::stop()
{
    Driver::stop();
}

int MWStick::frequency(const QString &channelId) const
{
    return 50;
}

QList<QString> MWStick::getChannelsByProtocol(const QString &protocolUid) const
{
    // todo: Неплохо было бы автоматизировать, используя getChannels(), чтобы один раз составить список, а здесь только выбирать из него
    QList<QString> retval;
    if (protocolUid == DeviceProtocols::uid_DynProtocol)
    {
        if ((m_tenso1.device != DeviceProtocols::tdBreath) && td1ChannelsByDevice.contains(m_tenso1.device))
            retval << td1ChannelsByDevice.value(m_tenso1.device).at(0);
        if ((m_tenso2.device != DeviceProtocols::tdBreath) && td2ChannelsByDevice.contains(m_tenso2.device))
            retval << td2ChannelsByDevice.value(m_tenso2.device).at(0);
        if ((m_tenso3.device != DeviceProtocols::tdBreath) && td3ChannelsByDevice.contains(m_tenso3.device))
            retval << td3ChannelsByDevice.value(m_tenso3.device).at(0);
    }

    return retval;
}

QList<QString> MWStick::getChannelsByFormat(const QString &formatUid) const
{
    // todo: Неплохо было бы автоматизировать, используя getChannels(), чтобы один раз составить список, а здесь только выбирать из него
    QList<QString> retval;
    if (formatUid == ChannelsDefines::cfSinglePositive)
    {
        if ((m_tenso1.device != DeviceProtocols::tdBreath) && td1ChannelsByDevice.contains(m_tenso1.device))
            retval << td1ChannelsByDevice.value(m_tenso1.device).at(0);
        if ((m_tenso2.device != DeviceProtocols::tdBreath) && td2ChannelsByDevice.contains(m_tenso2.device))
            retval << td2ChannelsByDevice.value(m_tenso2.device).at(0);
        if ((m_tenso3.device != DeviceProtocols::tdBreath) && td3ChannelsByDevice.contains(m_tenso3.device))
            retval << td3ChannelsByDevice.value(m_tenso3.device).at(0);
    }
    return retval;
}

QList<QString> MWStick::getChannels() const
{
    QList<QString> retval;
    retval << ChannelsDefines::chanDynHand1;
    return retval;
}

int MWStick::getSubChannelsCount(const QString &channelUid) const
{
    return 1;
}

bool MWStick::isChannelRecordingDefault(const QString &channelUid, const int subChan) const
{
    Q_UNUSED(channelUid);
    Q_UNUSED(subChan);
    return true;
}

DeviceProtocols::DeviceControl *MWStick::getDeviceControl(const QString &controlId, const QString &channelId)
{
    Q_UNUSED(channelId);
    if (controlId == DeviceProtocols::uid_CommonControl)
        return dynamic_cast<DeviceProtocols::CommonControl*>(this);
    else
    if (controlId == DeviceProtocols::uid_TensoControl)
        return dynamic_cast<DeviceProtocols::TensoControl*>(this);
    return nullptr;
}

QStringList MWStick::getProtocols()
{
    return QStringList() << DeviceProtocols::uid_DynProtocol;
}

QList<DeviceProtocols::Ports> MWStick::getPorts()
{
    return DeviceProtocols::comPorts();
}

void MWStick::calibrateTenso(const QString &channelUid)
{
    quint8 chan = 0;
    if (td1ChannelsByDevice.value(m_tenso1.device).contains(channelUid))
        chan = 0xA;
    else
    if (td2ChannelsByDevice.value(m_tenso2.device).contains(channelUid))
        chan = 0xB;
    else
    if (td3ChannelsByDevice.value(m_tenso3.device).contains(channelUid))
        chan = 0xC;
    if (chan != 0)
    {
        QByteArray cmd;
        cmd.resize(4);
        cmd[0] = 0x33;
        cmd[1] = static_cast<char>(chan);
        cmd[2] = 0x33;
        cmd[3] = 0x7;
        emit writeData(cmd);
        m_isCalibrated = true;
    }
}

void MWStick::getTensoValueDiapasone(const int chanNumber, double &min, double &max)
{
    Q_ASSERT(chanNumber >= 0 && chanNumber < 3);
    auto getMinMax = [&](DeviceProtocols::TensoChannel &tenso)
    {
        if (tenso.device != DeviceProtocols::tdBreath)
        {
            min = 0;
            max = tenso.pn;
        }
        else
        {
            min = m_tensoPercMin[chanNumber];
            max = m_tensoPercMax[chanNumber];
        }
    };

    if (chanNumber == 0)
        getMinMax(m_tenso1);
    else
    if (chanNumber == 1)
        getMinMax(m_tenso2);
    else
    if (chanNumber == 2)
        getMinMax(m_tenso3);
}

void MWStick::getTensoValueDiapasone(const QString channelId, double &min, double &max)
{
    if (td1ChannelsByDevice.value(m_tenso1.device).contains(channelId))
        getTensoValueDiapasone(0, min, max);
    else
    if (td2ChannelsByDevice.value(m_tenso2.device).contains(channelId))
        getTensoValueDiapasone(1, min, max);
    else
    if (td3ChannelsByDevice.value(m_tenso3.device).contains(channelId))
        getTensoValueDiapasone(2, min, max);
    else
    {
        min = 0;
        max = 100;
    }
}

void MWStick::setTensoValueDiapasone(const int chanNumber, const double min, const double max)
{
    Q_ASSERT(chanNumber >= 0 && chanNumber < 3);
    m_tensoPercMin[chanNumber] = min;
    m_tensoPercMax[chanNumber] = max;
}

bool MWStick::isCalibrated() const
{
    return m_isCalibrated;
}

SerialPortDefines::Settings MWStick::getSerialPortSettings()
{
    return SerialPortDefines::Settings(57600,
                                       QSerialPort::Data8,
                                       QSerialPort::NoParity,
                                       QSerialPort::OneStop,
                                       QSerialPort::NoFlowControl);
}

void MWStick::on_readData(const QByteArray data)
{
    Driver::on_readData(data);
    for (int i = 0; i < data.count(); i++)
    {
        quint8 B = static_cast<quint8>(data[i]);
        assignByteFromDevice(B);
    }
}

void MWStick::on_error(const QString &err)
{
    Driver::on_error(err);
    qDebug() << err;
}

void MWStick::assignByteFromDevice(quint8 b)
{
    //! Обработка байта тензоканала
    auto assignByteTenso = [&](const DeviceProtocols::TensoChannel tenso, const int chan, double &value)
    {
        if (m_countBytePack % 2 == 0)
        {
            m_prevB = b;
        }
        else
        {
            double dynVal = b * 256 + m_prevB;
            dynVal = dynVal - 0x8000;
            value = (tenso.pn * 1000 * dynVal) / (65535 * 128 * tenso.rkp);
            if (tenso.device == DeviceProtocols::tdBreath)
                value = ((value * 100) + (m_tensoPercMax[chan] - m_tensoPercMin[chan]) / 2);
        }
    };


    bool isTwoMarkers = false;
    bool isError = false;

    if (b == MarkerValue)    //! Пришел байт маркера
    {
        if (!m_isMarker){    //! Ожидание первого байта маркера
            m_isMarker = true;
        } else {           //! Ожидание второго байта маркера
            if (m_isPackage){   //! Два маркера внутри пакета - ошибка
                isError = true;
            }
            m_isMarker = false;
            m_isPackage = true;    //! Признак начала приема пакета
            m_countBytePack = 0;
            isTwoMarkers = true;
        }
    } else {            //! Не байт маркера
        m_isMarker = false;
    }

    if (!isTwoMarkers){    //! Если не было два маркера подряд, то эта ситуация внутри пакета и нам надо это отрабатывать
        if (m_isPackage){
            switch (m_countBytePack / 2) {
                case 0:{     //! Фронталь X
                    break;
                }
                case 1:{     //! Сагитталь Y
                    break;
                }
                case 2:{       //! Реакция опоры A
                    break;
                }
                case 3:{       //! Реакция опоры B
                    break;
                }
                case 4:{       //! Реакция опоры C
                    break;
                }
                case 5:{       //! Реакция опоры D
                    break;
                }
                case 6: {          //! RR + Synchro
                    break;
                }
                case 7: {             //! Тензоканал 1
                    assignByteTenso(m_tenso1, 0, m_t1);
                    break;
                }
                case 8: {             //! Тензоканал 2
                    assignByteTenso(m_tenso2, 1, m_t2);
                    break;
                }
                case 9: {             //! Тензоканал 3
                    assignByteTenso(m_tenso3, 2, m_t3);
                    break;
                }
                case 10: {            //! Пульс
                    break;
                }
                case 11: {break; }  //! Миограммы 4 канала и 4 отведения
                case 12: {break; }
                case 13: {break; }
                case 14: {break; }
                case 15: {break; }
                case 16: {break; }
                case 17: {break; }
                case 18: {break; }
                case 19: {break; }
                case 20: {break; }
                case 21: {break; }
                case 22: {break; }
                case 23: {break; }
                case 24: {break; }
                case 25: {break; }
                case 26: {break; }
            }

            //! Окончание разбора пакета
            if (m_countBytePack == m_countChannels * 2){  //! Достигли заданного кол-ва каналов
                incBlockCount();
                sendDataBlock();
                m_isPackage = false;                     //! Сбросим признак пакета
            }

            m_countBytePack++;
        }
    }

    //! Передача информации об ошибке маркера внутри пакета
    if (isError){
        emit error(DriverDefines::EC_MarkerIinsidePackage);
    }
}

void MWStick::sendDataBlock()
{
    sendTensoChannels();
}

void MWStick::sendTensoChannels()
{
    if (td1ChannelsByDevice.contains(m_tenso1.device))
    {
        auto tenso1 = new DeviceProtocols::TensoDvcData(this, td1ChannelsByDevice.value(m_tenso1.device).at(0), m_t1);
        emit sendData(tenso1);
        delete tenso1;
    }

    if (td2ChannelsByDevice.contains(m_tenso2.device))
    {
        auto tenso2 = new DeviceProtocols::TensoDvcData(this, td2ChannelsByDevice.value(m_tenso2.device).at(0), m_t2);
        emit sendData(tenso2);
        delete tenso2;
    }

    if (td3ChannelsByDevice.contains(m_tenso3.device))
    {
        auto tenso3 = new DeviceProtocols::TensoDvcData(this, td3ChannelsByDevice.value(m_tenso3.device).at(0), m_t3);
        emit sendData(tenso3);
        delete tenso3;
    }
}

QMap<QString, bool> MWStick::getChanRecordingDefault(const QJsonObject &obj)
{
    QMap<QString, bool> retval;
    retval.insert(ChannelsDefines::chanTenso1, obj["tenso1"].toBool());
    return retval;
}

QJsonObject MWStick::setChanRecordingDefault(const QMap<QString, bool> &recMap)
{
    QJsonObject recObj;
    recObj["tenso1"] = recMap.value(ChannelsDefines::chanTenso1);
    return recObj;
}


