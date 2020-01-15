#include "jumpheighttestparamsdialog.h"
#include "ui_jumpheighttestparamsdialog.h"

#include "jumpheighttestdefines.h"

#include <QDebug>
#include <QFile>

JumpHeightTestParamsDialog::JumpHeightTestParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JumpHeightTestParamsDialog)
{
    ui->setupUi(this);
    initUi();
    ui->cbFinishKind->addItems(QStringList() << tr("После заданного количества прыжков") << tr("По истечению времени"));
    ui->cbStrategy->addItems(QStringList() << tr("Максимальная высота прыжка") << tr("Минимальное время на платформе"));
}

JumpHeightTestParamsDialog::~JumpHeightTestParamsDialog()
{
    delete ui;
}

void JumpHeightTestParamsDialog::setParams(const QJsonObject &params)
{
    auto sFK = params["finish_kind"].toString();
    ui->cbFinishKind->setCurrentIndex(JumpHeightTestDefines::TestFinishKindIndex.value(sFK));
    ui->edJumpsCount->setValue(params["jumps_count"].toInt());
    ui->edTestTime->setValue(params["time"].toInt());
    auto sStr = params["strategy"].toString();
    ui->cbStrategy->setCurrentIndex(JumpHeightTestDefines::StrategyIndex.value(sStr));
    ui->edContactTimeBound->setValue(params["contact_time_bound"].toInt());
}

QJsonObject JumpHeightTestParamsDialog::getParams()
{
    QJsonObject retval;
    auto valFK = static_cast<JumpHeightTestDefines::TestFinishKind>(ui->cbFinishKind->currentIndex());
    retval["finish_kind"] = JumpHeightTestDefines::TestFinishKindValueName.value(valFK);
    retval["jumps_count"] = ui->edJumpsCount->value();
    retval["time"] = ui->edTestTime->value();
    auto valStr = static_cast<JumpHeightTestDefines::Strategy>(ui->cbStrategy->currentIndex());
    retval["strategy"] = JumpHeightTestDefines::StrategyValueName.value(valStr);
    retval["contact_time_bound"] = ui->edContactTimeBound->value();

    return retval;
}

void JumpHeightTestParamsDialog::on_finishKindChanged(int idx)
{
    ui->frJumpsCount->setVisible(idx == 0);
    ui->frTestTime->setVisible(idx == 1);
}

void JumpHeightTestParamsDialog::on_strategyChanged(int idx)
{
    ui->frContactTimeBound->setVisible(idx == 1);
}

void JumpHeightTestParamsDialog::initUi()
{
    QFile style( ":/qss/another_win.qss" );
    style.open( QFile::ReadOnly );
    QString stlDetail(style.readAll() );
    setStyleSheet(stlDetail);
}
