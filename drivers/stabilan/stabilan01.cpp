#include "stabilan01.h"

#include "stabilan01paramsdialog.h"
#include "aanalyserapplication.h"

#include <QApplication>
#include <QJsonObject>
#include <QDebug>

namespace  {
  QMap<Stabilan01Defines::Model, QString> StabilanModels =
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

  QMap<Stabilan01Defines::ZeroingType, QString> ZeroingTypes =
  {
      std::pair<Stabilan01Defines::ZeroingType, QString> (Stabilan01Defines::ztFast, Stabilan01Defines::ztnFast)
    , std::pair<Stabilan01Defines::ZeroingType, QString> (Stabilan01Defines::ztAveragePrev, Stabilan01Defines::ztnAveragePrev)
    , std::pair<Stabilan01Defines::ZeroingType, QString> (Stabilan01Defines::ztAverageNext, Stabilan01Defines::ztnAverageNext)
  };

}

///< -----------------------------------------------------------------------------------
///< Поток чтения данных

void ReadingDataStabilan01::run()
{
    m_isReading = true;
    do
    {

        sleep(2);
    }
    while (m_isReading);
}

void ReadingDataStabilan01::stop()
{
    m_isReading = false;
}


///< -----------------------------------------------------------------------------------
///< Сам драйвер

Stabilan01::Stabilan01(QObject *parent)
    : Driver(parent)
{

}

void Stabilan01::setParams(const DeviceProtocols::Ports port, const QJsonObject &params)
{
    m_port = port;
    m_model = static_cast<Stabilan01Defines::Model>(params["model"].toInt());
    m_zt = static_cast<Stabilan01Defines::ZeroingType>(params["zeroing_type"].toInt());
}

bool Stabilan01::editParams(QJsonObject &params)
{
    auto model = params["model"].toInt();
    auto zt = params["zeroing_type"].toInt();

    Stabilan01ParamsDialog dlg(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow());
    dlg.setModel(static_cast<Stabilan01Defines::Model>(model));
    dlg.setZeroingType(static_cast<Stabilan01Defines::ZeroingType>(zt));
    if (dlg.exec() == QDialog::Accepted)
    {
        params["model"] = static_cast<int>(dlg.model());
        params["zeroing_type"] = static_cast<int>(dlg.zeroingType());
        return true;
    }
    return false;
}

void Stabilan01::start()
{
    if (!m_readData)
    {
        auto *m_readData = new ReadingDataStabilan01();
        connect(m_readData, &ReadingDataStabilan01::dataExists, this, &Stabilan01::on_readData);
        connect(m_readData, &ReadingDataStabilan01::finished, m_readData, &QObject::deleteLater);
        m_readData->start();
    }
}

void Stabilan01::stop()
{
    if (m_readData)
        m_readData->stop();
}

QStringList Stabilan01::getProtocols()
{
    return QStringList() << DeviceProtocols::uid_StabProtocol;
}

QList<DeviceProtocols::Ports> Stabilan01::getPorts()
{
    return DeviceProtocols::comPorts();
}

void Stabilan01::calibrate()
{

}

void Stabilan01::zeroing()
{

}

QString Stabilan01::modelName(const Stabilan01Defines::Model mdlCode)
{
    return StabilanModels.value(mdlCode);
}

QList<Stabilan01Defines::Model> Stabilan01::models()
{
    return StabilanModels.keys();
}

QString Stabilan01::zeroingTypeName(const Stabilan01Defines::ZeroingType ztCode)
{
    return ZeroingTypes.value(ztCode);
}

QList<Stabilan01Defines::ZeroingType> Stabilan01::zeroingTypes()
{
    return ZeroingTypes.keys();
}

void Stabilan01::on_readData()
{

}

