#include "jumptestparamsdialog.h"
#include "ui_jumptestparamsdialog.h"

JumpTestParamsDialog::JumpTestParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JumpTestParamsDialog)
{
    ui->setupUi(this);
}

JumpTestParamsDialog::~JumpTestParamsDialog()
{
    delete ui;
}

void JumpTestParamsDialog::setParams(const QJsonObject &params)
{
    Q_UNUSED(params);
}

QJsonObject JumpTestParamsDialog::getParams()
{
    return QJsonObject();
}
