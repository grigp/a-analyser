#include "bilateral.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "deviceprotocols.h"
#include "bilateralparamsdialog.h"
#include "channelsutils.h"
#include "settingsprovider.h"

namespace  {

static QMap<QString, QString> ChannelFirstConvert =
{
      std::pair<QString, QString> (ChannelsDefines::chanMyogram, ChannelsDefines::FirstPlatform::chanMyogram)
    , std::pair<QString, QString> (ChannelsDefines::chanRitmogram, ChannelsDefines::FirstPlatform::chanRitmogram)
    , std::pair<QString, QString> (ChannelsDefines::chanDynHand1, ChannelsDefines::FirstPlatform::chanDynHand1)
    , std::pair<QString, QString> (ChannelsDefines::chanDynStand1, ChannelsDefines::FirstPlatform::chanDynStand1)
    , std::pair<QString, QString> (ChannelsDefines::chanBreath1, ChannelsDefines::FirstPlatform::chanBreath1)
    , std::pair<QString, QString> (ChannelsDefines::chanDynPush1, ChannelsDefines::FirstPlatform::chanDynPush1)
    , std::pair<QString, QString> (ChannelsDefines::chanDynHand2, ChannelsDefines::FirstPlatform::chanDynHand2)
    , std::pair<QString, QString> (ChannelsDefines::chanDynStand2, ChannelsDefines::FirstPlatform::chanDynStand2)
    , std::pair<QString, QString> (ChannelsDefines::chanBreath2, ChannelsDefines::FirstPlatform::chanBreath2)
    , std::pair<QString, QString> (ChannelsDefines::chanDynPush2, ChannelsDefines::FirstPlatform::chanDynPush2)
    , std::pair<QString, QString> (ChannelsDefines::chanDynHand3, ChannelsDefines::FirstPlatform::chanDynHand3)
    , std::pair<QString, QString> (ChannelsDefines::chanDynStand3, ChannelsDefines::FirstPlatform::chanDynStand3)
    , std::pair<QString, QString> (ChannelsDefines::chanBreath3, ChannelsDefines::FirstPlatform::chanBreath3)
    , std::pair<QString, QString> (ChannelsDefines::chanDynPush3, ChannelsDefines::FirstPlatform::chanDynPush3)
};

static QList<QString> ChannelFirst = {
    ChannelsDefines::FirstPlatform::chanMyogram
  , ChannelsDefines::FirstPlatform::chanRitmogram
  , ChannelsDefines::FirstPlatform::chanDynHand1
  , ChannelsDefines::FirstPlatform::chanDynStand1
  , ChannelsDefines::FirstPlatform::chanBreath1
  , ChannelsDefines::FirstPlatform::chanDynPush1
  , ChannelsDefines::FirstPlatform::chanDynHand2
  , ChannelsDefines::FirstPlatform::chanDynStand2
  , ChannelsDefines::FirstPlatform::chanBreath2
  , ChannelsDefines::FirstPlatform::chanDynPush2
  , ChannelsDefines::FirstPlatform::chanDynHand3
  , ChannelsDefines::FirstPlatform::chanDynStand3
  , ChannelsDefines::FirstPlatform::chanBreath3
  , ChannelsDefines::FirstPlatform::chanDynPush3
};

static QMap<QString, QString> ChannelSecondConvert =
{
      std::pair<QString, QString> (ChannelsDefines::chanMyogram, ChannelsDefines::SecondPlatform::chanMyogram)
    , std::pair<QString, QString> (ChannelsDefines::chanRitmogram, ChannelsDefines::SecondPlatform::chanRitmogram)
    , std::pair<QString, QString> (ChannelsDefines::chanDynHand1, ChannelsDefines::SecondPlatform::chanDynHand1)
    , std::pair<QString, QString> (ChannelsDefines::chanDynStand1, ChannelsDefines::SecondPlatform::chanDynStand1)
    , std::pair<QString, QString> (ChannelsDefines::chanBreath1, ChannelsDefines::SecondPlatform::chanBreath1)
    , std::pair<QString, QString> (ChannelsDefines::chanDynPush1, ChannelsDefines::SecondPlatform::chanDynPush1)
    , std::pair<QString, QString> (ChannelsDefines::chanDynHand2, ChannelsDefines::SecondPlatform::chanDynHand2)
    , std::pair<QString, QString> (ChannelsDefines::chanDynStand2, ChannelsDefines::SecondPlatform::chanDynStand2)
    , std::pair<QString, QString> (ChannelsDefines::chanBreath2, ChannelsDefines::SecondPlatform::chanBreath2)
    , std::pair<QString, QString> (ChannelsDefines::chanDynPush2, ChannelsDefines::SecondPlatform::chanDynPush2)
    , std::pair<QString, QString> (ChannelsDefines::chanDynHand3, ChannelsDefines::SecondPlatform::chanDynHand3)
    , std::pair<QString, QString> (ChannelsDefines::chanDynStand3, ChannelsDefines::SecondPlatform::chanDynStand3)
    , std::pair<QString, QString> (ChannelsDefines::chanBreath3, ChannelsDefines::SecondPlatform::chanBreath3)
    , std::pair<QString, QString> (ChannelsDefines::chanDynPush3, ChannelsDefines::SecondPlatform::chanDynPush3)
};

static QList<QString> ChannelSecond = {
    ChannelsDefines::SecondPlatform::chanMyogram
  , ChannelsDefines::SecondPlatform::chanRitmogram
  , ChannelsDefines::SecondPlatform::chanDynHand1
  , ChannelsDefines::SecondPlatform::chanDynStand1
  , ChannelsDefines::SecondPlatform::chanBreath1
  , ChannelsDefines::SecondPlatform::chanDynPush1
  , ChannelsDefines::SecondPlatform::chanDynHand2
  , ChannelsDefines::SecondPlatform::chanDynStand2
  , ChannelsDefines::SecondPlatform::chanBreath2
  , ChannelsDefines::SecondPlatform::chanDynPush2
  , ChannelsDefines::SecondPlatform::chanDynHand3
  , ChannelsDefines::SecondPlatform::chanDynStand3
  , ChannelsDefines::SecondPlatform::chanBreath3
  , ChannelsDefines::SecondPlatform::chanDynPush3
};

}

Bilateral::Bilateral(QObject *parent)
    : Driver (parent)
{
    setConversMaps(ChannelFirstConvert, m_cfc, m_cfcB);
    setConversMaps(ChannelSecondConvert, m_csc, m_cscB);
    clearDrivers();
}

Bilateral::~Bilateral()
{

}

void Bilateral::setParams(const DeviceProtocols::Ports port, const QJsonObject &params)
{
    setPortName(port);

    auto obj = params["plate1"].toObject();
    auto x = obj["x"].toInt(0);
    auto y = obj["y"].toInt(500);
    auto w = obj["width"].toInt(500);
    auto h = obj["height"].toInt(500);
    m_plate1 = QRect(x, y, w, h);

    obj = params["plate2"].toObject();
    x = obj["x"].toInt(500);
    y = obj["y"].toInt(500);
    w = obj["width"].toInt(500);
    h = obj["height"].toInt(500);
    m_plate2 = QRect(x, y, w, h);

    centeringPlatforms();

    m_center = QPoint((m_plate1.center().x() + m_plate2.center().x()) / 2,
                      (m_plate1.center().y() + m_plate2.center().y()) / 2);
}

bool Bilateral::editParams(QJsonObject &params)
{
    BilateralParamsDialog dlg(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow());

    auto obj = params["plate1"].toObject();
    auto x = obj["x"].toInt(0);
    auto y = obj["y"].toInt(500);
    auto w = obj["width"].toInt(500);
    auto h = obj["height"].toInt(500);
    dlg.setPlate1Pos(QPoint(x, y));
    dlg.setPlate1Size(QSize(w, h));

    obj = params["plate2"].toObject();
    x = obj["x"].toInt(500);
    y = obj["y"].toInt(500);
    w = obj["width"].toInt(500);
    h = obj["height"].toInt(500);
    dlg.setPlate2Pos(QPoint(x, y));
    dlg.setPlate2Size(QSize(w, h));

    if (dlg.exec() == QDialog::Accepted)
    {
        auto pos = dlg.getPlate1Pos();
        auto size = dlg.getPlate1Size();
        QJsonObject objPlate1;
        objPlate1["x"] = pos.x();
        objPlate1["y"] = pos.y();
        objPlate1["width"] = size.width();
        objPlate1["height"] = size.height();
        params["plate1"] = objPlate1;

        pos = dlg.getPlate2Pos();
        size = dlg.getPlate2Size();
        QJsonObject objPlate2;
        objPlate2["x"] = pos.x();
        objPlate2["y"] = pos.y();
        objPlate2["width"] = size.width();
        objPlate2["height"] = size.height();
        params["plate2"] = objPlate2;

        return true;
    }
    return false;
}

void Bilateral::start()
{
    init();

    if (m_drivers[0] && m_drivers[1])
    {

        for (int i = 0; i < 2; ++i)
        {
            connect(m_drivers[i], &Driver::sendData, this, &Bilateral::getData);
            connect(m_drivers[i], &Driver::communicationError, this, &Bilateral::communicationError);

            m_stabControl[i] = dynamic_cast<DeviceProtocols::StabControl*>(m_drivers[i]);
            auto listChanUid = m_drivers[i]->getChannelsByFormat(ChannelsDefines::cfDecartCoordinates);
            if (listChanUid.size() > 0)
                m_chanUid[i] = listChanUid.at(0);

            m_drivers[i]->start();
        }
    }

    emit started();
}

void Bilateral::stop()
{
    emit stopped();

    if (m_drivers[0])
    {
        m_drivers[0]->stop();
        m_drivers[0]->deleteLater();
    }

    if (m_drivers[1])
    {
        m_drivers[1]->stop();
        m_drivers[1]->deleteLater();
    }
}

int Bilateral::frequency(const QString &channelId) const
{
    auto drivers = getDrivers();
    int retval = 50;
    if (drivers.size() == 2)
    {
        if (ChannelFirst.contains(channelId) && drivers[0])
            retval = drivers[0]->frequency(channelId);
        else
        if (ChannelSecond.contains(channelId) && drivers[1])
            retval = drivers[1]->frequency(channelId);
    }
    for (int i = 0; i < drivers.size(); ++i)
        delete drivers[i];

    return retval;
}

QList<QString> Bilateral::getChannelsByProtocol(const QString &protocolUid) const
{
    Q_UNUSED(protocolUid);
    return QList<QString>();
}

QList<QString> Bilateral::getChannelsByFormat(const QString &formatUid) const
{
    QList<QString> retval;
    retval.clear();

    if (formatUid == ChannelsDefines::cfDecartCoordinates)
        retval << ChannelsDefines::chanStab << ChannelsDefines::chanStabLeft << ChannelsDefines::chanStabRight;
    else
    {
        auto drivers = getDrivers();
        if (drivers.size() == 2)
            for (int i = 0; i < 2; ++i)
                if (drivers[i])
                {
                    auto channels = drivers[i]->getChannelsByFormat(formatUid);
                    foreach (auto chan, channels)
                        if (ChannelsUtils::instance().channelType(chan) != ChannelsDefines::ctStabilogram)
                        {
                            QString bChan = chan;
                            if (i == 0)
                                bChan = m_cfc.value(chan);
                            else
                            if (i == 1)
                                bChan = m_csc.value(chan);
                            retval << bChan;
                        }
                }

        for (int i = 0; i < drivers.size(); ++i)
            delete drivers[i];
    }

    return retval;
}

QList<QString> Bilateral::getChannels() const
{
    QList<QString> retval;

    retval.clear();

    retval << ChannelsDefines::chanStab << ChannelsDefines::chanStabLeft << ChannelsDefines::chanStabRight;

    auto drivers = getDrivers();
    if (drivers.size() == 2)
        for (int i = 0; i < 2; ++i)
            if (drivers[i])
            {
                auto channels = drivers[i]->getChannels();
                foreach (auto chan, channels)
                    if (ChannelsUtils::instance().channelType(chan) != ChannelsDefines::ctStabilogram)
                    {
                        QString bChan = chan;
                        if (i == 0)
                            bChan = m_cfc.value(chan);
                        else
                        if (i == 1)
                            bChan = m_csc.value(chan);
                        retval << bChan;
                    }
            }

    for (int i = 0; i < drivers.size(); ++i)
        delete drivers[i];
    return retval;
}

int Bilateral::getSubChannelsCount(const QString &channelUid) const
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

bool Bilateral::isChannelRecordingDefault(const QString &channelUid, const int subChan) const
{
    if (ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctStabilogram ||
            ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctBalistogram)
        return true;
    else
    {
        bool retval = false;
        if (m_drivers[0] && m_cfcB.contains(channelUid))
            retval = m_drivers[0]->isChannelRecordingDefault(m_cfcB.value(channelUid), subChan);
        if (!retval && m_drivers[1] && m_cscB.contains(channelUid))
            retval = m_drivers[1]->isChannelRecordingDefault(m_cscB.value(channelUid), subChan);
        return retval;
    }
}

DeviceProtocols::DeviceControl *Bilateral::getDeviceControl(const QString &controlId, const QString &channelId)
{
    if (controlId == DeviceProtocols::uid_CommonControl)
        return dynamic_cast<DeviceProtocols::CommonControl*>(this);
    else
    if (controlId == DeviceProtocols::uid_StabControl)
        return dynamic_cast<DeviceProtocols::StabControl*>(this);
    else
    if (controlId == DeviceProtocols::uid_MultiPlatformControl)
            return dynamic_cast<DeviceProtocols::MultiPlatformControl*>(this);
    else
    if (ChannelFirst.contains(channelId))
    {
        if (m_drivers[0])
            return  m_drivers[0]->getDeviceControl(controlId);
    }
    else
    if (ChannelSecond.contains(channelId))
    {
        if (m_drivers[1])
            return  m_drivers[1]->getDeviceControl(controlId);
    }
    return nullptr;
}

QStringList Bilateral::getProtocols()
{
    return QStringList() << DeviceProtocols::uid_StabProtocol;
}

QList<DeviceProtocols::Ports> Bilateral::getPorts()
{
    return QList<DeviceProtocols::Ports>();
}

void Bilateral::getData(DeviceProtocols::DeviceData *data)
{
    if (data->sender() == m_drivers[0] && data->channelId() == m_chanUid[0])
    {
        DeviceProtocols::StabDvcData *stabData = static_cast<DeviceProtocols::StabDvcData*>(data);
        m_x1 = stabData->x();
        m_y1 = stabData->y();
        m_a1 = stabData->a();
        m_b1 = stabData->b();
        m_c1 = stabData->c();
        m_d1 = stabData->d();
        m_isData1 = true;
        assignStabData();
    }
    else
    if (data->sender() == m_drivers[1] && data->channelId() == m_chanUid[1])
    {
        DeviceProtocols::StabDvcData *stabData = static_cast<DeviceProtocols::StabDvcData*>(data);
        m_x2 = stabData->x();
        m_y2 = stabData->y();
        m_a2 = stabData->a();
        m_b2 = stabData->b();
        m_c2 = stabData->c();
        m_d2 = stabData->d();
        m_isData2 = true;
        assignStabData();
    }
    else
        sendAdvancedChannels(data);
}

void Bilateral::centeringPlatforms()
{
    auto calcRectMinMax = [&](QRect plate1,
                              QRect plate2,
                              int &xMin, int &xMax, int &yMin, int &yMax)
    {
        xMin = plate1.x();
        if (plate2.x() < plate1.x())
            xMin = plate2.x();
        xMax = plate1.x() + plate1.width();
        if (plate2.x() + plate2.width() > plate1.x() + plate1.width())
            xMax = plate2.x() + plate2.width();

        yMax = plate1.y();
        if (plate2.y() > plate1.y())
            yMax = plate2.y();
        yMin = plate1.y() - plate1.height();
        if (plate2.y() - plate2.height() < plate1.y() - plate1.height())
            yMin = plate2.y() - plate2.height();
    };

    QRect plate1 = m_plate1;
    QRect plate2 = m_plate2;

    int xMin = 0;
    int xMax = 0;
    int yMin = 0;
    int yMax = 0;
    calcRectMinMax(plate1, plate2, xMin, xMax, yMin, yMax);

    int xMid = (xMax + xMin) / 2;
    int yMid = (yMax + yMin) / 2;

    m_plate1 = QRect(plate1.x() - xMid, plate1.y() - yMid, plate1.width(), plate1.height());
    m_plate2 = QRect(plate2.x() - xMid, plate2.y() - yMid, plate2.width(), plate2.height());

    calcRectMinMax(m_plate1, m_plate2, xMin, xMax, yMin, yMax);

    m_diap = qMax(abs(xMin), abs(xMax));
    m_diap = qMax(m_diap, abs(yMin));
    m_diap = qMax(m_diap, abs(yMax));
}

void Bilateral::assignStabData()
{
    if (m_isData1 && m_isData2)
    {
        //! Левая
        auto sdLeft = new DeviceProtocols::StabDvcData(this, ChannelsDefines::chanStabLeft,
                                                         m_x1 - m_offsetX, m_y1 - m_offsetY,
                                                         m_a1, m_b1, m_c1, m_d1);
        emit sendData(sdLeft);
        delete sdLeft;

        //! Правая
        auto sdRight = new DeviceProtocols::StabDvcData(this, ChannelsDefines::chanStabRight,
                                                         m_x2 - m_offsetX, m_y2 - m_offsetY,
                                                         m_a2, m_b2, m_c2, m_d2);
        emit sendData(sdRight);
        delete sdRight;

        //! Общая. Расчет
        double z1 = m_a1 + m_b1 + m_c1 + m_d1;
        double z2 = m_a2 + m_b2 + m_c2 + m_d2;

        m_x = (z1 * ((m_plate1.center().x() - m_center.x()) + m_x1) +
               z2 * ((m_plate2.center().x() - m_center.x()) + m_x2));
        m_y = (z1 * ((m_plate1.center().y() - m_center.y()) + m_y1) +
               z2 * ((m_plate2.center().y() - m_center.y()) + m_y2));
        if (z1 + z2 > 0.5)
        {
            m_x = m_x / (z1 + z2);
            m_y = m_y / (z1 + z2);
        }
        else
        {
            m_x = m_x / 0.5;
            m_y = m_y / 0.5;
        }

        //! Общая. Передача
        auto sd = new DeviceProtocols::StabDvcData(this, ChannelsDefines::chanStab,
                                                   m_x - m_offsetX, m_y - m_offsetY,
                                                   z1 + z2);
        emit sendData(sd);
        delete sd;

        m_isData1 = false;
        m_isData2 = false;
    }
}

void Bilateral::sendAdvancedChannels(DeviceProtocols::DeviceData *data)
{
    auto sendWithChangedId = [&](QMap<QString, QString>& convertMap)
    {
        auto bChan = convertMap.value(data->channelId());
        if (bChan != "")
        {
            data->changeChannelId(bChan);
            emit sendData(data);
        }
    };

    if (data->sender() == m_drivers[0])
        sendWithChangedId(m_cfc);
    else
    if (data->sender() == m_drivers[1])
        sendWithChangedId(m_csc);
}

void Bilateral::clearDrivers()
{
    for (int i = 0; i < 2; ++i)
    {
        m_drivers[i] = nullptr;
        m_chanUid[i] = "";
    }
}

void Bilateral::init()
{
    if (!(m_drivers[0] || m_drivers[1]))
    {
        auto drivers = getDrivers();
        if (drivers.size() == 2)
            for (int i = 0; i < 2; ++i)
                m_drivers[i] = drivers[i];
        else
            for (int i = 0; i < drivers.size(); ++i)
                delete drivers[i];


//        //! Запрашиваем два верхних драйвера в списке подключений
//        Driver* drv = nullptr;
//        int n = 0;
//        int i = 0;
//        do
//        {
//            drv = static_cast<AAnalyserApplication*>(QApplication::instance())->
//                    getDriverByFormats(QStringList() << ChannelsDefines::cfDecartCoordinates, i);
//            //! Любой, но не билатеральный
//            if (drv && (drv->driverUid() != DevicesDefines::uid_bilateral))
//            {
//                m_drivers[n] = drv;
//                ++n;
//            }
//            else
//                delete drv;

//            ++i;
//        }
//        while (n < 2 && drv);
    }
}

QList<Driver *> Bilateral::getDrivers() const
{
    QList<Driver*> retval;
    retval.clear();
    //! Запрашиваем два верхних драйвера в списке подключений
    Driver* drv = nullptr;
    int n = 0;
    int i = 0;
    do
    {
        drv = static_cast<AAnalyserApplication*>(QApplication::instance())->
                getDriverByFormats(QStringList() << ChannelsDefines::cfDecartCoordinates, i);
        //! Любой, но не билатеральный
        if (drv && (drv->driverUid() != DevicesDefines::uid_bilateral))
        {
            retval << drv;
            ++n;
        }
        else
            delete drv;

        ++i;
    }
    while (n < 2 && drv);

    return retval;
}

void Bilateral::setConversMaps(const QMap<QString, QString> original,
                               QMap<QString, QString> &forvard,
                               QMap<QString, QString> &backward)
{
    forvard = original;
    backward.clear();
    foreach (auto key, original.keys())
        backward.insert(original.value(key), key);
}


void Bilateral::calibrate(const QString &channelUid)
{
    if (channelUid == ChannelsDefines::chanStabLeft)
        m_stabControl[0]->calibrate(ChannelsDefines::chanStab);
    else
    if (channelUid == ChannelsDefines::chanStabRight)
        m_stabControl[1]->calibrate(ChannelsDefines::chanStab);
    else
    if (channelUid == ChannelsDefines::chanStab)
    {
        m_stabControl[0]->calibrate(ChannelsDefines::chanStab);
        m_stabControl[1]->calibrate(ChannelsDefines::chanStab);
    }
}

void Bilateral::zeroing(const QString &channelUid)
{
    Q_UNUSED(channelUid);
    m_offsetX = m_x;
    m_offsetY = m_y;
    SettingsProvider::setValueToRegAppCopy("StabilanDriver", "zeroing_x", m_offsetX);
    SettingsProvider::setValueToRegAppCopy("StabilanDriver", "zeroing_y", m_offsetY);
}

QSize Bilateral::stabSize()
{
    int xMin = m_plate1.x();
    if (m_plate2.x() < xMin)
        xMin = m_plate2.x();
    int xMax = m_plate1.x() + m_plate1.width();
    if (m_plate2.x() + m_plate2.width() > xMax)
        xMax = m_plate2.x() + m_plate2.width();

    int yMin = m_plate1.y();
    if (m_plate2.y() < yMin)
        yMin = m_plate2.y();
    int yMax = m_plate1.y() + m_plate1.height();
    if (m_plate2.y() + m_plate2.height() > yMax)
        yMax = m_plate2.y() + m_plate2.height();

    return QSize(xMax - xMin, yMax - yMin);
}

QRect Bilateral::platform(const int num)
{
    Q_ASSERT(num >= 0 && num < 2);
    if (num == 0)
        return m_plate1;
    if (num == 1)
        return m_plate2;
    return QRect(0, 0, 0, 0);
}

int Bilateral::size() const
{
    return m_diap;
}

void Bilateral::calibratePlatform(const int num) const
{
    Q_ASSERT(num >= 0 && num < 2);
    if (m_stabControl[num])
        m_stabControl[num]->calibrate(ChannelsDefines::chanStab);
}

