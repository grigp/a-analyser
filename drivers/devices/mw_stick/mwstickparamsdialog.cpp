#include "mwstickparamsdialog.h"
#include "ui_mwstickparamsdialog.h"

MWStickParamsDialog::MWStickParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MWStickParamsDialog)
{
    ui->setupUi(this);

    ui->cbChan->addItems(QStringList() << tr("Канал 1") << tr("Канал 2") << tr("Канал 3"));
}

MWStickParamsDialog::~MWStickParamsDialog()
{
    delete ui;
}

int MWStickParamsDialog::channel() const
{
    return ui->cbChan->currentIndex();
}

void MWStickParamsDialog::setChannel(const int chan)
{
    ui->cbChan->setCurrentIndex(chan);
}

double MWStickParamsDialog::rkp() const
{
    return ui->edRKP->value();
}

void MWStickParamsDialog::setRkp(const double rkp)
{
    ui->edRKP->setValue(rkp);
}

int MWStickParamsDialog::pn() const
{
    return ui->edPN->value();
}

void MWStickParamsDialog::setPn(const int pn)
{
    ui->edPN->setValue(pn);
}
