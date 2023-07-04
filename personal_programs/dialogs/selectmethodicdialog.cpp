#include "selectmethodicdialog.h"
#include "ui_selectmethodicdialog.h"

#include <QUuid>

SelectMethodicDialog::SelectMethodicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectMethodicDialog)
{
    ui->setupUi(this);
    ui->wgtMethods->onDbConnect(false);
    auto btnOK = ui->buttonBox->button(QDialogButtonBox::Ok);
    btnOK->setEnabled(false);
    connect(ui->wgtMethods, &MethodsWidget::selectMethod, this, &SelectMethodicDialog::on_selectMethod);
}

SelectMethodicDialog::~SelectMethodicDialog()
{
    delete ui;
}

QString SelectMethodicDialog::methodic() const
{
    return ui->wgtMethods->methodic();
}

void SelectMethodicDialog::on_selectMethod(const QString &uid)
{
    auto btnOK = ui->buttonBox->button(QDialogButtonBox::Ok);
    btnOK->setEnabled((uid != "" && uid != QUuid().toString()));
}

