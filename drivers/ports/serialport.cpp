#include "serialport.h"
#include <qdebug.h>

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{

}

SerialPort::~SerialPort()
{
    emit finishedPort();//Сигнал о завершении работы
}

void SerialPort::toThread(QThread *thread)
{
    moveToThread(thread);
    m_nativePort.moveToThread(thread);
}

void SerialPort :: processPort()
{
    // подключаем проверку ошибок порта
    connect(&m_nativePort, &QSerialPort::errorOccurred, this, &SerialPort::handleError);
    // подключаем чтение с порта по сигналу readyRead()
    connect(&m_nativePort, &QSerialPort::readyRead, this, &SerialPort::ReadInPort);
}

void SerialPort :: WriteSettingsPort(QString name, int baudrate,int DataBits,
                         int Parity,int StopBits, int FlowControl)
{
    m_SettingsPort.name = name;
    m_SettingsPort.baudRate = static_cast<QSerialPort::BaudRate>(baudrate);
    m_SettingsPort.dataBits = static_cast<QSerialPort::DataBits>(DataBits);
    m_SettingsPort.parity = static_cast<QSerialPort::Parity>(Parity);
    m_SettingsPort.stopBits = static_cast<QSerialPort::StopBits>(StopBits);
    m_SettingsPort.flowControl = static_cast<QSerialPort::FlowControl>(FlowControl);
}

void SerialPort :: ConnectPort(void)
{
    m_nativePort.setPortName(m_SettingsPort.name);
    if (m_nativePort.open(QIODevice::ReadWrite))
    {
        if (m_nativePort.setBaudRate(m_SettingsPort.baudRate)
                && m_nativePort.setDataBits(m_SettingsPort.dataBits)
                && m_nativePort.setParity(m_SettingsPort.parity)
                && m_nativePort.setStopBits(m_SettingsPort.stopBits)
                && m_nativePort.setFlowControl(m_SettingsPort.flowControl))
        {
            if (m_nativePort.isOpen())
            {
        //        error_((m_SettingsPort.name+ " >> Открыт!"));
            }
        }
        else
        {
            m_nativePort.close();
            error_(m_nativePort.errorString());
        }
    }
    else
    {
        m_nativePort.close();
        error_(m_nativePort.errorString());
    }
}

// проверка ошибок при работе
void SerialPort::handleError(QSerialPort::SerialPortError error)
{
    if ( (m_nativePort.isOpen()) && (error == QSerialPort::ResourceError)) {
        error_(m_nativePort.errorString());
        DisconnectPort();
    }
}

// Отключаем порт
void  SerialPort::DisconnectPort()
{
    if(m_nativePort.isOpen())
    {
        m_nativePort.close();
        error_(m_SettingsPort.name + " >> " + tr("Закрыт!"));
    }
}

// Запись данных в порт
void SerialPort :: WriteToPort(QByteArray data)
{
    if(m_nativePort.isOpen())
    {
        m_nativePort.write(data);
    }
}

// Чтение данных из порта
void SerialPort :: ReadInPort()
{
    QByteArray data;
    data.append(m_nativePort.readAll());
    outPortD(data);
    //((QString)(adr.toInt())).toLatin1().toHex()
}
