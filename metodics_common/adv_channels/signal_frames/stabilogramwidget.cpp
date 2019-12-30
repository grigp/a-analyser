#include "stabilogramwidget.h"
#include "ui_stabilogramwidget.h"

#include "driver.h"

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

    ui->lblMassa->installEventFilter(this);
}

StabilogramWidget::~StabilogramWidget()
{
    delete ui;
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

        QString s = QString("X = %1").arg(stabData->x(), 0, 'f', 2) + "    " +
                    QString("Y = %1").arg(stabData->y(), 0, 'f', 2) + "    " +
                    QString("Z = %1").arg(stabData->z(), 0, 'f', 2);

        if (m_isShowMassa)
            ui->lblMassa->setText(QString(tr("Масса") + " %1" + " " + tr("кг")).arg(m_recZ.at(0)));
    }
}

void StabilogramWidget::setFrequency(const int frequency)
{
    ui->wgtStabOscill->setFrequency(frequency);
    ui->wgtZOscill->setFrequency(frequency);
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
