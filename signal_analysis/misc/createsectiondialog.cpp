#include "createsectiondialog.h"
#include "ui_createsectiondialog.h"

#include "amessagebox.h"
#include "signalaccess.h"

CreateSectionDialog::CreateSectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateSectionDialog)
{
    ui->setupUi(this);
}

CreateSectionDialog::~CreateSectionDialog()
{
    delete ui;
}

QString CreateSectionDialog::sectionName() const
{
    return ui->edSectionName->text();
}

void CreateSectionDialog::setSectionName(const QString name)
{
    ui->edSectionName->setText(name);
}

void CreateSectionDialog::assignSignal(SignalAccess *signal, const int def)
{
    ui->frChannel->setVisible(signal->subChansCount() > 1);
    for (int i = 0; i < signal->subChansCount(); ++i)
    {
        ui->cbChannel->addItem(signal->subChanName(i));
    }
    if (def >= 0 && def < ui->cbChannel->count())
        ui->cbChannel->setCurrentIndex(def);
}

int CreateSectionDialog::channel() const
{
    return ui->cbChannel->currentIndex();
}

void CreateSectionDialog::accept()
{
    if (ui->edSectionName->text() != "")
        QDialog::accept();
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Должно быть введено название секции"));
}
