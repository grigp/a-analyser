#include "stabilan01.h"

#include "stabilan01paramsdialog.h"
#include "aanalyserapplication.h"
#include "serialport.h"
#include "channelsutils.h"
#include "settingsprovider.h"
#include "driverdefines.h"

#include <QApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QtMath>
#include <QDebug>

namespace  {
  static QMap<Stabilan01Defines::Model, QString> StabilanModels =
  {
      std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcSt01, Stabilan01Defines::smnSt01)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcSt02, Stabilan01Defines::smnSt02)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcKSK123_20, Stabilan01Defines::smnKSK123_20)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcKSK123_21, Stabilan01Defines::smnKSK123_21)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcKSK123_22, Stabilan01Defines::smnKSK123_22)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcKSK123_23, Stabilan01Defines::smnKSK123_23)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcKSK123_30, Stabilan01Defines::smnKSK123_30)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcKSK123_31, Stabilan01Defines::smnKSK123_31)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcKSK123_32, Stabilan01Defines::smnKSK123_32)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcKSK123_33, Stabilan01Defines::smnKSK123_33)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcStabilan01_01, Stabilan01Defines::smnStabilan01_01)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcStabilan01_02, Stabilan01Defines::smnStabilan01_02)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcStabilan01_03, Stabilan01Defines::smnStabilan01_03)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcStabilan01_05, Stabilan01Defines::smnStabilan01_05)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcStabilan01_12, Stabilan01Defines::smnStabilan01_12)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcStabilan01_13, Stabilan01Defines::smnStabilan01_13)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcStabilan01_15, Stabilan01Defines::smnStabilan01_15)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcStabilan01_16, Stabilan01Defines::smnStabilan01_16)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcStabilan01_05NG, Stabilan01Defines::smnStabilan01_05NG)
    , std::pair<Stabilan01Defines::Model, QString> (Stabilan01Defines::smcStabilan01_12NG, Stabilan01Defines::smnStabilan01_12NG)
  };

  static QMap<Stabilan01Defines::Model, int> ChannelsCount =
  {
      std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcSt01, 2)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcSt02, 2)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcKSK123_20, 2)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcKSK123_21, 4)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcKSK123_22, 6)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcKSK123_23, 4)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcKSK123_30, 7)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcKSK123_31, 7)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcKSK123_32, 7)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcKSK123_33, 7)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcStabilan01_01, 11)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcStabilan01_02, 11)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcStabilan01_03, 11)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcStabilan01_05, 12)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcStabilan01_12, 27)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcStabilan01_13, 27)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcStabilan01_15, 27)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcStabilan01_16, 27)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcStabilan01_05NG, 27)
    , std::pair<Stabilan01Defines::Model, int> (Stabilan01Defines::smcStabilan01_12NG, 27)
  };

  static QMap<Stabilan01Defines::ZeroingType, QString> ZeroingTypes =
  {
      std::pair<Stabilan01Defines::ZeroingType, QString> (Stabilan01Defines::ztFast, Stabilan01Defines::ztnFast)
    , std::pair<Stabilan01Defines::ZeroingType, QString> (Stabilan01Defines::ztAveragePrev, Stabilan01Defines::ztnAveragePrev)
    , std::pair<Stabilan01Defines::ZeroingType, QString> (Stabilan01Defines::ztAverageNext, Stabilan01Defines::ztnAverageNext)
  };

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
      DeviceProtocols::uid_StabControl,
      DeviceProtocols::uid_TensoControl,
      DeviceProtocols::uid_PulseControl,
      DeviceProtocols::uid_MyoControl
  };

}


Stabilan01::Stabilan01(QObject *parent)
    : Driver(parent)
{
    m_offsetX = SettingsProvider::valueFromRegAppCopy("StabilanDriver", "zeroing_x").toDouble();
    m_offsetY = SettingsProvider::valueFromRegAppCopy("StabilanDriver", "zeroing_y").toDouble();

}

Stabilan01::~Stabilan01()
{

}

void Stabilan01::setParams(const DeviceProtocols::Ports port, const QJsonObject &params)
{
    setPortName(port);
    m_model = static_cast<Stabilan01Defines::Model>(params["model"].toInt());
    m_zt = static_cast<Stabilan01Defines::ZeroingType>(params["zeroing_type"].toInt());
    m_skgSource = static_cast<Stabilan01Defines::SKGSource>(params["skg_source"].toInt(0));

    m_chanRecordingDefault = Stabilan01::getChanRecordingDefault(params["chan_recording_default"].toObject());

    m_tenso1.device = static_cast<DeviceProtocols::TensoDevice>(params["tenso1"].toInt(0));
    m_tenso2.device = static_cast<DeviceProtocols::TensoDevice>(params["tenso2"].toInt(1));
    m_tenso3.device = static_cast<DeviceProtocols::TensoDevice>(params["tenso3"].toInt(2));
    m_tenso1.rkp = params["rkp1"].toDouble(1.7);
    m_tenso2.rkp = params["rkp2"].toDouble(1.7);
    m_tenso3.rkp = params["rkp3"].toDouble(1.7);
    m_tenso1.pn = params["pn1"].toDouble(100);
    m_tenso2.pn = params["pn2"].toDouble(500);
    m_tenso3.pn = params["pn3"].toDouble(1);

    m_countChannels = ChannelsCount.value(m_model);
}

bool Stabilan01::editParams(QJsonObject &params)
{
    auto model = params["model"].toInt();
    auto zt = params["zeroing_type"].toInt();

    Stabilan01ParamsDialog dlg(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow());
    dlg.setModel(static_cast<Stabilan01Defines::Model>(model));
    dlg.setZeroingType(static_cast<Stabilan01Defines::ZeroingType>(zt));
    dlg.setSKGSource(static_cast<Stabilan01Defines::SKGSource>(params["skg_source"].toInt()));

    dlg.setRecording(getChanRecordingDefault(params["chan_recording_default"].toObject()));

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
        params["model"] = static_cast<int>(dlg.model());
        params["zeroing_type"] = static_cast<int>(dlg.zeroingType());
        params["skg_source"] = dlg.skgSource();

        params["chan_recording_default"] = setChanRecordingDefault(dlg.getRecording());

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


void Stabilan01::start()
{
    Driver::start();
}

void Stabilan01::stop()
{

    Driver::stop();
}

int Stabilan01::frequency(const QString &channelId) const
{
    static QMap<QString, int> ChannelsFreq =
    {
        std::pair<QString, int> (ChannelsDefines::ctStabilogram, 50)
      , std::pair<QString, int> (ChannelsDefines::ctBalistogram, 50)
      , std::pair<QString, int> (ChannelsDefines::ctDynamo, 50)
      , std::pair<QString, int> (ChannelsDefines::ctBreath, 50)
      , std::pair<QString, int> (ChannelsDefines::ctMyogram, 200)
    };

    if (ChannelsFreq.contains(ChannelsUtils::instance().channelType(channelId)))
        return ChannelsFreq.value(ChannelsUtils::instance().channelType(channelId));
    return 50;
}

QList<QString> Stabilan01::getChannelsByProtocol(const QString &protocolUid) const
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
    if (protocolUid == DeviceProtocols::uid_MyoProtocol)
        retval << ChannelsDefines::chanMyogram;

    return retval;
}

QList<QString> Stabilan01::getChannelsByFormat(const QString &formatUid) const
{
    // todo: Неплохо было бы автоматизировать, используя getChannels(), чтобы один раз составить список, а здесь только выбирать из него
    QList<QString> retval;
    if (formatUid == ChannelsDefines::cfDecartCoordinates)
        retval << ChannelsDefines::chanStab;
    else
    if (formatUid == ChannelsDefines::cfSinglePositive)
    {
        if (Stabilan01Defines::ModelsWithTenso.contains(m_model))
        {
            if ((m_tenso1.device != DeviceProtocols::tdBreath) && td1ChannelsByDevice.contains(m_tenso1.device))
                retval << td1ChannelsByDevice.value(m_tenso1.device).at(0);
            if ((m_tenso2.device != DeviceProtocols::tdBreath) && td2ChannelsByDevice.contains(m_tenso2.device))
                retval << td2ChannelsByDevice.value(m_tenso2.device).at(0);
            if ((m_tenso3.device != DeviceProtocols::tdBreath) && td3ChannelsByDevice.contains(m_tenso3.device))
                retval << td3ChannelsByDevice.value(m_tenso3.device).at(0);
        }
        if (Stabilan01Defines::ModelsWithMyo.contains(m_model))
            retval << ChannelsDefines::chanMyogram;
    }
    else
    if (formatUid == ChannelsDefines::cfSingleDual)
    {
        if (Stabilan01Defines::ModelsWithTenso.contains(m_model))
        {
            if (m_tenso1.device == DeviceProtocols::tdBreath)
                retval << ChannelsDefines::chanBreath1;
            if (m_tenso2.device == DeviceProtocols::tdBreath)
                retval << ChannelsDefines::chanBreath2;
            if (m_tenso3.device == DeviceProtocols::tdBreath)
                retval << ChannelsDefines::chanBreath3;
        }
    }
    return retval;
}

QList<QString> Stabilan01::getChannels() const
{
    QList<QString> retval;
    retval << ChannelsDefines::chanStab;
    if (Stabilan01Defines::ModelsWithPulse.contains(m_model))
        retval << ChannelsDefines::chanRitmogram;
    if (Stabilan01Defines::ModelsWithTenso.contains(m_model))
    {
        if (td1ChannelsByDevice.contains(m_tenso1.device))
            retval << td1ChannelsByDevice.value(m_tenso1.device).at(0);
        if (td2ChannelsByDevice.contains(m_tenso2.device))
            retval << td2ChannelsByDevice.value(m_tenso2.device).at(0);
        if (td3ChannelsByDevice.contains(m_tenso3.device))
            retval << td3ChannelsByDevice.value(m_tenso3.device).at(0);
    }
    if (Stabilan01Defines::ModelsWithMyo.contains(m_model))
        retval << ChannelsDefines::chanMyogram;

    return retval;
}

int Stabilan01::getSubChannelsCount(const QString &channelUid) const
{
    if (ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctStabilogram ||
        ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctBalistogram ||
        ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctDynamo ||
        ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctBreath)
        return 1;
    else
    if (ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctMyogram)
        return 4;
    return 0;
}

bool Stabilan01::isChannelRecordingDefault(const QString &channelUid, const int subChan) const
{
    if (ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctMyogram && subChan > -1)
        return m_chanRecordingDefault.value("MyoChan" + QString::number(subChan));
    else
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

DeviceProtocols::DeviceControl *Stabilan01::getDeviceControl(const QString &controlId, const QString &channelId)
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
    else
    if (controlId == DeviceProtocols::uid_MyoControl)
        return dynamic_cast<DeviceProtocols::MyoControl*>(this);

    return nullptr;
}

QStringList Stabilan01::getProtocols()
{
    return QStringList() << DeviceProtocols::uid_StabProtocol;
}

QList<DeviceProtocols::Ports> Stabilan01::getPorts()
{
    return DeviceProtocols::comPorts();
}

void Stabilan01::calibrate(const QString &channelUid)
{
    Q_UNUSED(channelUid);
    QByteArray cmd;
    cmd.resize(2);
    cmd[0] = 0x33;
    cmd[1] = 0x0;
    emit writeData(cmd);
}

void Stabilan01::zeroing(const QString &channelUid)
{
    Q_UNUSED(channelUid);
    m_offsetX = m_X;
    m_offsetY = m_Y;
    SettingsProvider::setValueToRegAppCopy("StabilanDriver", "zeroing_x", m_offsetX);
    SettingsProvider::setValueToRegAppCopy("StabilanDriver", "zeroing_y", m_offsetY);
}

QSize Stabilan01::stabSize()
{
    if (m_model == Stabilan01Defines::smcSt01 ||
        m_model == Stabilan01Defines::smcSt02 ||
        m_model == Stabilan01Defines::smcKSK123_20 ||
        m_model == Stabilan01Defines::smcKSK123_21 ||
        m_model == Stabilan01Defines::smcKSK123_22 ||
        m_model == Stabilan01Defines::smcKSK123_23)
        return QSize(250, 250);
    else
    if (m_model == Stabilan01Defines::smcKSK123_30 ||
        m_model == Stabilan01Defines::smcKSK123_31 ||
        m_model == Stabilan01Defines::smcKSK123_32 ||
        m_model == Stabilan01Defines::smcKSK123_33)
        return QSize(300, 300);
    else
    if (m_model == Stabilan01Defines::smcStabilan01_01 ||
        m_model == Stabilan01Defines::smcStabilan01_02 ||
        m_model == Stabilan01Defines::smcStabilan01_03 ||
        m_model == Stabilan01Defines::smcStabilan01_05 ||
        m_model == Stabilan01Defines::smcStabilan01_12 ||
        m_model == Stabilan01Defines::smcStabilan01_13 ||
        m_model == Stabilan01Defines::smcStabilan01_15 ||
        m_model == Stabilan01Defines::smcStabilan01_16 ||
        m_model == Stabilan01Defines::smcStabilan01_05NG ||
        m_model == Stabilan01Defines::smcStabilan01_12NG)
        return QSize(500, 500);
    return QSize(500, 500);
}

void Stabilan01::calibrateTenso(const QString &channelUid)
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

void Stabilan01::getTensoValueDiapasone(const int chanNumber, double &min, double &max)
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

void Stabilan01::getTensoValueDiapasone(const QString channelId, double &min, double &max)
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

void Stabilan01::setTensoValueDiapasone(const int chanNumber, const double min, const double max)
{
    Q_ASSERT(chanNumber >= 0 && chanNumber < 3);
    m_tensoPercMin[chanNumber] = min;
    m_tensoPercMax[chanNumber] = max;
}

bool Stabilan01::isCalibrated() const
{
    return m_isCalibrated;
}

void Stabilan01::setBoundsDelArtifacts(const int low, const int high)
{
    Q_UNUSED(low);
    Q_UNUSED(high);

}

void Stabilan01::zeroingMyo()
{

}

void Stabilan01::zeroingMyo(const int channel)
{
    Q_UNUSED(channel);

}

double Stabilan01::amplitudeMyo()
{
    return 2;
}

QString Stabilan01::modelName(const Stabilan01Defines::Model mdlCode)
{
    switch (mdlCode) {
    case Stabilan01Defines::smcSt01 : return QCoreApplication::tr("СТ-01");
    case Stabilan01Defines::smcSt02 : return QCoreApplication::tr("СТ-02");
    case Stabilan01Defines::smcKSK123_20 : return QCoreApplication::tr("КСК-123-2.0");
    case Stabilan01Defines::smcKSK123_21 : return QCoreApplication::tr("КСК-123-2.1");
    case Stabilan01Defines::smcKSK123_22 : return QCoreApplication::tr("КСК-123-2.2");
    case Stabilan01Defines::smcKSK123_23 : return QCoreApplication::tr("КСК-123-2.3");
    case Stabilan01Defines::smcKSK123_30 : return QCoreApplication::tr("КСК-123-3.0");
    case Stabilan01Defines::smcKSK123_31 : return QCoreApplication::tr("КСК-123-3.1");
    case Stabilan01Defines::smcKSK123_32 : return QCoreApplication::tr("КСК-123-3.2");
    case Stabilan01Defines::smcKSK123_33 : return QCoreApplication::tr("КСК-123-3.3");
    case Stabilan01Defines::smcStabilan01_01 : return QCoreApplication::tr("Стабилан-01. Исполнение 01");
    case Stabilan01Defines::smcStabilan01_02 : return QCoreApplication::tr("Стабилан-01. Исполнение 02");
    case Stabilan01Defines::smcStabilan01_03 : return QCoreApplication::tr("Стабилан-01. Исполнение 03");
    case Stabilan01Defines::smcStabilan01_05 : return QCoreApplication::tr("Стабилан-01. Исполнение 05");
    case Stabilan01Defines::smcStabilan01_12 : return QCoreApplication::tr("Стабилан-01. Исполнение 12");
    case Stabilan01Defines::smcStabilan01_13 : return QCoreApplication::tr("Стабилан-01. Исполнение 13");
    case Stabilan01Defines::smcStabilan01_15 : return QCoreApplication::tr("Стабилан-01. Исполнение 15");
    case Stabilan01Defines::smcStabilan01_16 : return QCoreApplication::tr("Стабилан-01. Исполнение 16");
    case Stabilan01Defines::smcStabilan01_05NG : return QCoreApplication::tr("Стабилан-01. Исполнение 05 (после 2014)");
    case Stabilan01Defines::smcStabilan01_12NG : return QCoreApplication::tr("Стабилан-01. Исполнение 12 (после 2014)");
    }
    return QCoreApplication::tr("Модель неизвестная");
    //return StabilanModels.value(mdlCode);
}

QList<Stabilan01Defines::Model> Stabilan01::models()
{
    return StabilanModels.keys();
}

QString Stabilan01::zeroingTypeName(const Stabilan01Defines::ZeroingType ztCode)
{
    switch (ztCode) {
    case Stabilan01Defines::ztFast : return QCoreApplication::tr("Мгновенная");
    case Stabilan01Defines::ztAveragePrev : return QCoreApplication::tr("С усреднением после команды");
    case Stabilan01Defines::ztAverageNext : return QCoreApplication::tr("С усреднением по предыстории");
    }
    return QCoreApplication::tr("Мгновенная");
//    return ZeroingTypes.value(ztCode);
}

QList<Stabilan01Defines::ZeroingType> Stabilan01::zeroingTypes()
{
    return ZeroingTypes.keys();
}

SerialPortDefines::Settings Stabilan01::getSerialPortSettings()
{
    return SerialPortDefines::Settings(57600,
                                       QSerialPort::Data8,
                                       QSerialPort::NoParity,
                                       QSerialPort::OneStop,
                                       QSerialPort::NoFlowControl);
}

void Stabilan01::on_readData(const QByteArray data)
{
    Driver::on_readData(data);
    for (int i = 0; i < data.count(); i++)
    {
        quint8 B = static_cast<quint8>(data[i]);
        assignByteFromDevice(B);
    }
}

void Stabilan01::on_error(const QString &err)
{
    Driver::on_error(err);
    qDebug() << err;

}

//double r = 0;

void Stabilan01::assignByteFromDevice(quint8 b)
{
    //! Обработка байта реакции опоры
    auto assignBytePlate = [&](double &chan)
    {
        if (m_countBytePack % 2 == 0)
        {
            m_prevB = b;
        }
        else
        {
            chan = (b * 256 + m_prevB) / 1000.0;
        }
    };

    //! Обработка байта координат
    auto assignByteStab = [&](double &stabChan)
    {
        if (m_countBytePack % 2 == 0)
        {
            m_prevB = b;
        }
        else
        {
            if (b < 128)
                stabChan = (b * 256 + m_prevB) / 128.0;
            else
                stabChan = ((b - 256) * 256 + m_prevB) / 128.0;
        }
    };

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

    //! Обработка байта миограммы
    auto assignByteMyogram = [&](const int rec, const int chan)
    {
        if (m_countBytePack % 2 == 0)
        {
            m_prevB = b;
        }
        else
        {
            quint8 v = 0;
            if ((b & 8) == 0)
                v = b & 0x0F;
            else
                v = b | 0xF0;
            if (v < 128)
                m_myoValue[rec][chan] = (v * 256 + m_prevB);
            else
                m_myoValue[rec][chan] = ((v - 256) * 256 + m_prevB);
            m_myoValue[rec][chan] = (m_myoValue[rec][chan] + 2048) / 4096 * m_myoAmplitude + m_myoOffset[chan];
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
                    assignByteStab(m_X);
                    break;
                }
                case 1:{     //! Сагитталь Y
                    assignByteStab(m_Y);
                    break;
                }
                case 2:{       //! Реакция опоры A
                    assignBytePlate(m_A);
                    break;
                }
                case 3:{       //! Реакция опоры B
                    assignBytePlate(m_B);
                    break;
                }
                case 4:{       //! Реакция опоры C
                    assignBytePlate(m_C);
                    break;
                }
                case 5:{       //! Реакция опоры D
                    assignBytePlate(m_D);
                    break;
                }
                case 6: {          //! RR + Synchro
                    if (m_countBytePack % 2 == 0){
                        m_prevB = b;
                    } else {
                        if (((m_prevB & 0x1) != 0) && ((b & 0x2) == 0))
                        {
                            m_rrPor = b;
                            m_rrMark = true;
                            m_rrOk = true;
                        }
                        else
                            m_rrPor = 0;
//                        //! Линия синхронизации от устройства
//                        if FIsInpSynchroEnabled then
//                          AssignInpSynchro ( (Variables[7] and $6) shr 1 );
                    }
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
                    if (m_countBytePack % 2 == 0) {
                        m_prevB = b;
                    } else {
                        if (m_rrOk)
                        {
                            m_rrPor = 9 - (m_rrPor >> 2);
                            quint16 mant = ((b << 7) + (m_prevB >> 1)) | 0x8000;
                            m_valPulse = 60 / (mant * 2e-6 * exp(m_rrPor * log(2)));
                            m_rrOk = false;
                        }
                        else
                            m_valPulse = 0;
                    }
                    break;
                }
                case 11: {assignByteMyogram(0, 0); break; }  //! Миограммы 4 канала и 4 отведения
                case 12: {assignByteMyogram(0, 1); break; }
                case 13: {assignByteMyogram(0, 2); break; }
                case 14: {assignByteMyogram(0, 3); break; }
                case 15: {assignByteMyogram(1, 0); break; }
                case 16: {assignByteMyogram(1, 1); break; }
                case 17: {assignByteMyogram(1, 2); break; }
                case 18: {assignByteMyogram(1, 3); break; }
                case 19: {assignByteMyogram(2, 0); break; }
                case 20: {assignByteMyogram(2, 1); break; }
                case 21: {assignByteMyogram(2, 2); break; }
                case 22: {assignByteMyogram(2, 3); break; }
                case 23: {assignByteMyogram(3, 0); break; }
                case 24: {assignByteMyogram(3, 1); break; }
                case 25: {assignByteMyogram(3, 2); break; }
                case 26: {assignByteMyogram(3, 3); break; }
            }

            //! Окончание разбора пакета
            if (m_countBytePack == m_countChannels * 2){  //! Достигли заданного кол-ва каналов
                m_Z = m_A + m_B + m_C + m_D;                     //! Расчет баллистограммы

                //! Считаем стабилограмму, если должны
                if (m_skgSource == Stabilan01Defines::ssSelf)
                {
                    if (m_Z > 0.5)
                    {
                        m_X = (-m_A + m_B + m_C - m_D) / m_Z * 220;
                        m_Y = (m_A + m_B - m_C - m_D) / m_Z * 220;
                    }
                    else
                    {
                        m_X = 0;
                        m_Y = 0;
                    }
                }

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

void Stabilan01::sendDataBlock()
{
    sendStab();
    sendPulse();
    sendTensoChannels();
    sendMyogram();
}

void Stabilan01::sendStab()
{
    //! Эмуляция стабилограммы кругом
//                m_X = 10 * sin(r);
//                m_Y = 10 * cos(r);
//                r = r + 2 * M_PI / 50;

    //! Передача стабилограммы
    auto stabData = new DeviceProtocols::StabDvcData(this, ChannelsDefines::chanStab,
                                                     m_X - m_offsetX, m_Y - m_offsetY,
                                                     m_A, m_B, m_C, m_D);
    emit sendData(stabData);
    delete stabData;
}

void Stabilan01::sendPulse()
{
    if (m_valPulse > 0)
    {
        auto pulseData = new DeviceProtocols::PulseDvcData(this,
                                                           ChannelsDefines::chanRitmogram,
                                                           m_valPulse);
        emit sendData(pulseData);
        delete pulseData;
    }
}

void Stabilan01::sendMyogram()
{
    QVector<double> data;
    data.resize(4);

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            data[j] = m_myoValue[i][j];
        auto myogrData = new DeviceProtocols::MyogramDvcData(this, ChannelsDefines::chanMyogram, data);
        emit sendData(myogrData);
        delete myogrData;
    }
}

void Stabilan01::sendTensoChannels()
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

QMap<QString, bool> Stabilan01::getChanRecordingDefault(const QJsonObject &obj)
{
    QMap<QString, bool> retval;
    retval.insert(ChannelsDefines::chanStab, obj["stab"].toBool());
    retval.insert(ChannelsDefines::chanZ, obj["z"].toBool());
    retval.insert(ChannelsDefines::chanMyogram, obj["myo"].toBool());
    retval.insert(ChannelsDefines::chanRitmogram, obj["pulse"].toBool());
    retval.insert(ChannelsDefines::chanTenso1, obj["tenso1"].toBool());
    retval.insert(ChannelsDefines::chanTenso2, obj["tenso2"].toBool());
    retval.insert(ChannelsDefines::chanTenso3, obj["tenso3"].toBool());

    auto myoSubChans = obj["myo_chans"].toArray();
    for (int i = 0; i < myoSubChans.size(); ++i)
    {
        auto sco = myoSubChans.at(i).toObject();
        retval.insert("MyoChan" + QString::number(i), sco["recorded"].toBool());
    }

    return retval;
}

QJsonObject Stabilan01::setChanRecordingDefault(const QMap<QString, bool> &recMap)
{
    QJsonObject recObj;
    recObj["stab"] = recMap.value(ChannelsDefines::chanStab);
    recObj["z"] = recMap.value(ChannelsDefines::chanZ);
    recObj["pulse"] = recMap.value(ChannelsDefines::chanRitmogram);
    recObj["myo"] = recMap.value(ChannelsDefines::chanMyogram);
    recObj["tenso1"] = recMap.value(ChannelsDefines::chanTenso1);
    recObj["tenso2"] = recMap.value(ChannelsDefines::chanTenso2);
    recObj["tenso3"] = recMap.value(ChannelsDefines::chanTenso3);

    QJsonArray myoSubChan;
    for (int i = 0; i < 4; ++i)
    {
        QJsonObject sco;
        sco["recorded"] = recMap.value("MyoChan" + QString::number(i));
        myoSubChan.append(sco);
    }
    recObj["myo_chans"] = myoSubChan;

    return recObj;
}

