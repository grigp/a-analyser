#include "trentakeputparamsdialog.h"
#include "ui_trentakeputparamsdialog.h"

#include <QFile>

TrenTakePutParamsDialog::TrenTakePutParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrenTakePutParamsDialog)
{
    ui->setupUi(this);
    initUi();
}

TrenTakePutParamsDialog::~TrenTakePutParamsDialog()
{
    delete ui;
}

void TrenTakePutParamsDialog::setParams(const QJsonObject &params)
{
    Q_UNUSED(params);
}

QJsonObject TrenTakePutParamsDialog::getParams()
{
    return QJsonObject();
}

void TrenTakePutParamsDialog::initUi()
{
    QFile style( ":/qss/another_win.qss" );
    style.open( QFile::ReadOnly );
    QString stlDetail(style.readAll() );
    setStyleSheet(stlDetail);
}
