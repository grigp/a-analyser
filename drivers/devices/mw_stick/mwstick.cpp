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

    m_channel = params["channel"].toInt(0);
    m_rkp = params["rkp"].toDouble(0.8);
    m_pn = params["pn"].toInt(500);

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
        retval << ChannelsDefines::chanDynHand1;
    return retval;
}

QList<QString> MWStick::getChannelsByFormat(const QString &formatUid) const
{
    // todo: Неплохо было бы автоматизировать, используя getChannels(), чтобы один раз составить список, а здесь только выбирать из него
    QList<QString> retval;
    if (formatUid == ChannelsDefines::cfSinglePositive)
        retval << ChannelsDefines::chanDynHand1;
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
    Q_UNUSED(channelUid);
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
    Q_UNUSED(channelUid);
    quint8 chan = 0;
    if (m_channel == 0)
        chan = 0xA;
    else
    if (m_channel == 1)
        chan = 0xB;
    else
    if (m_channel == 2)
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
    min = 0;
    max = m_pn;
}

void MWStick::getTensoValueDiapasone(const QString channelId, double &min, double &max)
{
    if (channelId == ChannelsDefines::chanDynHand1)
    {
        min = 0;
        max = m_pn;
    }
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
    auto assignByteTenso = [&](double &value)
    {
        if (m_countBytePack % 2 == 0)
        {
            m_prevB = b;
        }
        else
        {
            double dynVal = b * 256 + m_prevB;
            dynVal = dynVal - 0x8000;
            value = (m_pn * 1000 * dynVal) / (65535 * 128 * m_rkp);
            if (value < 0)
                value = fabs(value);
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
                    //assignByteTenso(m_t1);
                    break;
                }
                case 8: {             //! Тензоканал 2
                    //assignByteTenso(m_t2);
                    break;
                }
                case 9: {             //! Тензоканал 3
                    assignByteTenso(m_t3);
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
            if (m_countBytePack == m_countChannels * 2)
            {  //! Достигли заданного кол-ва каналов
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
    double val = 0;
    if (m_channel == 0)
        val = m_t1;
    else
    if (m_channel == 1)
        val = m_t2;
    else
    if (m_channel == 2)
        val = m_t3;

    auto tenso = new DeviceProtocols::TensoDvcData(this, ChannelsDefines::chanDynHand1, val);
    emit sendData(tenso);
    delete tenso;
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


