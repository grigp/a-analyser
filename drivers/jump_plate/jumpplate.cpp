#include "jumpplate.h"

#include "jumpplateparamsdialog.h"
#include "aanalyserapplication.h"

namespace
{
const quint8 MarkerValue = 0x80;

}

JumpPlate::JumpPlate(QObject *parent)
    : Driver(parent)
{

}

void JumpPlate::setParams(const DeviceProtocols::Ports port, const QJsonObject &params)
{

}

bool JumpPlate::editParams(QJsonObject &params)
{
    JumpPlateParamsDialog dlg(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow());
    if (dlg.exec() == QDialog::Accepted)
    {
        return true;
    }
    return false;
}

void JumpPlate::start()
{
    Driver::start();

    //! Команда запуска платформы
    QByteArray cmd;
    cmd.resize(2);
    cmd[0] = 0xA5;
    cmd[1] = 0x63;
    emit writeData(cmd);

}

void JumpPlate::stop()
{
    //! Команда останова платформы
    QByteArray cmd;
    cmd.resize(2);
    cmd[0] = 0xA5;
    cmd[1] = 0x65;
    emit writeData(cmd);

    Driver::stop();
}

QStringList JumpPlate::getProtocols()
{
    return QStringList() << DeviceProtocols::uid_JumpPlateProtocol;
}

QList<DeviceProtocols::Ports> JumpPlate::getPorts()
{
    return DeviceProtocols::comPorts();
}

void JumpPlate::calibrate()
{

}

void JumpPlate::on_readData(const QByteArray data)
{
    Driver::on_readData(data);
    for (int i = 0; i < data.count(); i++)
    {
        quint8 B = data[i];
        assignByteFromDevice(B);
    }
}

void JumpPlate::on_error(const QString &err)
{
    Driver::on_error(err);
}

void JumpPlate::assignByteFromDevice(quint8 b)
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

    if (!isTwoMarkers)        //! Если не было два маркера подряд, то эта ситуация внутри пакета и нам надо это отрабатывать
    {
        if (m_isPackage)
        {
            switch (m_countBytePack / 4) {
            case 0:     //! Флаг нажатия первой платформы
            {
                if (m_countBytePack % 4 == 0)
                    m_byte0 = b;
                else
                if (m_countBytePack % 4 == 1)
                    m_byte1 = b;
                else
                if (m_countBytePack % 4 == 2)
                    m_byte2 = b;
                else
                    m_flagPlate1 = m_byte0 * 16777216 + m_byte1 * 65536 + m_byte2 * 256 + b;
                break;
            }
            case 1:     //! Счетчик первой платформы
            {
                if (m_countBytePack % 4 == 0)
                    m_byte0 = b;
                else
                if (m_countBytePack % 4 == 1)
                    m_byte1 = b;
                else
                if (m_countBytePack % 4 == 2)
                    m_byte2 = b;
                else
                    m_counterPlate1 = m_byte0 * 16777216 + m_byte1 * 65536 + m_byte2 * 256 + b;
                break;
            }
            case 2:      //! Флаг нажатия второй платформы
            {
                if (m_countBytePack % 4 == 0)
                    m_byte0 = b;
                else
                if (m_countBytePack % 4 == 1)
                    m_byte1 = b;
                else
                if (m_countBytePack % 4 == 2)
                    m_byte2 = b;
                else
                    m_flagPlate2 = m_byte0 * 16777216 + m_byte1 * 65536 + m_byte2 * 256 + b;
                break;
            }
            case 3:      //! Счетчик второй платформы
            {
                if (m_countBytePack % 4 == 0)
                    m_byte0 = b;
                else
                if (m_countBytePack % 4 == 1)
                    m_byte1 = b;
                else
                if (m_countBytePack % 4 == 2)
                    m_byte2 = b;
                else
                    m_counterPlate2 = m_byte0 * 16777216 + m_byte1 * 65536 + m_byte2 * 256 + b;
                break;
            }
            }

            //! Окончание разбора пакета
            if (m_countBytePack == m_countChannels * 2)     //! Достигли заданного кол-ва каналов
            {
                incBlockCount();
                //! Передача данных
                if (m_busyPlate1 != m_flagPlate1)
                {
                    m_busyPlate1 = m_flagPlate1;
                    auto data = new DeviceProtocols::JumpPlateDvcData(this, 1, m_busyPlate1, m_timePlate1);
                    emit sendData(data);
                    delete data;
                }

                if (m_busyPlate2 != m_flagPlate2)
                {
                    m_busyPlate2 = m_flagPlate2;
                    auto data = new DeviceProtocols::JumpPlateDvcData(this, 2, m_busyPlate2, m_timePlate2);
                    emit sendData(data);
                    delete data;
                }

                m_isPackage = false;                     //! Сбросим признак пакета
            }

            m_countBytePack++;
        }
    }

    //! Передача информации об ошибке маркера внутри пакета
    if (isError){
        emit error(EC_MarkerIinsidePackage);
    }
}
