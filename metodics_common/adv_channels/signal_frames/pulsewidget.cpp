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
//        objTestResultData()->addChannel(m_ritmogram);
    }
}

void PulseWidget::abortProbe()
{
    if (ui->btnRecord->isChecked())
    {
        m_ritmogram->clear();
//        delete m_ritmogram;
    }
}

void PulseWidget::saveProbe()
{
    if (ui->btnRecord->isChecked() && m_ritmogram)
        objTestResultData()->addChannel(m_ritmogram);
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

        //! Погасим индикатор через 100 мс с использованием таймера объекта
        m_pickTimerId = startTimer(100);

        QVector<double> recPulse;
        recPulse << value;
        ui->wgtRitmogram->addValue(recPulse);
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

void PulseWidget::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_pickTimerId && m_pickTimerId > -1)
    {
        if (!m_isFinish)
        {
            killTimer(m_pickTimerId);
            m_pickTimerId = -1;
            ui->pbPulse->setValue(0);
        }
    }
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
    if (checked)
    {
        if (!m_ritmogram)
            m_ritmogram = new Ritmogram(channelId(), 10);
//        objTestResultData()->addChannel(m_ritmogram);
    }
    else
    {
        if (m_ritmogram)
            m_ritmogram->clear();
//        delete m_ritmogram;
    }
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

