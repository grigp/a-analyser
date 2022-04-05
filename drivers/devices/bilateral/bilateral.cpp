#include "bilateral.h"


#include "aanalyserapplication.h"
#include "deviceprotocols.h"
#include "bilateralparamsdialog.h"

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
    m_plate1Pos = QPoint(x, y);
    m_plate1Size = QSize(w, h);

    obj = params["plate2"].toObject();
    x = obj["x"].toInt(500);
    y = obj["y"].toInt(500);
    w = obj["width"].toInt(500);
    h = obj["height"].toInt(500);
    m_plate2Pos = QPoint(x, y);
    m_plate2Size = QSize(w, h);
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
    return QList<QString>();
}

QList<QString> Bilateral::getChannels() const
{
    return QList<QString>();
}

int Bilateral::getSubChannelsCount(const QString &channelUid) const
{
    return 0;
}

bool Bilateral::isChannelRecordingDefault(const QString &channelUid) const
{
    return false;
}

QStringList Bilateral::getProtocols()
{
    return QStringList();
}

QList<DeviceProtocols::Ports> Bilateral::getPorts()
{
    return QList<DeviceProtocols::Ports>();
}

//void Bilateral::calibrate(const QString &channelUid)
//{

//}

//void Bilateral::zeroing(const QString &channelUid)
//{

//}

//void Bilateral::calibrateTenso(const QString &channelUid)
//{

//}

//void Bilateral::getTensoValueDiapasone(const int chanNumber, double &min, double &max)
//{

//}

//void Bilateral::getTensoValueDiapasone(const QString channelId, double &min, double &max)
//{

//}

//void Bilateral::setTensoValueDiapasone(const int chanNumber, const double min, const double max)
//{

//}

//void Bilateral::setBoundsDelArtifacts(const int low, const int high)
//{

//}

//void Bilateral::zeroingMyo()
//{

//}

//void Bilateral::zeroingMyo(const int channel)
//{

//}

//double Bilateral::amplitudeMyo()
//{

//}
