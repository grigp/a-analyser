#ifndef SERIALPORTDEFINES_H
#define SERIALPORTDEFINES_H

#include <QString>
#include <QSerialPort>

namespace SerialPortDefines {

///<Структура с настройками порта
struct Settings
{
    QString name;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;

    Settings(qint32 br,
             QSerialPort::DataBits db,
             QSerialPort::Parity pty,
             QSerialPort::StopBits sb,
             QSerialPort::FlowControl fc)
        : baudRate(br)
        , dataBits(db)
        , parity(pty)
        , stopBits(sb)
        , flowControl(fc)
    {name = "com1";}
    Settings(){}
};

}

#endif // SERIALPORTDEFINES_H
