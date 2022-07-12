#include "bedsidescales01.h"

#include "aanalyserapplication.h"
#include "bedsidescales01paramsdialog.h"
#include "channelsutils.h"

#include <QDebug>

namespace
{
static const quint8 Divider = 0xFF;
static const quint8 Addition = 0xFE;
//static const quint8 NetAddr = 0x03;
static const quint8 CodeOp = 0xCC;

}

BedsideScales01::BedsideScales01(QObject *parent)
    : Driver(parent)
{
    for (int i = 0; i < 4; ++i)
        m_offset[i] = 0;
}

void BedsideScales01::setParams(const DeviceProtocols::Ports port, const QJsonObject &params)
{
    setPortName(port);

    m_rkpSensor[0] = params["rkp1"].toDouble(1.9);
    m_rkpSensor[1] = params["rkp2"].toDouble(1.9);
    m_rkpSensor[2] = params["rkp3"].toDouble(1.9);
    m_rkpSensor[3] = params["rkp4"].toDouble(1.9);
}

bool BedsideScales01::editParams(QJsonObject &params)
{
    Q_UNUSED(params);
    BedsideScales01ParamsDialog dlg(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow());

    dlg.setRkpSensor1(params["rkp1"].toDouble(1.9));
    dlg.setRkpSensor2(params["rkp2"].toDouble(1.9));
    dlg.setRkpSensor3(params["rkp3"].toDouble(1.9));
    dlg.setRkpSensor4(params["rkp4"].toDouble(1.9));

    if (dlg.exec() == QDialog::Accepted)
    {
        params["rkp1"] = dlg.rkpSensor1();
        params["rkp2"] = dlg.rkpSensor2();
        params["rkp3"] = dlg.rkpSensor3();
        params["rkp4"] = dlg.rkpSensor4();

        return true;
    }
    return false;
}

void BedsideScales01::start()
{
    Driver::start();
}

void BedsideScales01::stop()
{
    Driver::stop();
}

int BedsideScales01::frequency(const QString &channelId) const
{
    Q_UNUSED(channelId);
    return 150;
}

QList<QString> BedsideScales01::getChannelsByProtocol(const QString &protocolUid) const
{
    // todo: Неплохо было бы автоматизировать, используя getChannels(), чтобы один раз составить список, а здесь только выбирать из него
    QList<QString> retval;
    if (protocolUid == DeviceProtocols::uid_WeightPlateProtocol)
        retval << ChannelsDefines::chanWeightPlate;
    else
    if (protocolUid == DeviceProtocols::uid_ADCValuesProtocol)
        retval << ChannelsDefines::chanADCValues;
    return retval;
}

QList<QString> BedsideScales01::getChannelsByFormat(const QString &formatUid) const
{
    // todo: Неплохо было бы автоматизировать, используя getChannels(), чтобы один раз составить список, а здесь только выбирать из него
    QList<QString> retval;
    if (formatUid == ChannelsDefines::cfSinglePositive)
        retval << ChannelsDefines::chanWeightPlate;
    return retval;
}

QList<QString> BedsideScales01::getChannels() const
{
    QList<QString> retval;
    retval << ChannelsDefines::chanWeightPlate << ChannelsDefines::chanADCValues;
    return retval;
}

int BedsideScales01::getSubChannelsCount(const QString &channelUid) const
{
    if (ChannelsUtils::instance().channelType(channelUid) == ChannelsDefines::ctWeightPlate)
        return 4;
    return 0;
}

bool BedsideScales01::isChannelRecordingDefault(const QString &channelUid, const int subChan) const
{
    Q_UNUSED(channelUid);
    Q_UNUSED(subChan);
    return true;
}

DeviceProtocols::DeviceControl *BedsideScales01::getDeviceControl(const QString &controlId, const QString &channelId)
{
    Q_UNUSED(channelId);
    if (controlId == DeviceProtocols::uid_CommonControl)
        return dynamic_cast<DeviceProtocols::CommonControl*>(this);
    else
    if (controlId == DeviceProtocols::uid_WeightPlateControl)
        return dynamic_cast<DeviceProtocols::MultiPlatformControl*>(this);
    return nullptr;
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
    Q_UNUSED(channelUid);
    m_isCalibrating = true;
    m_calibrCount = 0;
}

void BedsideScales01::getTensoValueDiapasone(const int chanNumber, double &min, double &max)
{
    Q_UNUSED(chanNumber);
    min = 0;
    max = 50;
}

void BedsideScales01::getTensoValueDiapasone(const QString channelId, double &min, double &max)
{
    Q_UNUSED(channelId);
    min = 0;
    max = 50;
}

void BedsideScales01::setTensoValueDiapasone(const int chanNumber, const double min, const double max)
{
    Q_UNUSED(chanNumber);
    Q_UNUSED(min);
    Q_UNUSED(max);
}

SerialPortDefines::Settings BedsideScales01::getSerialPortSettings()
{
    return SerialPortDefines::Settings(115200,
                                       QSerialPort::Data8,
                                       QSerialPort::NoParity,
                                       QSerialPort::OneStop,
                                       QSerialPort::NoFlowControl);
}

void BedsideScales01::on_readData(const QByteArray data)
{
    Driver::on_readData(data);
    for (int i = 0; i < data.count(); i++)
    {
        quint8 b = static_cast<quint8>(data[i]);
        assignByteFromDevice(b);
    }
}

void BedsideScales01::on_error(const QString &err)
{
    Driver::on_error(err);
    qDebug() << err;
}

//QVector<quint8> buf;


void BedsideScales01::assignByteFromDevice(quint8 b)
{
//    qDebug() << QString::number(b, 16);
    auto nextMark = [&](const quint8 marker)
    {
        if (b == marker)
        {
            --m_synchro;
            if (marker == CodeOp)
                m_dataByteCount = 0;
        }
        else
            m_synchro = 5;
    };

    if (b == Addition && m_bPrev == Divider)
        return;

//    buf << b;
    if (m_synchro == 5 || m_synchro == 4 || m_synchro == 3)
        nextMark(Divider);
    else
    if (m_synchro == 2)
    {
        if (m_circleCnt == 0xFF)
            m_circleCnt = b;
        else
        {
            if (m_circleCnt < 0x7F)
                ++m_circleCnt;
            else
                m_circleCnt = 0;
        }
        if (b != m_circleCnt)
        {
            ++m_errorCount;
            m_circleCnt = b;
        }
        --m_synchro;
    }
    else
    if (m_synchro == 1)
        nextMark(CodeOp);
    else
    if (m_synchro == 0)
    {
        if (m_dataByteCount < 8)
        {
            if (m_dataByteCount % 2 == 0)
                m_lo = b;
            else
            {
                m_adcValues[m_dataByteCount / 2] = b * 256 + m_lo;
//                m_values[m_dataByteCount / 2] = m_adcValues[m_dataByteCount / 2] * (50.0 / 65535.0);
                if (!m_isCalibrating)
                    m_values[m_dataByteCount / 2] = (50 * static_cast<double>(m_adcValues[m_dataByteCount / 2])) /
                                                    (static_cast<double>(65535) * 2 / 2)
                                                    - m_offset[m_dataByteCount / 2];  // m_rkpSensor[m_dataByteCount / 2]
                else
                    m_values[m_dataByteCount / 2] = (50 * static_cast<double>(m_adcValues[m_dataByteCount / 2])) /
                                                    (static_cast<double>(65535) * 2 / 2);
            }
            ++m_dataByteCount;
        }
        else
        {
            if (m_isCalibrating)
            {
                ++m_calibrCount;
                if (m_calibrCount < frequency(""))
                {
                    for (int i = 0; i < 4; ++i)
                        m_offset[i] += m_values[i];
                }
                else
                {
                    for (int i = 0; i < 4; ++i)
                        m_offset[i] /= m_calibrCount;
                    m_calibrCount = 0;
                    m_isCalibrating = false;
                }
            }

//            QString s = "";
//            foreach (auto v, buf)
//                s += (QString::number(v, 16) + " ");
//            qDebug() << s;
//            buf.clear();
            m_crc = b;
            sendDataBlock();
            m_dataByteCount = 0;
            m_synchro = 5;
        }
    }

    m_bPrev = b;
}

void BedsideScales01::sendDataBlock()
{
    incBlockCount();

    QVector<double> valDbl;
    for (int i = 0; i < 4; ++i)
        valDbl << m_values[i] ;
    auto dataDbl = new DeviceProtocols::WeightPlateDvcData(this, ChannelsDefines::chanWeightPlate, valDbl);
    emit sendData(dataDbl);
    delete dataDbl;

    QVector<quint16> valInt;
    for (int i = 0; i < 4; ++i)
        valInt << m_adcValues[i] ;
    auto dataInt = new DeviceProtocols::ADCValuesDvcData(this, ChannelsDefines::chanADCValues, valInt);
    emit sendData(dataInt);
    delete dataInt;
}
