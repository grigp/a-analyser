#include "jumptestparamsdialog.h"
#include "ui_jumptestparamsdialog.h"

#include "aanalyserapplication.h"

#include <QFile>
#include <QMessageBox>

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

void JumpTestParamsDialog::on_ok()
{
    accept();
}

void JumpTestParamsDialog::on_cancel()
{
    reject();
}

void JumpTestParamsDialog::on_default()
{
    auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Сбросить настройки к настройкам методики по умолчанию?"));
    if (QMessageBox::Yes == mr)
    {
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        auto params = static_cast<AAnalyserApplication*>(QApplication::instance())->setMetodicParamsDefault(mi.uid);
        setParams(params);
    }
}

void JumpTestParamsDialog::initUi()
{
    QFile style( ":/qss/another_win.qss" );
    style.open( QFile::ReadOnly );
    QString stlDetail(style.readAll() );
    setStyleSheet(stlDetail);
}
