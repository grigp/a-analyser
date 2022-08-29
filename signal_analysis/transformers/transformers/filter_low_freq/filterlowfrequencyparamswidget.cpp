#include "filterlowfrequencyparamswidget.h"
#include "ui_filterlowfrequencyparamswidget.h"

FilterLowFrequencyParamsWidget::FilterLowFrequencyParamsWidget(QWidget *parent) :
    SignalTransformerParamsWidget(parent),
    ui(new Ui::FilterLowFrequencyParamsWidget)
{
    ui->setupUi(this);
}

FilterLowFrequencyParamsWidget::~FilterLowFrequencyParamsWidget()
{
    delete ui;
}

void FilterLowFrequencyParamsWidget::setParams(const QJsonObject &params)
{
    ui->edFreq->setValue(params["freq"].toDouble(1.0));
}

QJsonObject FilterLowFrequencyParamsWidget::params() const
{
    QJsonObject retval;
    retval["freq"] = ui->edFreq->value();
    return retval;
}
