#include "droptestparamsdialog.h"
#include "ui_droptestparamsdialog.h"

#include "jumpplatedefines.h"

#include <QFile>

DropTestParamsDialog::DropTestParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DropTestParamsDialog)
{
    ui->setupUi(this);
    initUi();
    ui->cbFinishKind->addItems(QStringList() << tr("После заданного количества прыжков") << tr("По истечению времени"));
}

DropTestParamsDialog::~DropTestParamsDialog()
{
    delete ui;
}

void DropTestParamsDialog::setParams(const QJsonObject &params)
{
    auto sFK = params["finish_kind"].toString();
    ui->cbFinishKind->setCurrentIndex(JumpPlateDefines::TestFinishKindIndex.value(sFK));
    ui->edJumpsCount->setValue(params["jumps_count"].toInt());
    ui->edTestTime->setValue(params["time"].toInt());
    ui->edFallHeight->setValue(params["fall_height"].toInt());
}

QJsonObject DropTestParamsDialog::getParams()
{
    QJsonObject retval;
    auto valFK = static_cast<JumpPlateDefines::TestFinishKind>(ui->cbFinishKind->currentIndex());
    retval["finish_kind"] = JumpPlateDefines::TestFinishKindValueName.value(valFK);
    retval["jumps_count"] = ui->edJumpsCount->value();
    retval["time"] = ui->edTestTime->value();
    retval["fall_height"] = ui->edFallHeight->value();
    return retval;
}

void DropTestParamsDialog::on_finishKindChanged(int idx)
{
    ui->frJumpsCount->setVisible(idx == 0);
    ui->frTestTime->setVisible(idx == 1);
}

void DropTestParamsDialog::initUi()
{
    QFile style( ":/qss/another_win.qss" );
    style.open( QFile::ReadOnly );
    QString stlDetail(style.readAll() );
    setStyleSheet(stlDetail);
}