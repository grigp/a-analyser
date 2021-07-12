#include "pulsewidget.h"
#include "ui_pulsewidget.h"

#include "driver.h"
#include "ritmogram.h"
#include "testresultdata.h"

#include <QDebug>

PulseWidget::PulseWidget(Driver *drv, const QString channelId, QWidget *parent) :
    SignalWidget(drv, channelId, parent),
    ui(new Ui::PulseWidget)
{
    ui->setupUi(this);

    ui->wgtRitmogram->appendArea("", 1);
    ui->wgtRitmogram->setDiapazone(0, 0, 200);
    ui->wgtRitmogram->setSecundLabels(false);

    setRecordedChannels();
}

PulseWidget::~PulseWidget()
{
    m_isFinish = true;
    delete ui;
}

void PulseWidget::newProbe()
{
    if (ui->btnRecord->isChecked())
    {
        m_ritmogram = new Ritmogram(channelId(), 10);
        objTestResultData()->addChannel(m_ritmogram);
    }
}

void PulseWidget::abortProbe()
{
    if (ui->btnRecord->isChecked())
    {
        m_ritmogram->clear();
        delete m_ritmogram;
    }
}

void PulseWidget::getData(DeviceProtocols::DeviceData *data)
{
    if (data->channelId() == channelId())
    {
        DeviceProtocols::PulseDvcData *pulseData = static_cast<DeviceProtocols::PulseDvcData*>(data);

        double value = pulseData->value(0).toDouble();
        ui->pbPulse->setValue(static_cast<int>(value));
        ui->lblPulse->setText(QString("ЧСС - %1 уд/мин").arg(value));

        m_pulseMiddle = m_pulseMiddle + value;
        ++m_pulseCount;

        ui->lblPulseMid->setText(QString("Средний пульс - %1 уд/мин").arg(m_pulseMiddle/m_pulseCount));

        //! Погасим индикатор через 100 мс
        QTimer::singleShot(100, [=]
        {
            if (!m_isFinish)
            {
                ui->pbPulse->setValue(0);
            }
        });

        QVector<double> recPulse;
        recPulse << value;
        ui->wgtRitmogram->addValue(recPulse);
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
    if (data->channelId() == channelId())
    {
        DeviceProtocols::PulseDvcData *pulseData = static_cast<DeviceProtocols::PulseDvcData*>(data);
        if (ui->btnRecord->isChecked())
            m_ritmogram->addValue(pulseData->value(0).toDouble());
    }
}

void PulseWidget::setFrequency(const int frequency)
{
    Q_UNUSED(frequency);
}

void PulseWidget::enabledControls(const bool enabled)
{
    ui->btnReset->setEnabled(enabled);
    ui->btnRecord->setEnabled(enabled);
}

void PulseWidget::setAllwaysRecordingChannel(const QString &channelId)
{
    Q_UNUSED(channelId);
}

void PulseWidget::on_resetPulse()
{
    m_pulseMiddle = 0;
    m_pulseCount = 0;
    ui->wgtRitmogram->clear();
    ui->wgtRitmogram->appendArea("", 1);
    ui->wgtRitmogram->setDiapazone(0, 0, 200);
}

void PulseWidget::on_pulseRecChange(bool checked)
{
    setRecButton(ui->btnRecord, checked);
}

void PulseWidget::setRecordedChannels()
{
    auto isRec = driver()->isChannelRecordingDefault(channelId());

    ui->btnRecord->setChecked(isRec);
    setRecButton(ui->btnRecord, isRec);
}

void PulseWidget::setRecButton(QPushButton *btn, const bool checked)
{
    if (checked)
        btn->setIcon(QIcon(":/images/SaveOK.png"));
    else
        btn->setIcon(QIcon(":/images/SaveNO.png"));
}

