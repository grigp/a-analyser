#include "myogramwidget.h"
#include "ui_myogramwidget.h"

#include "driver.h"
#include "myogram.h"
#include "testresultdata.h"

#include <QDebug>

namespace  {
    const double FilterKoef = 0.01;
}

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
    ui->twViewMode->setCurrentIndex(0);
    setIndicatorsStyle();

    connect(drv, &Driver::started, this, &MyogramWidget::on_started);
}

MyogramWidget::~MyogramWidget()
{
    delete ui;
}

void MyogramWidget::newProbe()
{
    if (ui->btnMyoRecord->isChecked())
    {
        if (!m_myo)
            m_myo = new Myogram(channelId(), subChanCount(), ui->wgtMyoOscill->frequency());
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
        for (int i = 0; i < myoData->subChanCount(); ++i) {
            auto v = myoData->value(i).toDouble();
            recMyo << v;
            m_valueDisplay[i] = m_valueDisplay[i] * ( 1 - FilterKoef ) + v * FilterKoef;
            if (m_valueDisplay[i] > m_valueMax[i])
                m_valueMax[i] = m_valueDisplay[i];
            if (m_valueDisplay[i] < m_valueMin[i])
                m_valueMin[i] = m_valueDisplay[i];
        }
        showChannelValues(0, ui->indChan1Value, ui->indChan1ValueCur, ui->indChan1Decrease, ui->indChan1Min, ui->indChan1Max, ui->indChan1Diap);
        showChannelValues(1, ui->indChan2Value, ui->indChan2ValueCur, ui->indChan2Decrease, ui->indChan2Min, ui->indChan2Max, ui->indChan2Diap);
        showChannelValues(2, ui->indChan3Value, ui->indChan3ValueCur, ui->indChan3Decrease, ui->indChan3Min, ui->indChan3Max, ui->indChan3Diap);
        showChannelValues(3, ui->indChan4Value, ui->indChan4ValueCur, ui->indChan4Decrease, ui->indChan4Min, ui->indChan4Max, ui->indChan4Diap);

        ui->wgtMyoOscill->addValue(recMyo);
    }
}

void MyogramWidget::record(DeviceProtocols::DeviceData *data)
{
    if (data->channelId() == channelId())
    {
        DeviceProtocols::MyogramDvcData *myoData = static_cast<DeviceProtocols::MyogramDvcData*>(data);
        if (ui->btnMyoRecord->isChecked())
        {
            Q_ASSERT(m_myo);
            QVector<double> myoRec;
            for (int i = 0; i < myoData->subChanCount(); ++i)
                if (isSubChanRecord(i))
                    myoRec << myoData->value(i).toDouble();
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
            m_myo = new Myogram(channelId(), subChanCount(), ui->wgtMyoOscill->frequency());
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
    int n = subChanCount();
    if (m_myo)
        m_myo->setSubChansCount(n);
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

void MyogramWidget::on_resetValuesChan1()
{
    m_valueMin[0] = 10000;
    m_valueMax[0] = -10000;
}

void MyogramWidget::on_resetValuesChan2()
{
    m_valueMin[1] = 10000;
    m_valueMax[1] = -10000;
}

void MyogramWidget::on_resetValuesChan3()
{
    m_valueMin[2] = 10000;
    m_valueMax[2] = -10000;
}

void MyogramWidget::on_resetValuesChan4()
{
    m_valueMin[3] = 10000;
    m_valueMax[3] = -10000;
}

void MyogramWidget::setSubChannelsRecButtons()
{
    auto items = ui->gbSubChannels->children();
    m_subChanels.clear();
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
            m_subChanels << btn;
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

bool MyogramWidget::isSubChanRecord(const int scn) const
{
    Q_ASSERT(scn >= 0 && scn < m_subChanels.size());
    return m_subChanels.at(scn)->isChecked();
}

int MyogramWidget::subChanCount() const
{
    int retval = 0;
    for (int i = 0; i < m_subChanels.size(); ++i)
        if (isSubChanRecord(i))
            ++retval;
    return retval;
}

void MyogramWidget::setIndicatorsStyle()
{
    ui->indChan1Value->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan1ValueCur->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan1Decrease->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan1Min->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan1Max->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan1Diap->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");

    ui->indChan2Value->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan2ValueCur->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan2Decrease->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan2Min->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan2Max->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan2Diap->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");

    ui->indChan3Value->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan3ValueCur->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan3Decrease->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan3Min->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan3Max->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan3Diap->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");

    ui->indChan4Value->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan4ValueCur->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan4Decrease->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan4Min->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan4Max->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
    ui->indChan4Diap->setStyleSheet("font-size: 14pt; color: rgb(70, 70, 0);");
}

void MyogramWidget::showChannelValues(const int idx,
                                      QLabel* lblValue,
                                      QLabel* lblValueCur,
                                      QLabel* lblDecrease,
                                      QLabel* lblMin,
                                      QLabel* lblMax,
                                      QLabel* lblDiap
                                      )
{
    lblValue->setText(QString::number(m_valueDisplay[idx], 'f', 4));
    lblValueCur->setText(QString::number(m_valueDisplay[idx] * 1000, 'f', 0));
    lblDecrease->setText(QString::number(std::log(m_valueDisplay[idx] / 2) * 20 / std::log(10), 'f', 1));
    lblMin->setText(QString::number(m_valueMin[idx] * 1000, 'f', 3));
    lblMax->setText(QString::number(m_valueMax[idx] * 1000, 'f', 3));
    lblDiap->setText(QString::number((m_valueMax[idx] - m_valueMin[idx]) * 1000, 'f', 3));
}

