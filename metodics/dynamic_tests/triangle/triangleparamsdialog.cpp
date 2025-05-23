#include "triangleparamsdialog.h"
#include "ui_triangleparamsdialog.h"

#include "aanalyserapplication.h"
#include "basedefines.h"
#include "amessagebox.h"

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
    ui->cbDirectionMode->setCurrentIndex(BaseDefines::DirectionModeValueIndex.value(dm) - 1);

    auto tt = params["training_time"].toInt();
    auto at = params["analysis_time"].toInt();
    ui->edTimeTraining->setTime(QTime(0, tt / 60, tt % 60));
    ui->edTimeAnalysis->setTime(QTime(0, at / 60, at % 60));
    ui->edForce->setValue(params["force"].toInt());
    ui->edStageTime->setValue(params["stage_time"].toInt());
    ui->edMinDeviation->setValue(params["min_deviation"].toInt());
    ui->cbShowMarkerAnal->setChecked(params["show_marker_anal"].toBool());
}

QJsonObject TriangleParamsDialog::getParams()
{
    QJsonObject retval;
    auto valDM = static_cast<BaseDefines::DirectionMode>(ui->cbDirectionMode->currentIndex() + 1);
    retval["direction_mode"] = BaseDefines::DirectionModeValueName.value(valDM);

    auto tt = ui->edTimeTraining->time();
    auto at = ui->edTimeAnalysis->time();
    retval["training_time"] = tt.minute() * 60 + tt.second();
    retval["analysis_time"] = at.minute() * 60 + at.second();
    retval["force"] = ui->edForce->value();
    retval["stage_time"] = ui->edStageTime->value();
    retval["min_deviation"] = ui->edMinDeviation->value();
    retval["show_marker_anal"] = ui->cbShowMarkerAnal->isChecked();

    return retval;
}

void TriangleParamsDialog::on_ok()
{
    accept();
}

void TriangleParamsDialog::on_cancel()
{
    reject();
}

void TriangleParamsDialog::on_default()
{
    auto mr = AMessageBox::question(nullptr, tr("Запрос"), tr("Сбросить настройки к настройкам методики по умолчанию?"));
    if (AMessageBox::Yes == mr)
    {
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        auto params = static_cast<AAnalyserApplication*>(QApplication::instance())->setMetodicParamsDefault(mi.uid);
        setParams(params);
    }
}
