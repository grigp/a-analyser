#include "selectmethodicdialog.h"
#include "ui_selectmethodicdialog.h"

SelectMethodicDialog::SelectMethodicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectMethodicDialog)
{
    ui->setupUi(this);
    ui->wgtMethods->onDbConnect(false);
}

SelectMethodicDialog::~SelectMethodicDialog()
{
    delete ui;
}

QString SelectMethodicDialog::methodic() const
{
    return ui->wgtMethods->methodic();
}
