#include "teppingtestparamsdialog.h"
#include "ui_teppingtestparamsdialog.h"

#include "aanalyserapplication.h"
#include "jumpplatedefines.h"
#include "amessagebox.h"

#include <QFile>

TeppingTestParamsDialog::TeppingTestParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeppingTestParamsDialog)
{
    ui->setupUi(this);
    initUi();
    ui->cbFinishKind->addItems(QStringList() << tr("После заданного количества шагов")
                                             << tr("По истечению времени")
                                             << tr("По команде оператора"));
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

void TeppingTestParamsDialog::on_ok()
{
    accept();
}

void TeppingTestParamsDialog::on_cancel()
{
    reject();
}

void TeppingTestParamsDialog::on_default()
{
    auto mr = AMessageBox::question(nullptr, tr("Запрос"), tr("Сбросить настройки к настройкам методики по умолчанию?"));
    if (QMessageBox::Yes == mr)
    {
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        auto params = static_cast<AAnalyserApplication*>(QApplication::instance())->setMetodicParamsDefault(mi.uid);
        setParams(params);
    }
}

void TeppingTestParamsDialog::initUi()
{
    QFile style( ":/qss/another_win.qss" );
    style.open( QFile::ReadOnly );
    QString stlDetail(style.readAll() );
    setStyleSheet(stlDetail);
}
