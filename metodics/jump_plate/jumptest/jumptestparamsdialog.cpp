#include "jumptestparamsdialog.h"
#include "ui_jumptestparamsdialog.h"

#include <QFile>

JumpTestParamsDialog::JumpTestParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JumpTestParamsDialog)
{
    ui->setupUi(this);
    initUi();
    ui->cbMethodic->addItems(QStringList() << tr("Анализ высоты прыжка") << tr("Анализ соскакивания"));
}

JumpTestParamsDialog::~JumpTestParamsDialog()
{
    delete ui;
}

void JumpTestParamsDialog::setParams(const QJsonObject &params)
{
    auto metCode = params["methodic"].toInt();
    ui->cbMethodic->setCurrentIndex(metCode);

}

QJsonObject JumpTestParamsDialog::getParams()
{
    QJsonObject retval;
    retval["methodic"] = ui->cbMethodic->currentIndex();
    return retval;
}

void JumpTestParamsDialog::initUi()
{
    QFile style( ":/qss/another_win.qss" );
    style.open( QFile::ReadOnly );
    QString stlDetail(style.readAll() );
    setStyleSheet(stlDetail);
}
