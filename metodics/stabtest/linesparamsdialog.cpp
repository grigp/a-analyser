#include "linesparamsdialog.h"
#include "ui_linesparamsdialog.h"

#include <QColorDialog>


LinesParamsDialog::LinesParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LinesParamsDialog)
{
    ui->setupUi(this);

    ui->cbDirection->clear();
    foreach (auto key, BaseDefines::DirectionValueName.keys())
        ui->cbDirection->addItem(BaseDefines::DirectionValueName.value(key), key);
}

LinesParamsDialog::~LinesParamsDialog()
{
    delete ui;
}

BaseDefines::Directions LinesParamsDialog::direction() const
{
    return static_cast<BaseDefines::Directions>(ui->cbDirection->currentIndex());
}

void LinesParamsDialog::setDirection(const BaseDefines::Directions dir)
{
    ui->cbDirection->setCurrentIndex(dir);
}

int LinesParamsDialog::width() const
{
    return ui->edWidth->value();
}

void LinesParamsDialog::setWidth(const int w)
{
    ui->edWidth->setValue(w);
}

int LinesParamsDialog::speed() const
{
    return ui->edSpeed->value();
}

void LinesParamsDialog::setSpeed(const int spd)
{
    ui->edSpeed->setValue(spd);
}

int LinesParamsDialog::dutyCycle() const
{
    return ui->edDutyCicle->value();
}

void LinesParamsDialog::setDutyCycle(const int dc)
{
    ui->edDutyCicle->setValue(dc);
}

QColor LinesParamsDialog::color() const
{
    auto palette = ui->frColorSample->palette();
    return palette.color(QPalette::Background);
}

void LinesParamsDialog::setColor(const QColor col)
{
    ui->frColorSample->setStyleSheet("background-color:rgb(" + QString::number(col.red()) + ", "
                                                             + QString::number(col.green()) + ", "
                                                             + QString::number(col.blue()) + ")");
}

void LinesParamsDialog::on_selectColor()
{
    auto palette = ui->frColorSample->palette();
    auto col = QColorDialog::getColor(palette.color(QPalette::Background));
    setColor(col);
}

//auto palette = ui->frColorSample->palette();
//auto col = QColorDialog::getColor(palette.color(QPalette::Background));
//ui->frColorSample->setStyleSheet("background-color:rgb(" + QString::number(color.red()) + ", "
//                                                         + QString::number(color.green()) + ", "
//                                                         + QString::number(color.blue()) + ")");
