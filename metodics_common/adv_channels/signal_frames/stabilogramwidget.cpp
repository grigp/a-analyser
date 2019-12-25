#include "stabilogramwidget.h"
#include "ui_stabilogramwidget.h"

StabilogramWidget::StabilogramWidget(const QString channelId, QWidget *parent) :
    SignalWidget(channelId, parent),
    ui(new Ui::StabilogramWidget)
{
    ui->setupUi(this);
}

StabilogramWidget::~StabilogramWidget()
{
    delete ui;
}
