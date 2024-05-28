#include "mwcompetitivestickparamsdialog.h"
#include "ui_mwcompetitivestickparamsdialog.h"

#include <QJsonObject>
#include <QTime>

#include "aanalyserapplication.h"
#include "amessagebox.h"

MWCompetitiveStickParamsDialog::MWCompetitiveStickParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MWCompetitiveStickParamsDialog)
{
    ui->setupUi(this);
}

MWCompetitiveStickParamsDialog::~MWCompetitiveStickParamsDialog()
{
    delete ui;
}

void MWCompetitiveStickParamsDialog::setParams(const QJsonObject &params)
{
    ui->cbAutoEnd->setChecked(params["autoend"].toBool(true));
    ui->edTime->setTime(QTime::fromString(params["time"].toString("00:00"), "mm:ss"));
}

QJsonObject MWCompetitiveStickParamsDialog::getParams()
{
    QJsonObject retval;
    retval["autoend"] = ui->cbAutoEnd->isChecked();
    retval["time"] = ui->edTime->time().toString("mm:ss");
    return retval;
}

void MWCompetitiveStickParamsDialog::on_default()
{
    auto mr = AMessageBox::question(nullptr, tr("Запрос"), tr("Сбросить настройки к настройкам методики по умолчанию?"));
    if (AMessageBox::Yes == mr)
    {
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        auto params = static_cast<AAnalyserApplication*>(QApplication::instance())->setMetodicParamsDefault(mi.uid);
        setParams(params);
    }
}
