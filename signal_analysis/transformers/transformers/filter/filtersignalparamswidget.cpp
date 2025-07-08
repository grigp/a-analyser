#include "filtersignalparamswidget.h"
#include "ui_filtersignalparamswidget.h"

#include "baseutils.h"
#include "filtersignal.h"

FilterSignalParamsWidget::FilterSignalParamsWidget(QWidget *parent) :
    SignalTransformerParamsWidget(parent),
    ui(new Ui::FilterSignalParamsWidget)
{
    ui->setupUi(this);

    foreach (auto fk, FilterSignal::filterName().keys())
        ui->cbKind->addItem(FilterSignal::filterName().value(fk), fk);

    foreach (auto fd, FilterSignal::filterDirectionName().keys())
        ui->cbDirection->addItem(FilterSignal::filterDirectionName().value(fd), fd);
}

FilterSignalParamsWidget::~FilterSignalParamsWidget()
{
    delete ui;
}

void FilterSignalParamsWidget::setParams(const QJsonObject &params)
{
    ui->edFreq->setValue(params["freq_cutoff"].toDouble(1.0));

    int idx = 0;
    int kind = params["kind"].toInt();
    for (int i = 0; i < ui->cbKind->count(); ++i)
    {
        if (ui->cbKind->itemData(i).toInt() == kind)
        {
            idx = i;
            break;
        }
    }
    ui->cbKind->setCurrentIndex(idx);

    idx = 0;
    int dir = params["direction"].toInt();
    for (int i = 0; i < ui->cbDirection->count(); ++i)
    {
        if (ui->cbDirection->itemData(i).toInt() == dir)
        {
            idx = i;
            break;
        }
    }
    ui->cbDirection->setCurrentIndex(idx);
}

QJsonObject FilterSignalParamsWidget::params() const
{
    QJsonObject retval;
    retval["freq_cutoff"] = ui->edFreq->value();
    retval["kind"] = ui->cbKind->currentData().toInt();
    retval["direction"] = ui->cbDirection->currentData().toInt();
    return retval;
}
