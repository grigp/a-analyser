#include "breathwidget.h"
#include "ui_breathwidget.h"

#include "breathsignal.h"
#include "testresultdata.h"
#include "driver.h"

#include <QDebug>

BreathWidget::BreathWidget(Driver *drv, const QString channelId, QWidget *parent) :
    SignalWidget(drv, channelId, parent),
    ui(new Ui::BreathWidget)
{
    ui->setupUi(this);

//    m_breathControl = static_cast<DeviceProtocols::TensoControl*>(driver()->getDeviceControl(DeviceProtocols::uid_TensoControl, channelId));
//    if (m_breathControl)
//        m_breathControl->getTensoValueDiapasone(channelId, m_min, m_max);

    ui->wgtBreathOscill->appendArea("", 1);
//    ui->wgtBreathOscill->setDiapazone(0, m_min, m_max);

    ui->cbScale->addItems(QStringList() <<  "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128");
    setRecordedChannels();
}

BreathWidget::~BreathWidget()
{
    delete ui;
}

void BreathWidget::newProbe()
{
    if (ui->btnRecord->isChecked())
    {
        m_signal = new BreathSignal(channelId(), ui->wgtBreathOscill->frequency());
        objTestResultData()->addChannel(m_signal);
    }
}

void BreathWidget::abortProbe()
{
    if (ui->btnRecord->isChecked())
    {
        m_signal->clear();
        delete m_signal;
    }
}

void BreathWidget::getData(DeviceProtocols::DeviceData *data)
{
    if (data->channelId() == channelId())
    {
        if (!m_breathControl)
        {
            m_breathControl = static_cast<DeviceProtocols::TensoControl*>(driver()->getDeviceControl(DeviceProtocols::uid_TensoControl, channelId()));
            if (m_breathControl)
                m_breathControl->getTensoValueDiapasone(channelId(), m_min, m_max);
            ui->wgtBreathOscill->setDiapazone(0, m_min, m_max);
        }

        DeviceProtocols::TensoDvcData *dynData = static_cast<DeviceProtocols::TensoDvcData*>(data);

        QVector<double> recBreath;
        double value = dynData->value(0).toDouble();
        recBreath << value;
        ui->wgtBreathOscill->addValue(recBreath);
    }
}

void BreathWidget::record(DeviceProtocols::DeviceData *data)
{
    if (data->channelId() == channelId())
    {
        DeviceProtocols::TensoDvcData *dynData = static_cast<DeviceProtocols::TensoDvcData*>(data);
        if (ui->btnRecord->isChecked())
            m_signal->addValue(dynData->value(0).toDouble());
    }
}

void BreathWidget::setFrequency(const int frequency)
{
    ui->wgtBreathOscill->setFrequency(frequency);
}

void BreathWidget::enabledControls(const bool enabled)
{
    ui->cbScale->setEnabled(enabled);
    ui->lblScale->setEnabled(enabled);
    ui->btnCalibrate->setEnabled(enabled);
    ui->btnRecord->setEnabled(enabled);
}

void BreathWidget::setAllwaysRecordingChannel(const QString &channelId)
{
    Q_UNUSED(channelId);
}

void BreathWidget::on_calibrate()
{
    if (m_breathControl)
        m_breathControl->calibrateTenso(channelId());
}

void BreathWidget::on_scaleChange(int scaleIdx)
{
    int scale = 1;
    for (int i = 0; i < scaleIdx; ++i)
        scale *= 2;

    double d = (m_max - m_min) / scale;
    double minV = (m_max + m_min) / 2 - d / 2;
    double maxV = (m_max + m_min) / 2 + d / 2;
    ui->wgtBreathOscill->setDiapazone(0, minV, maxV);
}

void BreathWidget::on_recChange(bool checked)
{
    setRecButton(ui->btnRecord, checked);

    if (checked)
    {
        m_signal = new BreathSignal(channelId(), ui->wgtBreathOscill->frequency());
        objTestResultData()->addChannel(m_signal);
    }
    else
    {
        m_signal->clear();
        delete m_signal;
    }
}

void BreathWidget::setRecordedChannels()
{
    auto isRec = driver()->isChannelRecordingDefault(channelId());

    ui->btnRecord->setChecked(isRec);
    setRecButton(ui->btnRecord, isRec);
}

void BreathWidget::setRecButton(QPushButton *btn, const bool checked)
{
    if (checked)
        btn->setIcon(QIcon(":/images/SaveOK.png"));
    else
        btn->setIcon(QIcon(":/images/SaveNO.png"));
}
