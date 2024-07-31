#include "mwstick.h"

#include "mwstickparamsdialog.h"
#include "aanalyserapplication.h"
#include "serialport.h"
#include "channelsutils.h"
#include "settingsprovider.h"
#include "driverdefines.h"
#include "connection.h"

#include <QApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QtMath>
#include <QDebug>

namespace
{

const quint8 MarkerValue = 0x80;

///! Константы кодов типов каналов
static const int ChanU16Bit = 130;   ///< Число 16 бит без знака
static const int Chan16Bit = 132;    ///< Число 16 бит со знаком
static const int ChanU24Bit = 134;   ///< Число 24 бит без знака
static const int Chan24Bit = 136;    ///< Число 24 бит со знаком

static const quint8 Header1 = 0xAC;  ///< Заголовок 1
static const quint8 Header2 = 0x53;  ///< Заголовок 2

QMap<quint8, quint8> NextHeader {
    std::pair<quint8, quint8> (Header1, Header2)
  , std::pair<quint8, quint8> (Header2, Header1)
};

///< Поддерживаемые протоколы управления устройствами
QSet<QString> Controls =
{
    DeviceProtocols::uid_CommonControl,
    DeviceProtocols::uid_TensoControl,
};

}

MWStick::MWStick(QObject *parent)
    : Driver (parent)
{

}

MWStick::~MWStick()
{

}

void MWStick::setParams(const DeviceProtocols::Ports port, const QJsonObject &params)
{
    setPortName(port);

    m_chanRecordingDefault = MWStick::getChanRecordingDefault(params["chan_recording_default"].toObject());

    m_channel = params["channel"].toInt(0);
    m_rkp = params["rkp"].toDouble(0.8);
    m_pn = params["pn"].toInt(500);

    m_countChannels = 27;
}

bool MWStick::editParams(QJsonObject &params)
{
    MWStickParamsDialog dlg(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow());
    dlg.setChannel(params["channel"].toInt(0));
    dlg.setRkp(params["rkp"].toDouble(1.7));
    dlg.setPn(params["pn"].toInt(100));

    if (dlg.exec() == QDialog::Accepted)
    {
        params["channel"] = dlg.channel();
        params["rkp"] = dlg.rkp();
        params["pn"] = dlg.pn();

        return true;
    }
    return false;
}

void MWStick::start()
{
    Driver::start();
    infoRequest();
}

void MWStick::stop()
{
    stopDataSending();
    Driver::stop();
}

int MWStick::frequency(const QString &channelId) const
{
    Q_UNUSED(channelId);
    return 50;
}

QList<QString> MWStick::getChannelsByProtocol(const QString &protocolUid) const
{
    // todo: Неплохо было бы автоматизировать, используя getChannels(), чтобы один раз составить список, а здесь только выбирать из него
    QList<QString> retval;
    if (protocolUid == DeviceProtocols::uid_DynProtocol)
        retval << ChannelsDefines::chanMWStickForce;
    return retval;
}

QList<QString> MWStick::getChannelsByFormat(const QString &formatUid) const
{
    // todo: Неплохо было бы автоматизировать, используя getChannels(), чтобы один раз составить список, а здесь только выбирать из него
    QList<QString> retval;
    if (formatUid == ChannelsDefines::cfSinglePositive)
        retval << ChannelsDefines::chanMWStickForce;
    return retval;
}

QList<QString> MWStick::getChannels() const
{
    QList<QString> retval;
    retval << ChannelsDefines::chanMWStickForce;
    return retval;
}

int MWStick::getSubChannelsCount(const QString &channelUid) const
{
    Q_UNUSED(channelUid);
    return 1;
}

bool MWStick::isChannelRecordingDefault(const QString &channelUid, const int subChan) const
{
    Q_UNUSED(channelUid);
    Q_UNUSED(subChan);
    return true;
}

DeviceProtocols::DeviceControl *MWStick::getDeviceControl(const QString &controlId, const QString &channelId)
{
    Q_UNUSED(channelId);
    if (controlId == DeviceProtocols::uid_CommonControl)
        return dynamic_cast<DeviceProtocols::CommonControl*>(this);
    else
    if (controlId == DeviceProtocols::uid_TensoControl)
        return dynamic_cast<DeviceProtocols::TensoControl*>(this);
    return nullptr;
}

QStringList MWStick::getProtocols()
{
    return QStringList() << DeviceProtocols::uid_DynProtocol;
}

QList<DeviceProtocols::Ports> MWStick::getPorts()
{
    return DeviceProtocols::comPorts();
}

void MWStick::calibrateTenso(const QString &channelUid)
{
    Q_UNUSED(channelUid);
    quint8 chan = 11 + static_cast<quint8>(m_channel);

    QByteArray cmd;
    cmd.resize(2);
    cmd[0] = 90;
    cmd[1] = static_cast<char>(chan);
    emit writeData(cmd);
    m_isCalibrated = true;

//    quint8 chan = 0;
//    if (m_channel == 0)
//        chan = 0xA;
//    else
//    if (m_channel == 1)
//        chan = 0xB;
//    else
//    if (m_channel == 2)
//        chan = 0xC;
//    if (chan != 0)
//    {
//        QByteArray cmd;
//        cmd.resize(4);
//        cmd[0] = 0x33;
//        cmd[1] = static_cast<char>(chan);
//        cmd[2] = 0x33;
//        cmd[3] = 0x7;
//        emit writeData(cmd);
//        m_isCalibrated = true;
//    }
}

void MWStick::getTensoValueDiapasone(const int chanNumber, double &min, double &max)
{
    Q_ASSERT(chanNumber >= 0 && chanNumber < 3);
    min = 0;
    max = m_pn;
}

void MWStick::getTensoValueDiapasone(const QString channelId, double &min, double &max)
{
    if (channelId == ChannelsDefines::chanMWStickForce)
    {
        min = 0;
        max = m_pn;
    }
    else
    {
        min = 0;
        max = 100;
    }
}

void MWStick::setTensoValueDiapasone(const int chanNumber, const double min, const double max)
{
    Q_ASSERT(chanNumber >= 0 && chanNumber < 3);
    m_tensoPercMin[chanNumber] = min;
    m_tensoPercMax[chanNumber] = max;
}

bool MWStick::isCalibrated() const
{
    return m_isCalibrated;
}

SerialPortDefines::Settings MWStick::getSerialPortSettings()
{
    return SerialPortDefines::Settings(115200,
                                       QSerialPort::Data8,
                                       QSerialPort::NoParity,
                                       QSerialPort::OneStop,
                                       QSerialPort::NoFlowControl);
}

void MWStick::on_readData(const QByteArray data)
{
    Driver::on_readData(data);

    foreach (auto b, data)
    {
        if (m_dataStage == dsInfoRequest)
            assignByteOnInfoRequest(static_cast<quint8>(b));
        else
        if (m_dataStage == dsSending)
            assignByteOnSending(static_cast<quint8>(b));

//        assignByteFromDevice(static_cast<quint8>(b));
    }
}

void MWStick::on_error(const QString &err)
{
    Driver::on_error(err);
    qDebug() << err;
}

void MWStick::assignByteFromDevice(quint8 b)
{
    //! Обработка байта тензоканала
    auto assignByteTenso = [&](double &value)
    {
        if (m_countBytePack % 2 == 0)
        {
            m_prevB = b;
        }
        else
        {
            double dynVal = b * 256 + m_prevB;
            dynVal = dynVal - 0x8000;
            value = (m_pn * 1000 * dynVal) / (65535 * 128 * m_rkp);
            if (value < 0)
                value = fabs(value);
        }
    };


    bool isTwoMarkers = false;
    bool isError = false;

    if (b == MarkerValue)    //! Пришел байт маркера
    {
        if (!m_isMarker){    //! Ожидание первого байта маркера
            m_isMarker = true;
        } else {           //! Ожидание второго байта маркера
            if (m_isPackage){   //! Два маркера внутри пакета - ошибка
                isError = true;
            }
            m_isMarker = false;
            m_isPackage = true;    //! Признак начала приема пакета
            m_countBytePack = 0;
            isTwoMarkers = true;
        }
    } else {            //! Не байт маркера
        m_isMarker = false;
    }

    if (!isTwoMarkers){    //! Если не было два маркера подряд, то эта ситуация внутри пакета и нам надо это отрабатывать
        if (m_isPackage){
            switch (m_countBytePack / 2) {
                case 0:{     //! Фронталь X
                    break;
                }
                case 1:{     //! Сагитталь Y
                    break;
                }
                case 2:{       //! Реакция опоры A
                    break;
                }
                case 3:{       //! Реакция опоры B
                    break;
                }
                case 4:{       //! Реакция опоры C
                    break;
                }
                case 5:{       //! Реакция опоры D
                    break;
                }
                case 6: {          //! RR + Synchro
                    break;
                }
                case 7: {             //! Тензоканал 1
                    //assignByteTenso(m_t1);
                    break;
                }
                case 8: {             //! Тензоканал 2
                    //assignByteTenso(m_t2);
                    break;
                }
                case 9: {             //! Тензоканал 3
                    assignByteTenso(m_t3);
                    break;
                }
                case 10: {            //! Пульс
                    break;
                }
                case 11: {break; }  //! Миограммы 4 канала и 4 отведения
                case 12: {break; }
                case 13: {break; }
                case 14: {break; }
                case 15: {break; }
                case 16: {break; }
                case 17: {break; }
                case 18: {break; }
                case 19: {break; }
                case 20: {break; }
                case 21: {break; }
                case 22: {break; }
                case 23: {break; }
                case 24: {break; }
                case 25: {break; }
                case 26: {break; }
            }

            //! Окончание разбора пакета
            if (m_countBytePack == m_countChannels * 2)
            {  //! Достигли заданного кол-ва каналов
                incBlockCount();
                sendDataBlock();
                m_isPackage = false;                     //! Сбросим признак пакета
            }

            m_countBytePack++;
        }
    }

    //! Передача информации об ошибке маркера внутри пакета
    if (isError){
        emit error(DriverDefines::EC_MarkerIinsidePackage);
    }
}

void MWStick::assignByteOnInfoRequest(quint8 b)
{
    if (m_byteCount == 0)
    {
        if (b == 0xF5)
            ++m_byteCount;
    }
    else
    if (m_byteCount == 1)
    {
        m_dvcCode = b;
        ++m_byteCount;
    }
    else
    if (m_byteCount >= 2 && m_byteCount <= 5)
    {
        m_channelKind[m_byteCount - 2] = b;
        if (b == ChanU16Bit || b == Chan16Bit)
            m_blockSize += 2;
        else
        if (b == ChanU24Bit || b == Chan24Bit)
            m_blockSize += 3;

        ++m_byteCount;
    }
    else
    if (m_byteCount >= 6)
    {
        quint8 n = static_cast<quint8>(m_byteCount - 6);
        quint8 z = n % 4;
        m_koefParts[z] = b;
        if (z == 3)
        {
            //qDebug() << "----" << n/4;
            m_channelSK[n / 4] = ((m_koefParts[3] << 24) + (m_koefParts[2] << 16) + (m_koefParts[1] << 8) + m_koefParts[0]) /
                                 1000000.0;
        }
        ++m_byteCount;
    }

    if (m_byteCount == 22)
    {
//        qDebug() << "dvc code:" << m_dvcCode;
//        for (int i = 0; i < 4; ++i)
//            qDebug() << "chan kind:" << m_channelKind[i] << "    chan koef:" << m_channelSK[i];

        m_blockSize += 2;
        m_synchroBuf.resize(m_blockSize * 3);
        m_blockData.resize(m_blockSize - 2);
        m_byteCount = 0;
//        qDebug() << "block size:" << m_blockSize;
//        qDebug() << "----------------";
//        quint8 a = 0xAC;
//        quint8 b = 0x53;
//        qDebug() << a << static_cast<quint8>(~a) << "  " << b << static_cast<quint8>(~b);
        m_dataStage = dsNone;
        startDataSending();
    }


//    int m_byteCount {0};
//    quint8 m_dvcCode {0};     ///< Код устройства
//    quint8 m_channelKind[4];  ///< Типы каналов
//    double m_channelSK[4];    ///< Коэффициенты чувствительности каналов
//    quint8 m_koefParts[4];    ///< Для сборки коэф-та чувствительности
}

void MWStick::assignByteOnSending(quint8 b)
{
    if (!m_isSynchro)
    {
        m_synchroBuf[m_byteCount] = b;
        ++m_byteCount;
        if (m_byteCount == m_synchroBuf.size())
        {
            //! Поиск синхронизации
            auto idx = lookingForSynchronize();
            //! Если нашли
            if (idx > -1)
            {
                m_isSynchro = true;
                m_byteCount = 0;
                //! Для всех байт с точки синхронизации вызвать assignByte
                for (int i = idx; i < m_synchroBuf.size(); ++i)
                    assignByte(m_synchroBuf[i]);
            }
        }
    }
    else
    {
        assignByte(b);
    }
}

void MWStick::assignByte(quint8 b)
{
    if (NextHeader.contains(b) && (m_byteCount == 0 || m_byteCount == m_blockSize))
        m_byteCount = 0;
    else
    {
        if (m_byteCount < m_blockSize - 1)
            m_blockData[m_byteCount - 1] = b;
        else
        if (m_byteCount == m_blockSize - 1)
        {
            quint8 n = 0;

            auto assignDataChan = [&](int idx)
            {
                if (m_channelKind[idx] == ChanU16Bit)
                {
                    int b0 = m_blockData[n];
                    int b1 = m_blockData[n + 1];
                    m_values[idx] = b1*256 + b0;
                    n += 2;
                }
                else
                if (m_channelKind[idx] == Chan16Bit)
                {
                    int b0 = m_blockData[n];
                    int b1 = m_blockData[n + 1];
                    if (b1 < 128)
                        m_values[idx] = -((b1*256 + b0) - 0x8000);
                    else
                        m_values[idx] = (b1*256 + b0) - 0x8000;
//                    if (idx == 2)
//                        qDebug() << b1 << b0 << m_values[idx];
                    n += 2;
                }
                else
                if (m_channelKind[idx] == ChanU24Bit)
                {
                    int b0 = m_blockData[n];
                    int b1 = m_blockData[n + 1];
                    int b2 = m_blockData[n + 2];
                    m_values[idx] = b2*256*256 + b1*256 + b0;
                    n += 3;
                }
                else
                if (m_channelKind[idx] == Chan24Bit)
                {
                    int b0 = m_blockData[n];
                    int b1 = m_blockData[n + 1];
                    int b2 = m_blockData[n + 2];
                    if (b2 < 128)
                        m_values[idx] = b2*256*256 + b1*256 + b0;
                    else
                        m_values[idx] = (b2-256*256)*256*256 + (b1-256)*256 + b0;
                    n += 3;
                }

                m_values[idx] = (m_values[idx] * m_channelSK[idx]) / 1000.0;
            };

            assignDataChan(0);
            assignDataChan(1);
            assignDataChan(2);
            assignDataChan(3);

            incBlockCount();
            sendDataBlock();
        }
    }
    ++m_byteCount;
}

int MWStick::lookingForSynchronize()
{
    for (int i = 0; i < m_synchroBuf.size() - 2 * m_blockSize; ++i)
    {
        quint8 b = m_synchroBuf[i];
        if (NextHeader.contains(b))
        {
            quint8 b1 = m_synchroBuf[i + m_blockSize];
            quint8 b2 = m_synchroBuf[i + 2 * m_blockSize];
            if (b1 == NextHeader.value(b) && b2 == NextHeader.value(b1))
                return i;
        }

    }
    return -1;
}

void MWStick::sendDataBlock()
{
    sendTensoChannels();
}

void MWStick::sendTensoChannels()
{
//    double val = 0;
//    if (m_channel == 0)
//        val = m_t1;
//    else
//    if (m_channel == 1)
//        val = m_t2;
//    else
//    if (m_channel == 2)
//        val = m_t3;
    double val = m_values[m_channel];

    auto tenso = new DeviceProtocols::TensoDvcData(this, ChannelsDefines::chanMWStickForce, val);
    emit sendData(tenso);
    delete tenso;
}

QMap<QString, bool> MWStick::getChanRecordingDefault(const QJsonObject &obj)
{
    QMap<QString, bool> retval;
    retval.insert(ChannelsDefines::chanTenso1, obj["tenso1"].toBool());
    return retval;
}

QJsonObject MWStick::setChanRecordingDefault(const QMap<QString, bool> &recMap)
{
    QJsonObject recObj;
    recObj["tenso1"] = recMap.value(ChannelsDefines::chanTenso1);
    return recObj;
}

void MWStick::startDataSending()
{
    m_dataStage = dsSending;
    QByteArray cmd;
    cmd.resize(2);
    cmd[0] = static_cast<char>(90);
    cmd[1] = static_cast<char>(01);
    emit writeData(cmd);
}

void MWStick::stopDataSending()
{
    m_dataStage = dsNone;
    QByteArray cmd;
    cmd.resize(2);
    cmd[0] = static_cast<char>(90);
    cmd[1] = static_cast<char>(02);
    emit writeData(cmd);
}

void MWStick::infoRequest()
{
    m_dataStage = dsInfoRequest;
    m_byteCount = 0;
    QByteArray cmd;
    cmd.resize(2);
    cmd[0] = static_cast<char>(90);
    cmd[1] = static_cast<char>(20);
    emit writeData(cmd);
}


