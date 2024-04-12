#include "stepdeviationparamsdialog.h"
#include "ui_stepdeviationparamsdialog.h"

#include "aanalyserapplication.h"
#include "basedefines.h"
#include "amessagebox.h"

StepDeviationParamsDialog::StepDeviationParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StepDeviationParamsDialog)
{
    ui->setupUi(this);

    ui->cbDirection->addItems(QStringList()
                           << tr("Вперед")
                           << tr("Вправо")
                           << tr("Назад")
                           << tr("Влево"));
}

StepDeviationParamsDialog::~StepDeviationParamsDialog()
{
    delete ui;
}

void StepDeviationParamsDialog::setParams(const QJsonObject &params)
{
    ui->edMaxTime->setValue(params["max_time"].toInt());

    auto d = params["direction"].toString();
    ui->cbDirection->setCurrentIndex(BaseDefines::DirectionValueIndex.value(d));
}

QJsonObject StepDeviationParamsDialog::getParams()
{
    QJsonObject retval;
    retval["max_time"] = ui->edMaxTime->value();

    auto valD = static_cast<BaseDefines::Directions>(ui->cbDirection->currentIndex());
    retval["direction"] = BaseDefines::DirectionValueUIDName.value(valD);

    return retval;
}

void StepDeviationParamsDialog::on_ok()
{
    accept();
}

void StepDeviationParamsDialog::on_cancel()
{
    reject();
}

void StepDeviationParamsDialog::on_default()
{
    auto mr = AMessageBox::question(nullptr, tr("Запрос"), tr("Сбросить настройки к настройкам методики по умолчанию?"));
    if (QMessageBox::Yes == mr)
    {
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        auto params = static_cast<AAnalyserApplication*>(QApplication::instance())->setMetodicParamsDefault(mi.uid);
        setParams(params);
    }
}
