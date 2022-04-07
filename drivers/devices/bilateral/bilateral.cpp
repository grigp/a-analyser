#include "bilateral.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "deviceprotocols.h"
#include "bilateralparamsdialog.h"
#include "channelsutils.h"
#include "settingsprovider.h"

Bilateral::Bilateral(QObject *parent)
    : Driver (parent)
{

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
    for (int i = 0; i < 2; ++i)
    {
        m_drivers[i] = nullptr;
        m_chanUid[i] = "";
    }
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
            m_drivers[n] = drv;
            ++n;
        }
        ++i;
    }
    while (n < 2 && drv);

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
}

void Bilateral::stop()
{

}

int Bilateral::frequency(const QString &channelId) const
{
    return 50;
}

QList<QString> Bilateral::getChannelsByProtocol(const QString &protocolUid) const
{
    return QList<QString>();
}

QList<QString> Bilateral::getChannelsByFormat(const QString &formatUid) const
{
    QList<QString> retval;
    if (formatUid == ChannelsDefines::cfDecartCoordinates)
        retval << ChannelsDefines::chanStab << ChannelsDefines::chanStabLeft << ChannelsDefines::chanStabRight;
    //!TODO дописать для других каналов
    return retval;
}

QList<QString> Bilateral::getChannels() const
{
    QList<QString> retval;

    retval.clear();
    retval << ChannelsDefines::chanStab << ChannelsDefines::chanStabLeft << ChannelsDefines::chanStabRight;

    //!TODO дописать для других каналов
//    for (int i = 0; i < 2; ++i)
//    {
//        if (m_drivers[i])
//        {
//            auto channels = m_drivers[i]->getChannels();
//            foreach (auto chan, channels)
//                if (ChannelsUtils::instance().channelType(chan) != ChannelsDefines::ctStabilogram)
//                    retval << chan;
//        }
//    }
    return retval;
}

int Bilateral::getSubChannelsCount(const QString &channelUid) const
{
    if (ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctStabilogram ||
        ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctBallistogram ||
        ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctDynamo ||
        ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctBreath)
        return 1;
    else
    if (ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctMyogram)
        return 4;
    return 0;
}

bool Bilateral::isChannelRecordingDefault(const QString &channelUid) const
{
    return false;
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
        emit sendData(data);
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

