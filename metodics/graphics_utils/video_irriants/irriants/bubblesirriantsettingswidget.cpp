#include "bubblesirriantsettingswidget.h"
#include "ui_bubblesirriantsettingswidget.h"

#include "bubblesirriant.h"

#include <QColorDialog>

BubblesIrriantSettingsWidget::BubblesIrriantSettingsWidget(BubblesIrriant* irritant, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BubblesIrriantSettingsWidget)
  , m_irritant(irritant)
{
    ui->setupUi(this);

    ui->edBubblesCount->setValue(m_irritant->bubblesCount());
    ui->edMaxSize->setValue(m_irritant->maxSize());
    setSampleColor(m_irritant->color());

    setStyleSheet("font-size: 9pt;");
    ui->edBubblesCount->setStyleSheet("font-size: 8pt;");
    ui->edMaxSize->setStyleSheet("font-size: 8pt;");
}

BubblesIrriantSettingsWidget::~BubblesIrriantSettingsWidget()
{
    delete ui;
}

void BubblesIrriantSettingsWidget::on_bubblesCountChanged(int value)
{
    m_irritant->setBubblesCount(value);
}

void BubblesIrriantSettingsWidget::on_MaxSizeChanged(int size)
{
    m_irritant->setMaxSize(size);
}

void BubblesIrriantSettingsWidget::on_btnSetColorClicked()
{
    auto palette = ui->frColorSample->palette();
    auto color = QColorDialog::getColor(palette.color(QPalette::Background));
    setSampleColor(color);
    m_irritant->setColor(color);
}

void BubblesIrriantSettingsWidget::setSampleColor(const QColor color)
{
    ui->frColorSample->setStyleSheet("background-color:rgb(" + QString::number(color.red()) + ", "
                                                             + QString::number(color.green()) + ", "
                                                             + QString::number(color.blue()) + ")");
}
