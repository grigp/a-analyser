#include "linesirriantsettingswidget.h"
#include "ui_linesirriantsettingswidget.h"

#include "linesirriant.h"

#include <QColorDialog>
#include <QDebug>

LinesIrriantSettingsWidget::LinesIrriantSettingsWidget(LinesIrriant* iritant, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinesIrriantSettingsWidget)
  , m_irritant(iritant)
{
    ui->setupUi(this);
}

LinesIrriantSettingsWidget::~LinesIrriantSettingsWidget()
{
    delete ui;
}

void LinesIrriantSettingsWidget::on_cbDirectionChanged(int idx)
{
    m_irritant->setDirection(static_cast<BaseUtils::Directions>(idx + 1));
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
