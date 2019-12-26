#include "stabilogramwidget.h"
#include "ui_stabilogramwidget.h"

StabilogramWidget::StabilogramWidget(const QString channelId, QWidget *parent) :
    SignalWidget(channelId, parent),
    ui(new Ui::StabilogramWidget)
{
    ui->setupUi(this);
    ui->wgtStabOscill->appendArea("Стабилограмма", 1);
    ui->wgtZOscill->appendArea("Баллистограмма", 1);
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

        QString s = QString("X = %1").arg(stabData->x(), 0, 'f', 2) + "    " +
                    QString("Y = %1").arg(stabData->y(), 0, 'f', 2) + "    " +
                    QString("Z = %1").arg(stabData->z(), 0, 'f', 2);
//        ui->label->setText(s);
    }
}
