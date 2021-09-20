#include "linesirriantsettingswidget.h"
#include "ui_linesirriantsettingswidget.h"

#include "linesirriant.h"

#include <QColorDialog>
#include <QDebug>

LinesIrriantSettingsWidget::LinesIrriantSettingsWidget(LinesIrriant* irritant, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinesIrriantSettingsWidget)
  , m_irritant(irritant)
{
    ui->setupUi(this);

    ui->cbDirection->clear();
    foreach (auto key, BaseUtils::DirectionValueName.keys())
        ui->cbDirection->addItem(BaseUtils::DirectionValueName.value(key), key);
    ui->cbDirection->setCurrentIndex(m_irritant->direction());
    ui->edSpeed->setValue(m_irritant->speed());
    ui->edWidth->setValue(m_irritant->width());
    ui->edDutyCicle->setValue(m_irritant->dutyCycle());

    setStyleSheet("font-size: 9pt;");
    ui->edSpeed->setStyleSheet("font-size: 8pt;");
    ui->edWidth->setStyleSheet("font-size: 8pt;");
    ui->edDutyCicle->setStyleSheet("font-size: 8pt;");

}

LinesIrriantSettingsWidget::~LinesIrriantSettingsWidget()
{
    delete ui;
}

void LinesIrriantSettingsWidget::on_cbDirectionChanged(int idx)
{
    Q_UNUSED(idx);

    if (ui->cbDirection->count() == BaseUtils::DirectionValueName.size())
    {
        BaseUtils::Directions dir = static_cast<BaseUtils::Directions>(ui->cbDirection->currentData().toInt());
        m_irritant->setDirection(dir);
    }
}

void LinesIrriantSettingsWidget::on_widthChanged(int value)
{
    m_irritant->setWidth(value);
}

void LinesIrriantSettingsWidget::on_speedChanged(int speed)
{
    m_irritant->setSpeed(speed);
}

void LinesIrriantSettingsWidget::on_dutyCicleChanged(int dc)
{
    m_irritant->setDutyCycle(dc);
}

void LinesIrriantSettingsWidget::on_btnSetColorClicked()
{
    auto palette = ui->frColorSample->palette();
    auto color = QColorDialog::getColor(palette.color(QPalette::Background));

    ui->frColorSample->setStyleSheet("background-color:rgb(" + QString::number(color.red()) + ", "
                                                             + QString::number(color.green()) + ", "
                                                             + QString::number(color.blue()) + ")");
    m_irritant->setColor(color);
}
