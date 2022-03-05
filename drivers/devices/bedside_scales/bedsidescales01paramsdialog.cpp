#include "bedsidescales01paramsdialog.h"
#include "ui_bedsidescales01paramsdialog.h"

BedsideScales01ParamsDialog::BedsideScales01ParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BedsideScales01ParamsDialog)
{
    ui->setupUi(this);
}

BedsideScales01ParamsDialog::~BedsideScales01ParamsDialog()
{
    delete ui;
}
