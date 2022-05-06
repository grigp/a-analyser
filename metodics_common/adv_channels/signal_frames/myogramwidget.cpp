#include "myogramwidget.h"
#include "ui_myogramwidget.h"

#include "driver.h"
#include "myogram.h"
#include "testresultdata.h"

#include <QDebug>

MyogramWidget::MyogramWidget(Driver *drv, const QString channelId, QWidget *parent)
    : SignalWidget(drv, channelId, parent),
    ui(new Ui::MyogramWidget)
{
    ui->setupUi(this);

    m_myoControl = static_cast<DeviceProtocols::MyoControl*>(driver()->getDeviceControl(DeviceProtocols::uid_MyoControl, channelId));
    if (m_myoControl)
        m_amplMyo = m_myoControl->amplitudeMyo();


    for (int i = 0; i < driver()->getSubChannelsCount(channelId); ++i)
        ui->wgtMyoOscill->appendArea(QString(tr("Отведение") + " %1").arg(i + 1), 1);
    for (int i = 0; i < driver()->getSubChannelsCount(channelId); ++i)
        ui->wgtMyoOscill->setDiapazone(i, 0, m_amplMyo);

    ui->cbScale->addItems(QStringList() <<  "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128" << "256" << "512");

    connect(drv, &Driver::started, this, &MyogramWidget::on_started);
}

MyogramWidget::~MyogramWidget()
{
    delete ui;
}

void MyogramWidget::newProbe()
{
    setRecordedChannels();
    if (ui->btnMyoRecord->isChecked())
    {
        if (!m_myo)
        {
            m_myo = new Myogram(ChannelsDefines::chanMyogram, driver()->getSubChannelsCount(channelId()), ui->wgtMyoOscill->frequency());
        }
    }
}

void MyogramWidget::abortProbe()
{
    if (ui->btnMyoRecord->isChecked())
        m_myo->clear();
}

void MyogramWidget::saveProbe()
{
    if (ui->btnMyoRecord->isChecked() && m_myo)
        objTestResultData()->addChannel(m_myo);
}

void MyogramWidget::getData(DeviceProtocols::DeviceData *data)
{
    if (data->channelId() == channelId())
    {
        DeviceProtocols::MyogramDvcData *myoData = static_cast<DeviceProtocols::MyogramDvcData*>(data);

        QVector<double> recMyo;
        for (int i = 0; i < myoData->subChanCount(); ++i)
            recMyo << myoData->value(i).toDouble();
        ui->wgtMyoOscill->addValue(recMyo);
//        m_recStab = recMyo;
    }
}

void MyogramWidget::record(DeviceProtocols::DeviceData *data)
{
    if (data->channelId() == channelId())
    {
        DeviceProtocols::MyogramDvcData *myoData = static_cast<DeviceProtocols::MyogramDvcData*>(data);
        if (ui->btnMyoRecord->isChecked())
        {
            QVector<double> myoRec;
            myoRec.resize(myoData->subChanCount());
            for (int i = 0; i < myoData->subChanCount(); ++i)
                myoRec[i] = myoData->value(i).toDouble();
            m_myo->addValue(myoRec);
        }
    }
}

void MyogramWidget::setFrequency(const int frequency)
{
    ui->wgtMyoOscill->setFrequency(frequency);
}

void MyogramWidget::enabledControls(const bool enabled)
{
    ui->lblScale->setEnabled(enabled);
    ui->cbScale->setEnabled(enabled);
    ui->btnMyoRecord->setEnabled(enabled);
    foreach (auto* btn, m_btnSubChans)
        btn->setEnabled(enabled);
}

void MyogramWidget::setAllwaysRecordingChannel(const QString &channelId)
{
    Q_UNUSED(channelId);
}

void MyogramWidget::on_started()
{
    setRecordedChannels();
}

void MyogramWidget::on_recMyoClick(bool checked)
{
    setRecButton(ui->btnMyoRecord, checked);

    if (checked)
    {
        if (!m_myo)
            m_myo = new Myogram(ChannelsDefines::chanMyogram, driver()->getSubChannelsCount(channelId()), ui->wgtMyoOscill->frequency());
    }
    else
    {
        if (m_myo)
        {
            m_myo->clear();
//            delete m_myo;
        }
    }
}

void MyogramWidget::on_recMyoChanClick(bool checked)
{
    setRecButton(static_cast<QPushButton*>(sender()), checked);
}

void MyogramWidget::on_myoScaleChange(int scaleIdx)
{
    int scale = 1;
    for (int i = 0; i < scaleIdx; ++i)
        scale *= 2;

    double maxV = m_amplMyo / scale;

    for (int i = 0; i < driver()->getSubChannelsCount(channelId()); ++i)
        ui->wgtMyoOscill->setDiapazone(i, 0, maxV);
}

void MyogramWidget::setSubChannelsRecButtons()
{
    auto items = ui->gbSubChannels->children();
    if (items.size() <= 1)
    {
        for (int i = 0; i < driver()->getSubChannelsCount(channelId()); ++i)
        {
            auto* btn = new QPushButton(ui->gbSubChannels);
            btn->setText("");
            btn->setCheckable(true);
            auto isDefault = driver()->isChannelRecordingDefault(channelId(), i);   //"MyoChan" + QString::number(i));
            btn->setChecked(isDefault);
            setRecButton(btn, isDefault);
            ui->gbSubChannels->layout()->addWidget(btn);
            m_btnSubChans.append(btn);
            connect(btn, &QPushButton::clicked, this, &MyogramWidget::on_recMyoChanClick);
        }
    }
}

void MyogramWidget::setRecordedChannels()
{
    auto isRec = driver()->isChannelRecordingDefault(channelId());

    setSubChannelsRecButtons();

    ui->btnMyoRecord->setChecked(isRec);
    setRecButton(ui->btnMyoRecord, isRec);
}

void MyogramWidget::setRecButton(QPushButton *btn, const bool checked)
{
    if (checked)
        btn->setIcon(QIcon(":/images/SaveOK.png"));
    else
        btn->setIcon(QIcon(":/images/SaveNO.png"));
}
