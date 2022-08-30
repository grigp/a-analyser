#include "filterhighfrequencyparamswidget.h"
#include "ui_filterhighfrequencyparamswidget.h"

#include "baseutils.h"

FilterHighFrequencyParamsWidget::FilterHighFrequencyParamsWidget(QWidget *parent) :
    SignalTransformerParamsWidget(parent),
    ui(new Ui::FilterHighFrequencyParamsWidget)
{
    ui->setupUi(this);

    foreach (auto fk, BaseUtils::FilterName.keys())
        ui->cbKind->addItem(BaseUtils::FilterName.value(fk), fk);
}

FilterHighFrequencyParamsWidget::~FilterHighFrequencyParamsWidget()
{
    delete ui;
}

void FilterHighFrequencyParamsWidget::setParams(const QJsonObject &params)
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
}

QJsonObject FilterHighFrequencyParamsWidget::params() const
{
    QJsonObject retval;
    retval["freq_cutoff"] = ui->edFreq->value();
    retval["kind"] = ui->cbKind->currentData().toInt();
    return retval;
}
