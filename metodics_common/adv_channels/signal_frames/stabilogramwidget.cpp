#include "stabilogramwidget.h"
#include "ui_stabilogramwidget.h"

StabilogramWidget::StabilogramWidget(const QString channelId, QWidget *parent) :
    SignalWidget(channelId, parent),
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

        QVector<double> recZ;
        recZ << stabData->z();
        ui->wgtZOscill->addValue(recZ);

        QString s = QString("X = %1").arg(stabData->x(), 0, 'f', 2) + "    " +
                    QString("Y = %1").arg(stabData->y(), 0, 'f', 2) + "    " +
                    QString("Z = %1").arg(stabData->z(), 0, 'f', 2);
//        ui->label->setText(s);
    }
}

void StabilogramWidget::setFrequency(const int frequency)
{
    ui->wgtStabOscill->setFrequency(frequency);
    ui->wgtZOscill->setFrequency(frequency);
}
