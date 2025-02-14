#include "trentetrisparamsdialog.h"
#include "ui_trentetrisparamsdialog.h"

#include "aanalyserapplication.h"
#include "trentetrisdefines.h"
#include "amessagebox.h"

#include <QDebug>

TrenTetrisParamsDialog::TrenTetrisParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrenTetrisParamsDialog)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);
    ui->cbScale->addItems(QStringList() << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128");
    ui->cbMovingMode->addItems(QStringList() << tr("Захват и спуск") << tr("Автоматический"));
    ui->cbComplexityMode->addItems(QStringList() << tr("Фигуры") << tr("Кубики"));
    ui->cbDeleteMode->addItems(QStringList() << tr("Заполненные строки") << tr("Совпадающие цвета"));
}

TrenTetrisParamsDialog::~TrenTetrisParamsDialog()
{
    delete ui;
}

void TrenTetrisParamsDialog::setParams(const QJsonObject &params)
{
    m_params = params;

    auto tm = params["time"].toInt();
    ui->edGameTime->setTime(QTime(0, tm / 60, tm % 60));
    ui->cbScale->setCurrentIndex(params["scale"].toInt());

    auto mm = TrenTetrisDefines::MovingModeValueIndex.value(params["moving_mode"].toString());
    ui->cbMovingMode->setCurrentIndex(mm);
    auto cm = TrenTetrisDefines::ComplexityModeValueIndex.value(params["complexity"].toString());
    ui->cbComplexityMode->setCurrentIndex(cm);
    auto dm = TrenTetrisDefines::DeletingModeValueIndex.value(params["deleting"].toString());
    ui->cbDeleteMode->setCurrentIndex(dm);
    ui->cbIsShowGrid->setChecked(params["is_show_grid"].toBool(false));

    auto objPhisioChan = params["phisio_chan"].toObject();
    ui->cbUseAdvancedChannel->setChecked(objPhisioChan["enabled"].toBool());
    ui->edForce->setValue(objPhisioChan["force"].toInt(10));
    ui->edMyogram->setValue(objPhisioChan["myogram"].toInt(200));
    onPhisioChanEnabledChange(objPhisioChan["enabled"].toBool());
}

QJsonObject TrenTetrisParamsDialog::getParams()
{
    auto tm = ui->edGameTime->time();
    m_params["time"] = tm.minute() * 60 + tm.second();
    m_params["scale"] = ui->cbScale->currentIndex();

    auto mm = TrenTetrisDefines::MovingModeValueName.value(static_cast<TrenTetrisDefines::MovingMode>(ui->cbMovingMode->currentIndex()));
    m_params["moving_mode"] = mm;
    auto cm = TrenTetrisDefines::ComplexityValueName.value(static_cast<TrenTetrisDefines::ComplexityMode>(ui->cbComplexityMode->currentIndex()));
    m_params["complexity"] = cm;
    auto dm = TrenTetrisDefines::DeletingValueName.value(static_cast<TrenTetrisDefines::DeletingMode>(ui->cbDeleteMode->currentIndex()));
    m_params["deleting"] = dm;
    m_params["is_show_grid"] = ui->cbIsShowGrid->isChecked();

    QJsonObject objPhisioChan;
    objPhisioChan["enabled"] = ui->cbUseAdvancedChannel->isChecked();
    objPhisioChan["force"] = ui->edForce->value();
    objPhisioChan["myogram"] = ui->edMyogram->value();
    m_params["phisio_chan"] = objPhisioChan;

    return m_params;
}

void TrenTetrisParamsDialog::onPhisioChanEnabledChange(bool enabled)
{
    Q_UNUSED(enabled);
//    ui->frForce->setEnabled(enabled);
    //    ui->frMyogram->setEnabled(enabled);
}

void TrenTetrisParamsDialog::on_ok()
{
    accept();
}

void TrenTetrisParamsDialog::on_cancel()
{
    reject();
}

void TrenTetrisParamsDialog::on_default()
{
    auto mr = AMessageBox::question(nullptr, tr("Запрос"), tr("Сбросить настройки к настройкам методики по умолчанию?"));
    if (AMessageBox::Yes == mr)
    {
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        auto params = static_cast<AAnalyserApplication*>(QApplication::instance())->setMetodicParamsDefault(mi.uid);
        setParams(params);
    }
}
