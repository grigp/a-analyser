#include "triangleparamsdialog.h"
#include "ui_triangleparamsdialog.h"

#include "baseutils.h"

#include <QDebug>

TriangleParamsDialog::TriangleParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TriangleParamsDialog)
{
    ui->setupUi(this);

    ui->cbDirectionMode->addItems(QStringList()
                           << tr("По часовой стрелке")
                           << tr("Против часовой стрелки"));
}

TriangleParamsDialog::~TriangleParamsDialog()
{
    delete ui;
}

void TriangleParamsDialog::setParams(const QJsonObject &params)
{
    auto dm = params["direction_mode"].toString();
    ui->cbDirectionMode->setCurrentIndex(BaseUtils::DirectionModeValueIndex.value(dm) - 1);

    ui->edForce->setValue(params["force"].toInt());
    ui->edStageTime->setValue(params["stage_time"].toInt());
    ui->edMinDeviation->setValue(params["min_deviation"].toInt());
    ui->cbShowMarkerAnal->setChecked(params["show_marker_anal"].toBool());
}

QJsonObject TriangleParamsDialog::getParams()
{
    QJsonObject retval;
    auto valDM = static_cast<BaseUtils::DirectionMode>(ui->cbDirectionMode->currentIndex() + 1);
    retval["direction_mode"] = BaseUtils::DirectionModeValueName.value(valDM);

    retval["force"] = ui->edForce->value();
    retval["stage_time"] = ui->edStageTime->value();
    retval["min_deviation"] = ui->edMinDeviation->value();
    retval["show_marker_anal"] = ui->cbShowMarkerAnal->isChecked();

    return retval;
}
