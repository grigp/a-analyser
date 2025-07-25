#include "pulsewidget.h"
#include "ui_pulsewidget.h"

#include "driver.h"
#include "ritmogram.h"
#include "testresultdata.h"
#include "settingsprovider.h"

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

    auto valIsDel = SettingsProvider::valueFromRegAppCopy("PulseRecord", "DeleteArtifacts", false).toBool();
    ui->cbDelAtf->setChecked(valIsDel);
    setDelAtf(valIsDel);

    auto valLoBnd = SettingsProvider::valueFromRegAppCopy("PulseRecord", "ArtifactBoundLower", 30).toInt();
    ui->edLoBound->setValue(valLoBnd);

    auto valUpBnd = SettingsProvider::valueFromRegAppCopy("PulseRecord", "ArtifactBoundUpper", 200).toInt();
    ui->edUpBound->setValue(valUpBnd);
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

        //! Проверка в режиме отсечения артефактов
        bool isCorrectValue = true;
        if (ui->cbDelAtf->isChecked())
            isCorrectValue = (value >= ui->edLoBound->value() && value <= ui->edUpBound->value());

        //! Регистрация удара пульса
        if (isCorrectValue)
        {
            ui->pbPulse->setValue(static_cast<int>(value));
            ui->lblPulse->setText(QString(tr("ЧСС - %1 уд/мин")).arg(value));

            m_pulseMiddle = m_pulseMiddle + value;
            ++m_pulseCount;

            ui->lblPulseMid->setText(QString(tr("Средний пульс - %1 уд/мин")).arg(m_pulseMiddle/m_pulseCount));

            //! Погасим индикатор через 100 мс с использованием таймера объекта
            m_pickTimerId = startTimer(100);

            QVector<double> recPulse;
            recPulse << value;
            ui->wgtRitmogram->addValue(recPulse);
        }
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

void PulseWidget::on_delAtfMode(bool value)
{
    setDelAtf(value);
    SettingsProvider::setValueToRegAppCopy("PulseRecord", "DeleteArtifacts", value);
}

void PulseWidget::on_changeLoBound(int value)
{
    SettingsProvider::setValueToRegAppCopy("PulseRecord", "ArtifactBoundLower", value);
}

void PulseWidget::on_changeUpBound(int value)
{
    SettingsProvider::setValueToRegAppCopy("PulseRecord", "ArtifactBoundUpper", value);
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
    {
        btn->setIcon(QIcon(":/images/SaveOK.png"));
        btn->setToolTip(tr("Сигнал будет записан"));
    }
    else
    {
        btn->setIcon(QIcon(":/images/SaveNO.png"));
        btn->setToolTip(tr("Сигнал не будет записан"));
    }
}

void PulseWidget::setDelAtf(const bool isDelete)
{
    ui->frLoBound->setEnabled(isDelete);
    ui->frUpBound->setEnabled(isDelete);
}

