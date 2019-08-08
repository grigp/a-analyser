#include "stabilan01.h"

#include "stabilan01paramsdialog.h"
#include "aanalyserapplication.h"
#include "serialport.h"

#include <QApplication>
#include <QJsonObject>
#include <QtMath>
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

  const quint8 MarkerValue = 0x80;
}


Stabilan01::Stabilan01(QObject *parent)
    : Driver(parent)
{

}

void Stabilan01::setParams(const DeviceProtocols::Ports port, const QJsonObject &params)
{
    setPortName(port);
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
    Driver::start();
}

void Stabilan01::stop()
{
    Driver::stop();
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
    QByteArray cmd;
    cmd.resize(2);
    cmd[0] = 0x33;
    cmd[1] = 0x0;
    emit writeData(cmd);
}

void Stabilan01::zeroing()
{
    m_offsetX = m_X;
    m_offsetY = m_Y;
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

void Stabilan01::on_readData(const QByteArray data)
{
    Driver::on_readData(data);
    for (int i = 0; i < data.count(); i++)
    {
        quint8 B = data[i];
        assignByteFromDevice(B);
    }
}

void Stabilan01::on_error(const QString &err)
{
    Driver::on_error(err);
    qDebug() << err;

}

void Stabilan01::assignByteFromDevice(quint8 b)
{
    bool isTwoMarkers = false;
    bool isError = false;

    if (b == MarkerValue)    // Пришел байт маркера
    {
        if (!m_isMarker){    // Ожидание первого байта маркера
            m_isMarker = true;
        } else {           // Ожидание второго байта маркера
            if (m_isPackage){   // Два маркера внутри пакета - ошибка
                isError = true;
            }
            m_isMarker = false;
            m_isPackage = true;    // Признак начала приема пакета
            m_countBytePack = 0;
            isTwoMarkers = true;
        }
    } else {            // Не байт маркера
        m_isMarker = false;
    }

    if (!isTwoMarkers){    // Если не было два маркера подряд, то эта ситуация внутри пакета и нам надо это отрабатывать
        if (m_isPackage){
            switch (m_countBytePack / 2) {
                case 0:{     // Фронталь X
                    if (m_countBytePack % 2 == 0){
                        m_prevB = b;
                    } else {
                        if (b < 128){
                            m_X = (b * 256 + m_prevB) / 128.0;
                        } else {
                            m_X = ((b - 256) * 256 + m_prevB) / 128.0;
                        }
                    }
                    break;
                }
                case 1:{     // Сагитталь Y
                    if (m_countBytePack % 2 == 0){
                        m_prevB = b;
                    } else {
                        if (b < 128){
                            m_Y = (b * 256 + m_prevB) / 128.0;
                        } else {
                            m_Y = ((b - 256) * 256 + m_prevB) / 128.0;
                        }
                    }
                    break;
                }
                case 2:{       // Реакция опоры A
                    if (m_countBytePack % 2 == 0){
                        m_prevB = b;
                    } else {
                        m_A = (b * 256 + m_prevB) / 1000.0;
                    }
                }
                case 3:{       // Реакция опоры B
                    if (m_countBytePack % 2 == 0){
                        m_prevB = b;
                    } else {
                        m_B = (b * 256 + m_prevB) / 1000.0;
                    }
                }
                case 4:{       // Реакция опоры C
                    if (m_countBytePack % 2 == 0){
                        m_prevB = b;
                    } else {
                        m_C = (b * 256 + m_prevB) / 1000.0;
                    }
                }
                case 5:{       // Реакция опоры D
                    if (m_countBytePack % 2 == 0){
                        m_prevB = b;
                    } else {
                        m_D = (b * 256 + m_prevB) / 1000.0;
                    }
                }
            }

            // Окончание разбора пакета
            if (m_countBytePack == m_countChannels * 2){  // Достигли заданного кол-ва каналов
                m_Z = m_A + m_B + m_C + m_D;                     // Расчет баллистограммы

                incBlockCount();
                // Передача стабилограммы
                auto stabData = new DeviceProtocols::StabDvcData(this,
                                                                 m_X - m_offsetX, m_Y - m_offsetY,
                                                                 m_A, m_B, m_C, m_D);
                emit sendData(stabData);
                delete stabData;

                m_isPackage = false;                     // Сбросим признак пакета
            }

            m_countBytePack++;
        }
    }

    // Передача информации об ошибке маркера внутри пакета
    if (isError){
        emit error(EC_MarkerIinsidePackage);
    }
}

