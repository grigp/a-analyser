#include "patientkarddialog.h"
#include "ui_patientkarddialog.h"

#include <QComboBox>

PatientKardDialog::PatientKardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientKardDialog)
{
    ui->setupUi(this);

    ui->cbSex->addItem(DataDefines::SexToText.value(DataDefines::male));
    ui->cbSex->addItem(DataDefines::SexToText.value(DataDefines::female));
}

PatientKardDialog::~PatientKardDialog()
{
    delete ui;
}

QString PatientKardDialog::fio() const
{
    return ui->edFIO->text();
}

void PatientKardDialog::setFio(const QString &fio)
{
    ui->edFIO->setText(fio);
}

QDate PatientKardDialog::born() const
{
    return ui->edBorn->date();
}

void PatientKardDialog::setBorn(const QDate &born)
{
    ui->edBorn->setDate(born);
}

DataDefines::Sex PatientKardDialog::sex() const
{
    return static_cast<DataDefines::Sex>(ui->cbSex->currentIndex());
}

void PatientKardDialog::setSex(const DataDefines::Sex &sex)
{
    ui->cbSex->setCurrentIndex(static_cast<int>(sex));
}

int PatientKardDialog::massa() const
{
    return ui->edWeight->value();
}

void PatientKardDialog::setMassa(const int w)
{
    ui->edWeight->setValue(w);
}

int PatientKardDialog::height() const
{
    return ui->edHeight->value();
}

void PatientKardDialog::setHeight(const int h)
{
    ui->edHeight->setValue(h);
}

void PatientKardDialog::setPersonalProgram(const QString &ppName, QPixmap& pic)
{
    ui->lblPP->setText(ppName);
    ui->lblPPPic->setPixmap(pic);
}
