#include "trentakeputparamsdialog.h"
#include "ui_trentakeputparamsdialog.h"

#include <QFile>

TrenTakePutParamsDialog::TrenTakePutParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrenTakePutParamsDialog)
{
    ui->setupUi(this);
    initUi();

    ui->tabWidget->setCurrentIndex(0);
    ui->cbScale->addItems(QStringList() << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128");
}

TrenTakePutParamsDialog::~TrenTakePutParamsDialog()
{
    delete ui;
}

void TrenTakePutParamsDialog::setParams(const QJsonObject &params)
{
    m_params = params;

    ui->edGameTime->setValue(params["time"].toInt());
    ui->cbScale->setCurrentIndex(params["scale"].toInt());

    auto objPhisioChan = params["phisio_chan"].toObject();
    ui->cbUseAdvancedChannel->setChecked(objPhisioChan["enabled"].toBool());
    ui->edForce->setValue(objPhisioChan["force"].toInt(10));
    ui->edMyogram->setValue(objPhisioChan["myogram"].toInt(200));
    onPhisioChanEnabledChange(objPhisioChan["enabled"].toBool());
}

QJsonObject TrenTakePutParamsDialog::getParams()
{
    m_params["time"] = ui->edGameTime->value();
    m_params["scale"] = ui->cbScale->currentIndex();

    QJsonObject objPhisioChan;
    objPhisioChan["enabled"] = ui->cbUseAdvancedChannel->isChecked();
    objPhisioChan["force"] = ui->edForce->value();
    objPhisioChan["myogram"] = ui->edMyogram->value();
    m_params["phisio_chan"] = objPhisioChan;

    return m_params;
}

void TrenTakePutParamsDialog::onPhisioChanEnabledChange(bool enabled)
{
    ui->frForce->setEnabled(enabled);
    ui->frMyogram->setEnabled(enabled);
}

void TrenTakePutParamsDialog::initUi()
{
    QFile style( ":/qss/another_win.qss" );
    style.open( QFile::ReadOnly );
    QString stlDetail(style.readAll() );
    setStyleSheet(stlDetail);
}
