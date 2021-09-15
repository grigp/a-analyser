#include "linesirriantsettingswidget.h"
#include "ui_linesirriantsettingswidget.h"

LinesIrriantSettingsWidget::LinesIrriantSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinesIrriantSettingsWidget)
{
    ui->setupUi(this);
}

LinesIrriantSettingsWidget::~LinesIrriantSettingsWidget()
{
    delete ui;
}
