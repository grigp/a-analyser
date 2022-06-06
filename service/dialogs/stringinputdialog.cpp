#include "stringinputdialog.h"
#include "ui_stringinputdialog.h"

StringInputDialog::StringInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StringInputDialog)
{
    ui->setupUi(this);
}

StringInputDialog::~StringInputDialog()
{
    delete ui;
}

void StringInputDialog::setTitle(const QString &title)
{
    setWindowTitle(title);
}

void StringInputDialog::setName(const QString &title)
{
    ui->lblText->setText(title);
}

QString StringInputDialog::text() const
{
    return ui->edText->text();
}

void StringInputDialog::setText(const QString &text)
{
    ui->edText->setText(text);
}
