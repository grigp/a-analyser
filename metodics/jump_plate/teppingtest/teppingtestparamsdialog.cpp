#include "teppingtestparamsdialog.h"
#include "ui_teppingtestparamsdialog.h"

#include "jumpplatedefines.h"

#include <QFile>

TeppingTestParamsDialog::TeppingTestParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeppingTestParamsDialog)
{
    ui->setupUi(this);
    initUi();
    ui->cbFinishKind->addItems(QStringList() << tr("После заданного количества шагов") << tr("По истечению времени"));
}

TeppingTestParamsDialog::~TeppingTestParamsDialog()
{
    delete ui;
}

void TeppingTestParamsDialog::setParams(const QJsonObject &params)
{
    auto sFK = params["finish_kind"].toString();
    ui->cbFinishKind->setCurrentIndex(JumpPlateDefines::TestFinishKindIndex.value(sFK));
    ui->edStepsCount->setValue(params["steps_count"].toInt());
    ui->edTestTime->setValue(params["time"].toInt());
}

QJsonObject TeppingTestParamsDialog::getParams()
{
    QJsonObject retval;
    auto valFK = static_cast<JumpPlateDefines::TestFinishKind>(ui->cbFinishKind->currentIndex());
    retval["finish_kind"] = JumpPlateDefines::TestFinishKindValueName.value(valFK);
    retval["steps_count"] = ui->edStepsCount->value();
    retval["time"] = ui->edTestTime->value();

    return retval;
}

void TeppingTestParamsDialog::on_finishKindChanged(int idx)
{
    ui->frStepsCount->setVisible(idx == 0);
    ui->frTestTime->setVisible(idx == 1);
}

void TeppingTestParamsDialog::initUi()
{
    QFile style( ":/qss/another_win.qss" );
    style.open( QFile::ReadOnly );
    QString stlDetail(style.readAll() );
    setStyleSheet(stlDetail);
}