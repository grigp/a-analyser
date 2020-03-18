#include "pulsewidget.h"
#include "ui_pulsewidget.h"

#include <QTimer>

PulseWidget::PulseWidget(Driver *drv, const QString channelId, QWidget *parent) :
    SignalWidget(drv, channelId, parent),
    ui(new Ui::PulseWidget)
{
    ui->setupUi(this);
}

PulseWidget::~PulseWidget()
{
    delete ui;
}

void PulseWidget::newProbe()
{

}

void PulseWidget::abortProbe()
{

}

void PulseWidget::getData(DeviceProtocols::DeviceData *data)
{
    if (data->channelId() == channelId())
    {
        DeviceProtocols::PulseDvcData *pulseData = static_cast<DeviceProtocols::PulseDvcData*>(data);

        double value = pulseData->value(0).toDouble();
        ui->pbPulse->setValue(value);
        ui->lblPulse->setText(QString("ЧСС - %1 уд/мин").arg(value));

        m_pulseMiddle = m_pulseMiddle + value;
        ++m_pulseCount;

        ui->lblPulseMid->setText(QString("Средний пульс - %1 уд/мин").arg(m_pulseMiddle/m_pulseCount));

        QTimer::singleShot(100, [=]
        {
            ui->pbPulse->setValue(0);
        });

//        QVector<double> recDyn;
//        double value = dynData->value(0).toDouble();
//        recDyn << value;
//        ui->wgtDynamoOscill->addValue(recDyn);
//        ui->pbDiag->setValue(value);
//        ui->lblDiagValue->setText(QString("%1 " + tr("кг")).arg(round(value)));

//        if (value > m_valueMax)
//        {
//            m_valueMax = value;
//            ui->lblMaxVal->setText(QString(tr("Макс") + " %1 " + tr("кг")).arg(round(m_valueMax)));
//        }
    }
}

void PulseWidget::record(DeviceProtocols::DeviceData *data)
{

}

void PulseWidget::setFrequency(const int frequency)
{

}

void PulseWidget::enabledControls(const bool enabled)
{

}

void PulseWidget::setAllwaysRecordingChannel(const QString &channelId)
{

}

void PulseWidget::on_resetPulse()
{
    m_pulseMiddle = 0;
    m_pulseCount = 0;
}

