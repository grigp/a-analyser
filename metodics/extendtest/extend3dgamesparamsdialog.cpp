#include "extend3dgamesparamsdialog.h"
#include "ui_extend3dgamesparamsdialog.h"

#include "aanalyserapplication.h"

#include <QMessageBox>

Extend3DGamesParamsDialog::Extend3DGamesParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Extend3DGamesParamsDialog)
{
    ui->setupUi(this);
}

Extend3DGamesParamsDialog::~Extend3DGamesParamsDialog()
{
    delete ui;
}

void Extend3DGamesParamsDialog::setParams(const QJsonObject &params)
{
    Q_UNUSED(params);
}

QJsonObject Extend3DGamesParamsDialog::getParams()
{
    return  QJsonObject();
}

void Extend3DGamesParamsDialog::on_ok()
{
    accept();
}

void Extend3DGamesParamsDialog::on_cancel()
{
    reject();
}

void Extend3DGamesParamsDialog::on_default()
{
    auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Сбросить настройки к настройкам методики по умолчанию?"));
    if (QMessageBox::Yes == mr)
    {
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        auto params = static_cast<AAnalyserApplication*>(QApplication::instance())->setMetodicParamsDefault(mi.uid);
        setParams(params);
    }
}
