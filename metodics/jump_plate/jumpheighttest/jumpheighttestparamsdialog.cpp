#include "jumpheighttestparamsdialog.h"
#include "ui_jumpheighttestparamsdialog.h"

#include "aanalyserapplication.h"
#include "jumpplatedefines.h"
#include "jumpheighttestdefines.h"

#include <QMessageBox>
#include <QDebug>
#include <QFile>

JumpHeightTestParamsDialog::JumpHeightTestParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JumpHeightTestParamsDialog)
{
    ui->setupUi(this);
    initUi();
    ui->cbFinishKind->addItems(QStringList() << tr("После заданного количества прыжков")
                                             << tr("По истечению времени")
                                             << tr("По команде оператора"));
    ui->cbStrategy->addItems(QStringList() << tr("Максимальная высота прыжка")
                                           << tr("Минимальное время на платформе"));
}

JumpHeightTestParamsDialog::~JumpHeightTestParamsDialog()
{
    delete ui;
}

void JumpHeightTestParamsDialog::setParams(const QJsonObject &params)
{
    auto sFK = params["finish_kind"].toString();
    ui->cbFinishKind->setCurrentIndex(JumpPlateDefines::TestFinishKindIndex.value(sFK));
    ui->edJumpsCount->setValue(params["jumps_count"].toInt());
    ui->edTestTime->setValue(params["time"].toInt());
    auto sStr = params["strategy"].toString();
    ui->cbStrategy->setCurrentIndex(JumpHeightTestDefines::StrategyIndex.value(sStr));
    ui->edContactTimeBound->setValue(params["contact_time_bound"].toInt());
}

QJsonObject JumpHeightTestParamsDialog::getParams()
{
    QJsonObject retval;
    auto valFK = static_cast<JumpPlateDefines::TestFinishKind>(ui->cbFinishKind->currentIndex());
    retval["finish_kind"] = JumpPlateDefines::TestFinishKindValueName.value(valFK);
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

void JumpHeightTestParamsDialog::on_ok()
{
    accept();
}

void JumpHeightTestParamsDialog::on_cancel()
{
    reject();
}

void JumpHeightTestParamsDialog::on_default()
{
    auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Сбросить настройки к настройкам методики по умолчанию?"));
    if (QMessageBox::Yes == mr)
    {
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        auto params = static_cast<AAnalyserApplication*>(QApplication::instance())->setMetodicParamsDefault(mi.uid);
        setParams(params);
    }
}

void JumpHeightTestParamsDialog::initUi()
{
    QFile style( ":/qss/another_win.qss" );
    style.open( QFile::ReadOnly );
    QString stlDetail(style.readAll() );
    setStyleSheet(stlDetail);
}
