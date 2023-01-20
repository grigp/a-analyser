#include "driver.h"

#include "serialport.h"
#include "driverdefines.h"

#include <QDebug>

Driver::Driver(QObject *parent) : QObject(parent)
{
}

Driver::~Driver()
{
}

void Driver::start()
{
    if (!m_trdInput)
    {
        m_trdInput = new QThread();
        m_port = new SerialPort();
        m_port->toThread(m_trdInput);

        connect(m_port, &SerialPort::error_, this, &Driver::on_error);
        connect(m_trdInput, &QThread::started, m_port, &SerialPort::processPort);
        connect(m_port, &SerialPort::finishedPort, m_trdInput, &QThread::quit);
        connect(m_trdInput, &QThread::finished, m_port, &SerialPort::deleteLater);
        connect(m_port, &SerialPort::finishedPort, m_trdInput, &QThread::deleteLater);
        connect(this, &Driver::portSettings, m_port, &SerialPort::WriteSettingsPort);
        connect(this, &Driver::connectPort, m_port, &SerialPort::ConnectPort);
        connect(this, &Driver::disconnectPort, m_port, &SerialPort::DisconnectPort);
        connect(m_port, &SerialPort::outPortD, this, &Driver::on_readData);
        connect(this, &Driver::writeData, m_port, &SerialPort::WriteToPort);

        m_tmCommError = startTimer(1000);

        auto sps = getSerialPortSettings();
        emit portSettings(DeviceProtocols::serialPortName(m_portName),
                          sps.baudRate, sps.dataBits, sps.parity, sps.stopBits, sps.flowControl);
        emit connectPort();

        m_trdInput->start();
    }

    emit started();
}

void Driver::stop()
{
    emit stopped();
    disconnect(m_port, &SerialPort::outPortD, this, &Driver::on_readData);
    emit disconnectPort();

}

void Driver::on_readData(const QByteArray data)
{
    Q_UNUSED(data);
}

void Driver::on_error(const QString &err)
{
    Q_UNUSED(err);
}

SerialPortDefines::Settings Driver::getSerialPortSettings()
{
    return SerialPortDefines::Settings(57600,
                                       QSerialPort::Data8,
                                       QSerialPort::NoParity,
                                       QSerialPort::OneStop,
                                       QSerialPort::NoFlowControl);
}

void Driver::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_tmCommError)
    {
        if (m_blockCount == m_blockCountPrev)
        {
            if (!m_isCommunicationError)
            {
                m_isCommunicationError = true;
                qDebug() << "error";
                emit communicationError(driverName(), DeviceProtocols::portName(m_portName), DriverDefines::EC_NoData);
            }
        }
        else
            m_isCommunicationError = false;
        m_blockCountPrev = m_blockCount;
    }
}


