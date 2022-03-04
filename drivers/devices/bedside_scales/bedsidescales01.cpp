#include "bedsidescales01.h"

BedsideScales01::BedsideScales01(QObject *parent)
    : Driver(parent)
{

}

void BedsideScales01::setParams(const DeviceProtocols::Ports port, const QJsonObject &params)
{

}

bool BedsideScales01::editParams(QJsonObject &params)
{

}

void BedsideScales01::start()
{

}

void BedsideScales01::stop()
{

}

int BedsideScales01::frequency(const QString &channelId) const
{

}

QList<QString> BedsideScales01::getChannelsByProtocol(const QString &protocolUid) const
{

}

QList<QString> BedsideScales01::getChannelsByFormat(const QString &formatUid) const
{

}

QList<QString> BedsideScales01::getChannels() const
{

}

int BedsideScales01::getSubChannelsCount(const QString &channelUid) const
{

}

bool BedsideScales01::isChannelRecordingDefault(const QString &channelUid) const
{

}

QStringList BedsideScales01::getProtocols()
{

}

QList<DeviceProtocols::Ports> BedsideScales01::getPorts()
{

}
