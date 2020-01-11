#include "stabilogramwidget.h"
#include "ui_stabilogramwidget.h"

#include "driver.h"
#include "stabilogram.h"
#include "ballistogram.h"
#include "testresultdata.h"
#include "channelsutils.h"

#include <QDebug>

StabilogramWidget::StabilogramWidget(Driver *driver, const QString channelId, QWidget *parent) :
    SignalWidget(driver, channelId, parent),
    ui(new Ui::StabilogramWidget)
{
    ui->setupUi(this);
    ui->wgtStabOscill->appendArea("", 2);
    ui->wgtZOscill->appendArea("", 1);
    ui->wgtStabOscill->setDiapazone(0, -128, 128);
    ui->wgtZOscill->setDiapazone(0, 0, 150);

    ui->cbStabScale->addItems(QStringList() << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128");
    ui->cbZScale->addItems(QStringList() <<  "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128" << "256" << "512");
    ui->cbStabScale->setCurrentIndex(0);
    ui->cbZScale->setCurrentIndex(0);
    ui->tabWindows->setCurrentIndex(0);

    setRecordedChannels();

    ui->lblMassa->installEventFilter(this);
}

StabilogramWidget::~StabilogramWidget()
{
    delete ui;
}

void StabilogramWidget::newProbe()
{
    if (ui->btnStabRecord->isChecked())
    {
        m_stb = new Stabilogram(ChannelsDefines::chanStab, ui->wgtStabOscill->frequency());
        objTestResultData()->addChannel(m_stb);
    }
    if (ui->btnZRecord->isChecked())
    {
        m_z = new Ballistogram(ChannelsDefines::chanZ, ui->wgtZOscill->frequency());
        objTestResultData()->addChannel(m_z);
    }
}

void StabilogramWidget::abortProbe()
{
    if (ui->btnStabRecord->isChecked())
        m_stb->clear();
    if (ui->btnZRecord->isChecked())
        m_z->clear();
}

void StabilogramWidget::getData(DeviceProtocols::DeviceData *data)
{
    if (data->channelId() == channelId())
    {
        DeviceProtocols::StabDvcData *stabData = static_cast<DeviceProtocols::StabDvcData*>(data);

        QVector<double> recStab;
        recStab << stabData->x() << stabData->y();
        ui->wgtStabOscill->addValue(recStab);
        m_recStab = recStab;

        QVector<double> recZ;
        recZ << stabData->z();
        ui->wgtZOscill->addValue(recZ);
        m_recZ = recZ;

        showAdvancedInfo(stabData);

        if (m_isShowMassa)
            ui->lblMassa->setText(QString(tr("Масса") + " %1" + " " + tr("кг")).arg(m_recZ.at(0)));

    }
}

void StabilogramWidget::record(DeviceProtocols::DeviceData *data)
{
    if (data->channelId() == channelId())
    {
        DeviceProtocols::StabDvcData *stabData = static_cast<DeviceProtocols::StabDvcData*>(data);
        if (ui->btnStabRecord->isChecked())
        {
            SignalsDefines::StabRec rec(std::make_tuple(stabData->x(), stabData->y()));
            m_stb->addValue(rec);
        }
        if (ui->btnZRecord->isChecked())
            m_z->addValue(stabData->z());
    }
}

void StabilogramWidget::setFrequency(const int frequency)
{
    ui->wgtStabOscill->setFrequency(frequency);
    ui->wgtZOscill->setFrequency(frequency);
}

void StabilogramWidget::enabledControls(const bool enabled)
{
    ui->btnStabRecord->setEnabled(enabled);
    ui->btnStabZeroing->setEnabled(enabled);
    ui->btnZCalibrate->setEnabled(enabled);
    ui->btnZRecord->setEnabled(enabled);
    ui->cbStabScale->setEnabled(enabled);
    ui->cbZScale->setEnabled(enabled);
}

void StabilogramWidget::setAllwaysRecordingChannel(const QString &channelId)
{
    Q_UNUSED(channelId);
    ui->btnStabRecord->setChecked(true);
    setRecButton(ui->btnStabRecord, true);
    ui->btnStabRecord->setVisible(false);
}

bool StabilogramWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->lblMassa && event->type() == QEvent::MouseButtonDblClick)
        m_isShowMassa = true;

    return QWidget::eventFilter(watched, event);
}

void StabilogramWidget::on_changeScaleStab(int scaleIdx)
{
    double minV = -128;
    double maxV = 128;

    int scale = 1;
    for (int i = 0; i < scaleIdx; ++i)
        scale *= 2;

    double diap = (maxV - minV) / scale;
    double midV = (maxV + minV) / 2;
    minV = midV - diap / 2;
    maxV = midV + diap / 2;
    ui->wgtStabOscill->setDiapazone(0, minV, maxV);
}

void StabilogramWidget::on_changeScaleZ(int scaleIdx)
{
    int scale = 1;
    for (int i = 0; i < scaleIdx; ++i)
        scale *= 2;

    double maxV = 150 / scale;
    ui->wgtZOscill->setDiapazone(0, 0, maxV);
}

void StabilogramWidget::on_zeroingStab()
{
    ui->wgtStabOscill->area(0)->setOffset(0, m_recStab.at(0));
    ui->wgtStabOscill->area(0)->setOffset(1, m_recStab.at(1));
}

void StabilogramWidget::on_calibrate()
{
    DeviceProtocols::StabControl* stabControl = dynamic_cast<DeviceProtocols::StabControl*>(driver());
    if (stabControl)
        stabControl->calibrate(ChannelsDefines::chanStab);
}

void StabilogramWidget::on_recStabClick(bool checked)
{
    setRecButton(ui->btnStabRecord, checked);
}

void StabilogramWidget::on_recZClick(bool checked)
{
    setRecButton(ui->btnZRecord, checked);
}

void StabilogramWidget::showAdvancedInfo(DeviceProtocols::StabDvcData *stabData)
{
    ui->lblReactAValue->setText(QString::number(stabData->a(), 'f', 2));
    ui->lblReactBValue->setText(QString::number(stabData->b(), 'f', 2));
    ui->lblReactCValue->setText(QString::number(stabData->c(), 'f', 2));
    ui->lblReactDValue->setText(QString::number(stabData->d(), 'f', 2));

    auto summ = stabData->a() + stabData->b() + stabData->c() + stabData->d();
    ui->lblSumm->setText(QString(tr("Сумма") + " = %1").arg(QString::number(summ, 'f', 2)));

    ui->pbValueA->setValue(stabData->a() / 40.0 * ui->pbValueA->maximum());
    ui->pbValueB->setValue(stabData->b() / 40.0 * ui->pbValueB->maximum());
    ui->pbValueC->setValue(stabData->c() / 40.0 * ui->pbValueC->maximum());
    ui->pbValueD->setValue(stabData->d() / 40.0 * ui->pbValueD->maximum());
}

void StabilogramWidget::setRecordedChannels()
{
    auto isRecStab = driver()->isChannelRecordingDefault(channelId());
    auto zChannelId = ChannelsUtils::instance().zChanForStabChan(channelId());
    auto isRecZ = false;
    if (zChannelId != "")
        isRecZ = driver()->isChannelRecordingDefault(zChannelId);

    ui->btnStabRecord->setChecked(isRecStab);
    setRecButton(ui->btnStabRecord, isRecStab);

    ui->btnZRecord->setChecked(isRecZ);
    setRecButton(ui->btnZRecord, isRecZ);
}

void StabilogramWidget::setRecButton(QPushButton *btn, const bool checked)
{
    if (checked)
        btn->setIcon(QIcon(":/images/SaveOK.png"));
    else
        btn->setIcon(QIcon(":/images/SaveNO.png"));
}
