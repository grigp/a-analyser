#include "trenstabexecute.h"
#include "ui_trenstabexecute.h"

#include "driver.h"

#include <QLayout>
#include <QLabel>
#include <QDebug>

TrenStabExecute::TrenStabExecute(QWidget *parent) :
    TrenExecute(parent),
    ui(new Ui::TrenStabExecute)
{
    ui->setupUi(this);
}

TrenStabExecute::~TrenStabExecute()
{
    delete ui;
}

void TrenStabExecute::setParams(const QJsonObject &params)
{
    TrenExecute::setParams(params);

    m_scale = params["scale"].toInt();
}

void TrenStabExecute::start()
{
    TrenExecute::start();
    if (driver())
        m_dcControl = dynamic_cast<DeviceProtocols::DecartCoordControl*>(driver());
//        m_dcControl = static_cast<DeviceProtocols::DecartCoordControl*>(driver()->getDeviceControl(DeviceProtocols::uid_StabControl));
}

void TrenStabExecute::fillGameControl(QFrame *frame)
{
    m_btnZeroing = new QPushButton(frame);
    m_btnZeroing->setText(tr("Центровка"));
    frame->layout()->addWidget(m_btnZeroing);
    connect(m_btnZeroing, &QPushButton::clicked, this, &TrenStabExecute::on_zeroing);

    auto* frScale = new QFrame(frame);
    frame->layout()->addWidget(frScale);
    auto* frScaleLayout = new QHBoxLayout(frScale);
    frScale->setLayout(frScaleLayout);

    auto* lblScale = new QLabel(frScale);
    lblScale->setText(tr("Масштаб"));
    frScale->layout()->addWidget(lblScale);

    m_cbScale = new QComboBox(frScale);
    m_cbScale->addItems(QStringList() << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128");
    frScale->layout()->addWidget(m_cbScale);
    m_cbScale->setCurrentIndex(m_scale);
    connect(m_cbScale, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &TrenStabExecute::on_scaleChange);
}

void TrenStabExecute::elementsInteraction(DeviceProtocols::DeviceData *data)
{
    TrenExecute::elementsInteraction(data);
}

int TrenStabExecute::scale()
{
    return m_cbScale->currentIndex();
}

void TrenStabExecute::on_zeroing()
{
    auto chanUid = currentChannelUID();
    if (m_dcControl && chanUid != "")
            m_dcControl->zeroing(chanUid);
}

void TrenStabExecute::on_scaleChange(int scaleIdx)
{
    Q_UNUSED(scaleIdx);
}
