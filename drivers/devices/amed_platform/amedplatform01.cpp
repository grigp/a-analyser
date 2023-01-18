#include "amedplatform01.h"

#include "aanalyserapplication.h"
#include "amedplatform01paramsdialog.h"
#include "channelsutils.h"

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

}

AMedPlatform01::AMedPlatform01(QObject *parent)
    : Driver (parent)
{

}

AMedPlatform01::~AMedPlatform01()
{

}

void AMedPlatform01::setParams(const DeviceProtocols::Ports port, const QJsonObject &params)
{
    setPortName(port);
//    m_model = static_cast<Stabilan01Defines::Model>(params["model"].toInt());
//    m_zt = static_cast<Stabilan01Defines::ZeroingType>(params["zeroing_type"].toInt());
    m_chanRecordingDefault = AMedPlatform01::getChanRecordingDefault(params["chan_recording_default"].toObject());

    m_tenso1.device = static_cast<DeviceProtocols::TensoDevice>(params["tenso1"].toInt(0));
    m_tenso2.device = static_cast<DeviceProtocols::TensoDevice>(params["tenso2"].toInt(1));
    m_tenso3.device = static_cast<DeviceProtocols::TensoDevice>(params["tenso3"].toInt(2));
    m_tenso1.rkp = params["rkp1"].toDouble(1.7);
    m_tenso2.rkp = params["rkp2"].toDouble(1.7);
    m_tenso3.rkp = params["rkp3"].toDouble(1.7);
    m_tenso1.pn = params["pn1"].toDouble(100);
    m_tenso2.pn = params["pn2"].toDouble(500);
    m_tenso3.pn = params["pn3"].toDouble(1);

    //m_countChannels = ChannelsCount.value(m_model);
}

bool AMedPlatform01::editParams(QJsonObject &params)
{
//    auto model = params["model"].toInt();
//    auto zt = params["zeroing_type"].toInt();

    AMedPlatform01ParamsDialog dlg(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow());
//    dlg.setModel(static_cast<Stabilan01Defines::Model>(model));
//    dlg.setZeroingType(static_cast<Stabilan01Defines::ZeroingType>(zt));
//    dlg.setRecording(getChanRecordingDefault(params["chan_recording_default"].toObject()));

    dlg.setKindTenso1(static_cast<DeviceProtocols::TensoDevice>(params["tenso1"].toInt(0)));
    dlg.setKindTenso2(static_cast<DeviceProtocols::TensoDevice>(params["tenso2"].toInt(1)));
    dlg.setKindTenso3(static_cast<DeviceProtocols::TensoDevice>(params["tenso3"].toInt(2)));
    dlg.setRkpTenso1(params["rkp1"].toDouble(1.7));
    dlg.setRkpTenso2(params["rkp2"].toDouble(1.7));
    dlg.setRkpTenso3(params["rkp3"].toDouble(1.7));
    dlg.setPnTenso1(params["pn1"].toDouble(100));
    dlg.setPnTenso2(params["pn2"].toDouble(500));
    dlg.setPnTenso3(params["pn3"].toDouble(1));

    if (dlg.exec() == QDialog::Accepted)
    {
//        params["model"] = static_cast<int>(dlg.model());
//        params["zeroing_type"] = static_cast<int>(dlg.zeroingType());
//        params["chan_recording_default"] = setChanRecordingDefault(dlg.getRecording());

        params["tenso1"] = dlg.kindTenso1();
        params["tenso2"] = dlg.kindTenso2();
        params["tenso3"] = dlg.kindTenso3();
        params["rkp1"] = dlg.rkpTenso1();
        params["rkp2"] = dlg.rkpTenso2();
        params["rkp3"] = dlg.rkpTenso3();
        params["pn1"] = dlg.pnTenso1();
        params["pn2"] = dlg.pnTenso2();
        params["pn3"] = dlg.pnTenso3();

        return true;
    }
    return false;
}

void AMedPlatform01::start()
{
    Driver::start();
    cmdStartImpulse();
}

void AMedPlatform01::stop()
{
    cmdStop();
    Driver::stop();
}

int AMedPlatform01::frequency(const QString &channelId) const
{
    static QMap<QString, int> ChannelsFreq =
    {
        std::pair<QString, int> (ChannelsDefines::ctStabilogram, 100)
      , std::pair<QString, int> (ChannelsDefines::ctBalistogram, 100)
      , std::pair<QString, int> (ChannelsDefines::ctDynamo, 100)
      , std::pair<QString, int> (ChannelsDefines::ctBreath, 100)
    };

    if (ChannelsFreq.contains(ChannelsUtils::instance().channelType(channelId)))
        return ChannelsFreq.value(ChannelsUtils::instance().channelType(channelId));
    return 100;
}

QList<QString> AMedPlatform01::getChannelsByProtocol(const QString &protocolUid) const
{
    // todo: Неплохо было бы автоматизировать, используя getChannels(), чтобы один раз составить список, а здесь только выбирать из него
    QList<QString> retval;
    if (protocolUid == DeviceProtocols::uid_StabProtocol)
        retval << ChannelsDefines::chanStab;
    else
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

QList<QString> AMedPlatform01::getChannelsByFormat(const QString &formatUid) const
{
    // todo: Неплохо было бы автоматизировать, используя getChannels(), чтобы один раз составить список, а здесь только выбирать из него
    QList<QString> retval;
    if (formatUid == ChannelsDefines::cfDecartCoordinates)
        retval << ChannelsDefines::chanStab;
//    else
//    if (formatUid == ChannelsDefines::cfSinglePositive)
//    {
//        if (Stabilan01Defines::ModelsWithTenso.contains(m_model))
//        {
//            if ((m_tenso1.device != DeviceProtocols::tdBreath) && td1ChannelsByDevice.contains(m_tenso1.device))
//                retval << td1ChannelsByDevice.value(m_tenso1.device).at(0);
//            if ((m_tenso2.device != DeviceProtocols::tdBreath) && td2ChannelsByDevice.contains(m_tenso2.device))
//                retval << td2ChannelsByDevice.value(m_tenso2.device).at(0);
//            if ((m_tenso3.device != DeviceProtocols::tdBreath) && td3ChannelsByDevice.contains(m_tenso3.device))
//                retval << td3ChannelsByDevice.value(m_tenso3.device).at(0);
//        }
//    }
//    else
//    if (formatUid == ChannelsDefines::cfSingleDual)
//    {
//        if (Stabilan01Defines::ModelsWithTenso.contains(m_model))
//        {
//            if (m_tenso1.device == DeviceProtocols::tdBreath)
//                retval << ChannelsDefines::chanBreath1;
//            if (m_tenso2.device == DeviceProtocols::tdBreath)
//                retval << ChannelsDefines::chanBreath2;
//            if (m_tenso3.device == DeviceProtocols::tdBreath)
//                retval << ChannelsDefines::chanBreath3;
//        }
//    }
    return retval;
}

QList<QString> AMedPlatform01::getChannels() const
{
    QList<QString> retval;
    retval << ChannelsDefines::chanStab;
//    if (Stabilan01Defines::ModelsWithPulse.contains(m_model))
//        retval << ChannelsDefines::chanRitmogram;
//    if (Stabilan01Defines::ModelsWithTenso.contains(m_model))
//    {
//        if (td1ChannelsByDevice.contains(m_tenso1.device))
//            retval << td1ChannelsByDevice.value(m_tenso1.device).at(0);
//        if (td2ChannelsByDevice.contains(m_tenso2.device))
//            retval << td2ChannelsByDevice.value(m_tenso2.device).at(0);
//        if (td3ChannelsByDevice.contains(m_tenso3.device))
//            retval << td3ChannelsByDevice.value(m_tenso3.device).at(0);
//    }

    return retval;
}

int AMedPlatform01::getSubChannelsCount(const QString &channelUid) const
{
    if (ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctStabilogram ||
        ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctBalistogram ||
        ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctDynamo ||
        ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctBreath)
        return 1;
    return 0;
}

bool AMedPlatform01::isChannelRecordingDefault(const QString &channelUid, const int subChan) const
{
    if (m_chanRecordingDefault.contains(channelUid))
        return m_chanRecordingDefault.value(channelUid);
    else
    {
        if (td1ChannelsByDevice.value(m_tenso1.device).contains(channelUid))
            return m_chanRecordingDefault.value(ChannelsDefines::chanTenso1);
        else
        if (td2ChannelsByDevice.value(m_tenso2.device).contains(channelUid))
            return m_chanRecordingDefault.value(ChannelsDefines::chanTenso2);
        else
        if (td3ChannelsByDevice.value(m_tenso3.device).contains(channelUid))
            return m_chanRecordingDefault.value(ChannelsDefines::chanTenso3);
    }
    return false;
}

DeviceProtocols::DeviceControl *AMedPlatform01::getDeviceControl(const QString &controlId, const QString &channelId)
{
    Q_UNUSED(channelId);
    if (controlId == DeviceProtocols::uid_CommonControl)
        return dynamic_cast<DeviceProtocols::CommonControl*>(this);
    else
    if (controlId == DeviceProtocols::uid_StabControl)
        return dynamic_cast<DeviceProtocols::StabControl*>(this);
    else
    if (controlId == DeviceProtocols::uid_TensoControl)
        return dynamic_cast<DeviceProtocols::TensoControl*>(this);
    else
    if (controlId == DeviceProtocols::uid_PulseControl)
        return dynamic_cast<DeviceProtocols::PulseControl*>(this);

    return nullptr;
}

QStringList AMedPlatform01::getProtocols()
{
    return QStringList() << DeviceProtocols::uid_StabProtocol;
}

QList<DeviceProtocols::Ports> AMedPlatform01::getPorts()
{
    return DeviceProtocols::comPorts();
}

void AMedPlatform01::calibrate(const QString &channelUid)
{
    Q_UNUSED(channelUid);
//    QByteArray cmd;
//    cmd.resize(2);
//    cmd[0] = 0x33;
//    cmd[1] = 0x0;
//    emit writeData(cmd);
}

void AMedPlatform01::zeroing(const QString &channelUid)
{

}

QSize AMedPlatform01::stabSize()
{

}

void AMedPlatform01::calibrateTenso(const QString &channelUid)
{

}

void AMedPlatform01::getTensoValueDiapasone(const int chanNumber, double &min, double &max)
{

}

void AMedPlatform01::getTensoValueDiapasone(const QString channelId, double &min, double &max)
{

}

void AMedPlatform01::setTensoValueDiapasone(const int chanNumber, const double min, const double max)
{

}

bool AMedPlatform01::isCalibrated() const
{

}

SerialPortDefines::Settings AMedPlatform01::getSerialPortSettings()
{
    return SerialPortDefines::Settings(115200,
                                       QSerialPort::Data8,
                                       QSerialPort::NoParity,
                                       QSerialPort::OneStop,
                                       QSerialPort::NoFlowControl);
}

void AMedPlatform01::on_readData(const QByteArray data)
{
    Driver::on_readData(data);
    for (int i = 0; i < data.count(); i++)
    {
        quint8 B = static_cast<quint8>(data[i]);
        assignByteFromDevice(B);
    }
}

void AMedPlatform01::on_error(const QString &err)
{
    Driver::on_error(err);
    qDebug() << "AMedPlatform01 :" << err;
}

//double min {INT_MAX};
//double max {-INT_MAX};

int n {0};

void AMedPlatform01::assignByteFromDevice(quint8 b)
{
    qDebug() << n << b;
    ++n;
    bool isTwoMarkers = false;
    bool isError = false;

    if (b == MarkerValue)    //! Пришел байт маркера
    {
        if (!m_isMarker){    //! Ожидание первого байта маркера
            m_isMarker = true;
        }
        else
        {           //! Ожидание второго байта маркера
            if (m_isPackage){   //! Два маркера внутри пакета - ошибка
                isError = true;
            }
            m_isMarker = false;
            m_isPackage = true;    //! Признак начала приема пакета
            m_countBytePack = 0;
            isTwoMarkers = true;
            n = 0;
        }
    }
    else              //! Не байт маркера
        m_isMarker = false;

    if (!isTwoMarkers)   //! Если не было два маркера подряд, то эта ситуация внутри пакета и нам надо это отрабатывать
    {
        if (m_isPackage)
        {
            if (m_countBytePack == 0)
            {
                m_circleCounter = b;
            }
            else
            {
                int cnt = m_countBytePack - 1;
                if (cnt % 3 == 0)
                    m_byteLo = b;
                else
                if (cnt % 3 == 1)
                    m_byteMid = b;
                else
                {
                    int value = (b << 16) + (m_byteMid << 8) + m_byteLo;
                    if (cnt / 3 == 0)
                        m_A = value;
                    else
                    if (cnt / 3 == 1)
                        m_B = value;
                    else
                    if (cnt / 3 == 2)
                        m_C = value;
                    else
                    if (cnt / 3 == 3)
                        m_D = value;
                    else
                    if (cnt / 3 == 4)
                        m_t1 = value;
                    else
                    if (cnt / 3 == 5)
                        m_t2 = value;
                    else
                    if (cnt / 3 == 6)
                        m_t3 = value;
                }
            }

            //! Окончание разбора пакета
            if (m_countBytePack + 1 == m_countChannels * 3){  //! Достигли заданного кол-ва каналов
//                qDebug() << m_B;
                m_X = m_A;
                m_Y = m_B;
                m_Z = m_C;//m_A + m_B + m_C + m_D;                     //! Расчет баллистограммы

                incBlockCount();
                sendDataBlock();
                m_isPackage = false;                     //! Сбросим признак пакета
            }

            m_countBytePack++;
        }
    }

    //! Передача информации об ошибке маркера внутри пакета
    if (isError)
    {
        emit error(EC_MarkerIinsidePackage);
    }
}

void AMedPlatform01::sendDataBlock()
{
    sendStab();
//    sendPulse();
//    sendTensoChannels();
}

void AMedPlatform01::sendStab()
{
    //! Эмуляция стабилограммы кругом
//                m_X = 10 * sin(r);
//                m_Y = 10 * cos(r);
//                r = r + 2 * M_PI / 50;

    //! Передача стабилограммы
    auto stabData = new DeviceProtocols::StabDvcData(this, ChannelsDefines::chanStab,
                                                     m_X, m_Y,
//                                                     m_X - m_offsetX, m_Y - m_offsetY,
                                                     m_A, m_B, m_C, m_D);
    emit sendData(stabData);
    delete stabData;
}

QMap<QString, bool> AMedPlatform01::getChanRecordingDefault(const QJsonObject &obj)
{
    QMap<QString, bool> retval;
    retval.insert(ChannelsDefines::chanStab, obj["stab"].toBool());
    retval.insert(ChannelsDefines::chanZ, obj["z"].toBool());
    retval.insert(ChannelsDefines::chanRitmogram, obj["pulse"].toBool());
    retval.insert(ChannelsDefines::chanTenso1, obj["tenso1"].toBool());
    retval.insert(ChannelsDefines::chanTenso2, obj["tenso2"].toBool());
    retval.insert(ChannelsDefines::chanTenso3, obj["tenso3"].toBool());

    return retval;
}

QJsonObject AMedPlatform01::setChanRecordingDefault(const QMap<QString, bool> &recMap)
{
    QJsonObject recObj;
    recObj["stab"] = recMap.value(ChannelsDefines::chanStab);
    recObj["z"] = recMap.value(ChannelsDefines::chanZ);
    recObj["pulse"] = recMap.value(ChannelsDefines::chanRitmogram);
    recObj["tenso1"] = recMap.value(ChannelsDefines::chanTenso1);
    recObj["tenso2"] = recMap.value(ChannelsDefines::chanTenso2);
    recObj["tenso3"] = recMap.value(ChannelsDefines::chanTenso3);

    return recObj;
}

void AMedPlatform01::cmdStartSinus()
{
    QByteArray cmd;
    cmd.resize(2);
    cmd[0] = static_cast<char>(0xA5);
    cmd[1] = static_cast<char>(0x63);
    emit writeData(cmd);
}

void AMedPlatform01::cmdStartImpulse()
{
    QByteArray cmd;
    cmd.resize(2);
    cmd[0] = static_cast<char>(0xA5);
    cmd[1] = static_cast<char>(0x64);
    emit writeData(cmd);
}

void AMedPlatform01::cmdStop()
{
    QByteArray cmd;
    cmd.resize(2);
    cmd[0] = static_cast<char>(0xA5);
    cmd[1] = static_cast<char>(0x6F);
    emit writeData(cmd);
}
