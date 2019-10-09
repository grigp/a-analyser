#include "testpropertydialog.h"
#include "ui_testpropertydialog.h"

TestPropertyDialog::TestPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestPropertyDialog)
{
    ui->setupUi(this);
}

TestPropertyDialog::~TestPropertyDialog()
{
    delete ui;
}

QString TestPropertyDialog::comment()
{
    return ui->edComment->toPlainText();
}

void TestPropertyDialog::setComment(const QString &comment)
{
    ui->edComment->setText(comment);
}

QString TestPropertyDialog::condition()
{
    return QString("");
}

void TestPropertyDialog::setCondition(const QString &condition)
{

}

bool TestPropertyDialog::normContained()
{
    return ui->cbInNormContained->isChecked();
}

void TestPropertyDialog::setNormContained(const bool nc)
{
    ui->cbInNormContained->setChecked(nc);
}
