#include "dynamowidget.h"
#include "ui_dynamowidget.h"

#include "driver.h"
#include "testresultdata.h"
#include "channelsutils.h"
#include "dynamosignal.h"

#include <QDebug>

DynamoWidget::DynamoWidget(Driver *drv, const QString channelId, QWidget *parent) :
    SignalWidget(drv, channelId, parent),
    ui(new Ui::DynamoWidget)
{
    ui->setupUi(this);

//    m_dynControl = static_cast<DeviceProtocols::TensoControl*>(driver()->getDeviceControl(DeviceProtocols::uid_TensoControl, channelId));
//    if (m_dynControl)
//        m_dynControl->getTensoValueDiapasone(channelId, m_min, m_max);

    ui->wgtDynamoOscill->appendArea("", 1);
//    ui->wgtDynamoOscill->setDiapazone(0, m_min, m_max);
//    ui->lblDiagMin->setText(QString::number(m_min));
//    ui->lblDiagMax->setText(QString::number(m_max));
//    ui->pbDiag->setMinimum(static_cast<int>(m_min));
//    ui->pbDiag->setMaximum(static_cast<int>(m_max));

    ui->cbScale->addItems(QStringList() <<  "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128");

    connect(drv, &Driver::started, this, &DynamoWidget::on_started);
}

DynamoWidget::~DynamoWidget()
{
    delete ui;
}

void DynamoWidget::newProbe()
{
    if (ui->btnDynRecord->isChecked())
    {
        if (!m_dynamo)
            m_dynamo = new DynamoSignal(channelId(), ui->wgtDynamoOscill->frequency());
    }
}

void DynamoWidget::abortProbe()
{
    if (ui->btnDynRecord->isChecked())
        m_dynamo->clear();
}

void DynamoWidget::saveProbe()
{
    if (ui->btnDynRecord->isChecked() && m_dynamo)
        objTestResultData()->addChannel(m_dynamo);
}

void DynamoWidget::getData(DeviceProtocols::DeviceData *data)
{
    if (data->channelId() == channelId())
    {
        if (!m_dynControl)
        {
            m_dynControl = static_cast<DeviceProtocols::TensoControl*>(driver()->getDeviceControl(DeviceProtocols::uid_TensoControl, channelId()));
            m_dynControl->getTensoValueDiapasone(channelId(), m_min, m_max);
            ui->wgtDynamoOscill->setDiapazone(0, m_min, m_max);
            ui->lblDiagMin->setText(QString::number(m_min));
            ui->lblDiagMax->setText(QString::number(m_max));
            ui->pbDiag->setMinimum(static_cast<int>(m_min));
            ui->pbDiag->setMaximum(static_cast<int>(m_max));
        }

        DeviceProtocols::TensoDvcData *dynData = static_cast<DeviceProtocols::TensoDvcData*>(data);

        QVector<double> recDyn;
        double value = dynData->value(0).toDouble();
        recDyn << value;
        ui->wgtDynamoOscill->addValue(recDyn);
        ui->pbDiag->setValue(static_cast<int>(value));
        ui->lblDiagValue->setText(QString("%1 " + tr("кг")).arg(round(value)));

        if (value > m_valueMax)
        {
            m_valueMax = value;
            ui->lblMaxVal->setText(QString(tr("Макс") + " %1 " + tr("кг")).arg(round(m_valueMax)));
        }
    }
}

void DynamoWidget::record(DeviceProtocols::DeviceData *data)
{
    if (data->channelId() == channelId())
    {
        DeviceProtocols::TensoDvcData *dynData = static_cast<DeviceProtocols::TensoDvcData*>(data);
        if (ui->btnDynRecord->isChecked())
        {
            Q_ASSERT(m_dynamo);
            m_dynamo->addValue(dynData->value(0).toDouble());
        }
    }
}

void DynamoWidget::setFrequency(const int frequency)
{
    ui->wgtDynamoOscill->setFrequency(frequency);
}

void DynamoWidget::enabledControls(const bool enabled)
{
    ui->cbScale->setEnabled(enabled);
    ui->btnCalibrate->setEnabled(enabled);
    ui->btnDynRecord->setEnabled(enabled);
}

void DynamoWidget::setAllwaysRecordingChannel(const QString &channelId)
{
    Q_UNUSED(channelId);
}

void DynamoWidget::on_started()
{
    setRecordedChannels();
}

void DynamoWidget::on_resetValueMax()
{
    m_valueMax = 0;
}

void DynamoWidget::on_calibrate()
{
    if (m_dynControl)
        m_dynControl->calibrateTenso(channelId());
}

void DynamoWidget::on_scaleChange(int scaleIdx)
{
    int scale = 1;
    for (int i = 0; i < scaleIdx; ++i)
        scale *= 2;

    double maxV = m_max / scale;
    ui->wgtDynamoOscill->setDiapazone(0, m_min, maxV);
}

void DynamoWidget::on_dynRecChange(bool checked)
{
    setRecButton(ui->btnDynRecord, checked);

    if (checked)
    {
        if (!m_dynamo)
            m_dynamo = new DynamoSignal(channelId(), ui->wgtDynamoOscill->frequency());
    }
    else
    {
        if (m_dynamo)
            m_dynamo->clear();
//        delete m_dynamo;
    }
}

void DynamoWidget::setRecordedChannels()
{
    auto isRec = driver()->isChannelRecordingDefault(channelId());

    ui->btnDynRecord->setChecked(isRec);
    setRecButton(ui->btnDynRecord, isRec);
}

void DynamoWidget::setRecButton(QPushButton *btn, const bool checked)
{
    if (checked)
        btn->setIcon(QIcon(":/images/SaveOK.png"));
    else
        btn->setIcon(QIcon(":/images/SaveNO.png"));
}
