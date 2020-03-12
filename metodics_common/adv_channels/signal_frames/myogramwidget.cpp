#include "myogramwidget.h"
#include "ui_myogramwidget.h"

#include "driver.h"
#include "myogram.h"
#include "testresultdata.h"

#include <QDebug>

MyogramWidget::MyogramWidget(Driver *driver, const QString channelId, QWidget *parent)
    : SignalWidget(driver, channelId, parent),
    ui(new Ui::MyogramWidget)
{
    ui->setupUi(this);

    ui->wgtMyoOscill->appendArea(tr("Отведение 1"), 1);
    ui->wgtMyoOscill->appendArea(tr("Отведение 2"), 1);
    ui->wgtMyoOscill->appendArea(tr("Отведение 3"), 1);
    ui->wgtMyoOscill->appendArea(tr("Отведение 4"), 1);
    ui->wgtMyoOscill->setDiapazone(0, 0, 2);
    ui->wgtMyoOscill->setDiapazone(1, 0, 2);
    ui->wgtMyoOscill->setDiapazone(2, 0, 2);
    ui->wgtMyoOscill->setDiapazone(3, 0, 2);
//    ui->wgtMyoOscill->setFrequency(driver->frequency(channelId));

    ui->cbScale->addItems(QStringList() <<  "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128" << "256" << "512");

    setRecordedChannels();
}

MyogramWidget::~MyogramWidget()
{
    delete ui;
}

void MyogramWidget::newProbe()
{
    if (ui->btnMyoRecord->isChecked())
    {
        m_myo = new Myogram(ChannelsDefines::chanMyogram, driver()->getSubChannelsCount(channelId()), ui->wgtMyoOscill->frequency());
        objTestResultData()->addChannel(m_myo);
    }
}

void MyogramWidget::abortProbe()
{
    if (ui->btnMyoRecord->isChecked())
    {
        m_myo->clear();
        delete m_myo;
    }
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

void MyogramWidget::on_recMyoClick(bool checked)
{
    setRecButton(ui->btnMyoRecord, checked);
}

void MyogramWidget::on_recMyoChanClick(bool checked)
{
    setRecButton(static_cast<QPushButton*>(sender()), checked);
}

void MyogramWidget::setSubChannelsRecButtons()
{
    for (int i = 0; i < driver()->getSubChannelsCount(channelId()); ++i)
    {
        auto* btn = new QPushButton(ui->gbSubChannels);
        btn->setText("");
        btn->setCheckable(true);
        btn->setChecked(driver()->isChannelRecordingDefault("MyoChan" + QString::number(i)));
        setRecButton(btn, driver()->isChannelRecordingDefault("MyoChan" + QString::number(i)));
        ui->gbSubChannels->layout()->addWidget(btn);
        m_btnSubChans.append(btn);
        connect(btn, &QPushButton::clicked, this, &MyogramWidget::on_recMyoChanClick);
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
