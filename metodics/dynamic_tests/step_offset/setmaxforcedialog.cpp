#include "setmaxforcedialog.h"
#include "ui_setmaxforcedialog.h"

SetMaxForceDialog::SetMaxForceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetMaxForceDialog)
{
    ui->setupUi(this);
}

SetMaxForceDialog::~SetMaxForceDialog()
{
    delete ui;
}

void SetMaxForceDialog::getData(const double x, const double y)
{
    QString sx = QString::number(x);
    QString sy = QString::number(y);

    ui->label->setText("x = " + sx + "      y = " + sy);
}
