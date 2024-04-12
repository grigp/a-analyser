#include "boxerdodgingparamsdialog.h"
#include "ui_boxerdodgingparamsdialog.h"

#include "aanalyserapplication.h"
#include "amessagebox.h"

BoxerDodgingParamsDialog::BoxerDodgingParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoxerDodgingParamsDialog)
{
    ui->setupUi(this);
}

BoxerDodgingParamsDialog::~BoxerDodgingParamsDialog()
{
    delete ui;
}

void BoxerDodgingParamsDialog::setParams(const QJsonObject &params)
{
    ui->edDeviationThreshold->setValue(params["deviation_threshold"].toInt());
    auto tm = params["time"].toInt();
    ui->edTime->setTime(QTime(0, tm / 60, tm % 60));
    ui->edStimulTimeMin->setValue(params["min_len"].toInt());
    ui->edStimulTimeMax->setValue(params["max_len"].toInt());
}

QJsonObject BoxerDodgingParamsDialog::getParams()
{
    QJsonObject retval;

    retval["deviation_threshold"] = ui->edDeviationThreshold->value();
    auto tm = ui->edTime->time();
    retval["time"] = tm.minute() * 60 + tm.second();
    retval["min_len"] = ui->edStimulTimeMin->value();
    retval["max_len"] = ui->edStimulTimeMax->value();

    return retval;
}

void BoxerDodgingParamsDialog::on_ok()
{
    accept();
}

void BoxerDodgingParamsDialog::on_cancel()
{
    reject();
}

void BoxerDodgingParamsDialog::on_default()
{
    auto mr = AMessageBox::question(nullptr, tr("Запрос"), tr("Сбросить настройки к настройкам методики по умолчанию?"));
    if (QMessageBox::Yes == mr)
    {
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        auto params = static_cast<AAnalyserApplication*>(QApplication::instance())->setMetodicParamsDefault(mi.uid);
        setParams(params);
    }
}
