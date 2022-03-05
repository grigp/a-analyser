#include "bedsidescales01.h"

#include "aanalyserapplication.h"
#include "bedsidescales01paramsdialog.h"

BedsideScales01::BedsideScales01(QObject *parent)
    : Driver(parent)
{

}

void BedsideScales01::setParams(const DeviceProtocols::Ports port, const QJsonObject &params)
{
    setPortName(port);
    Q_UNUSED(params);
}

bool BedsideScales01::editParams(QJsonObject &params)
{
    Q_UNUSED(params);
    BedsideScales01ParamsDialog dlg(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow());

    if (dlg.exec() == QDialog::Accepted)
    {

        return true;
    }
    return false;
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
    return QStringList() << DeviceProtocols::uid_WeightPlateProtocol;
}

QList<DeviceProtocols::Ports> BedsideScales01::getPorts()
{
    return DeviceProtocols::comPorts();
}

void BedsideScales01::calibrateTenso(const QString &channelUid)
{

}

void BedsideScales01::getTensoValueDiapasone(const int chanNumber, double &min, double &max)
{

}

void BedsideScales01::getTensoValueDiapasone(const QString channelId, double &min, double &max)
{

}

void BedsideScales01::setTensoValueDiapasone(const int chanNumber, const double min, const double max)
{

}
