#include "trentetrisparamsdialog.h"
#include "ui_trentetrisparamsdialog.h"

#include "trentetrisdefines.h"

#include <QDebug>

TrenTetrisParamsDialog::TrenTetrisParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrenTetrisParamsDialog)
{
    ui->setupUi(this);

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

    ui->edGameTime->setValue(params["time"].toInt());
    ui->cbScale->setCurrentIndex(params["scale"].toInt());

    auto mm = TrenTetrisDefines::MovingModeValueIndex.value(params["moving_mode"].toString());
    ui->cbMovingMode->setCurrentIndex(mm);
    auto cm = TrenTetrisDefines::ComplexityModeValueIndex.value(params["complexity"].toString());
    ui->cbComplexityMode->setCurrentIndex(cm);
    auto dm = TrenTetrisDefines::DeletingModeValueIndex.value(params["deleting"].toString());
    ui->cbDeleteMode->setCurrentIndex(dm);
}

QJsonObject TrenTetrisParamsDialog::getParams()
{
    m_params["time"] = ui->edGameTime->value();
    m_params["scale"] = ui->cbScale->currentIndex();

    auto mm = TrenTetrisDefines::MovingModeValueName.value(static_cast<TrenTetrisDefines::MovingMode>(ui->cbMovingMode->currentIndex()));
    m_params["moving_mode"] = mm;
    auto cm = TrenTetrisDefines::ComplexityValueName.value(static_cast<TrenTetrisDefines::ComplexityMode>(ui->cbComplexityMode->currentIndex()));
    m_params["complexity"] = cm;
    auto dm = TrenTetrisDefines::DeletingValueName.value(static_cast<TrenTetrisDefines::DeletingMode>(ui->cbDeleteMode->currentIndex()));
    m_params["deleting"] = dm;

    return m_params;
}
