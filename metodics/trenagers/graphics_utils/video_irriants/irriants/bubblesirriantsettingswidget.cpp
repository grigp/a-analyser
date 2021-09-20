#include "bubblesirriantsettingswidget.h"
#include "ui_bubblesirriantsettingswidget.h"

#include "bubblesirriant.h"

BubblesIrriantSettingsWidget::BubblesIrriantSettingsWidget(BubblesIrriant* irritant, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BubblesIrriantSettingsWidget)
  , m_irritant(irritant)
{
    ui->setupUi(this);

    setStyleSheet("font-size: 9pt;");
    ui->edBubblesCount->setStyleSheet("font-size: 8pt;");
}

BubblesIrriantSettingsWidget::~BubblesIrriantSettingsWidget()
{
    delete ui;
}

void BubblesIrriantSettingsWidget::on_bubblesCountChanged(int value)
{
    m_irritant->setBubblesCount(value);
}
