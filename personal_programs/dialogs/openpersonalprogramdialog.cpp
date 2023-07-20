#include "openpersonalprogramdialog.h"
#include "ui_openpersonalprogramdialog.h"

#include "aanalyserapplication.h"

OpenPersonalProgramDialog::OpenPersonalProgramDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenPersonalProgramDialog)
{
    ui->setupUi(this);
    setStyleSheet(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow()->styleSheet());
}

OpenPersonalProgramDialog::~OpenPersonalProgramDialog()
{
    delete ui;
}

void OpenPersonalProgramDialog::setPatientFio(const QString &fio)
{
    ui->lblFio->setText(fio);
}

void OpenPersonalProgramDialog::setPersonalProgram(const QString &name, const QPixmap &pic)
{
    ui->lblPPName->setText(name);
    ui->lblPPImage->setPixmap(pic);
}
