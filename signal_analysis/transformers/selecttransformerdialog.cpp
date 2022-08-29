#include "selecttransformerdialog.h"
#include "ui_selecttransformerdialog.h"

#include <QStandardItemModel>
#include <QUuid>

#include "aanalyserapplication.h"
#include "signaltransformerparamswidget.h"

SelectTransformerDialog::SelectTransformerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectTransformerDialog)
{
    ui->setupUi(this);
    load();
}

SelectTransformerDialog::~SelectTransformerDialog()
{
    delete ui;
}

QString SelectTransformerDialog::transformer() const
{
    auto idx = ui->lvTransform->currentIndex();
    if (idx.isValid())
        return idx.data(UidRole).toString();
    return QUuid().toString();
}

QJsonObject SelectTransformerDialog::params() const
{
    if (m_wgtParams)
        return m_wgtParams->params();
    return QJsonObject();
}


void SelectTransformerDialog::on_selectItem(QModelIndex index)
{
    auto uid = index.data(UidRole).toString();
    m_wgtParams = static_cast<AAnalyserApplication*>(QApplication::instance())->getSignalTransformParamsWidget(uid);

    if (m_wgtParams)
    {
        //! Очистка панели кнопок
        while (QLayoutItem* item = ui->frParams->layout()->takeAt(0))
        {
            delete item->widget();
            delete item;
        }

        ui->frParams->layout()->addWidget(static_cast<QWidget*>(m_wgtParams));
    }
}

void SelectTransformerDialog::load()
{
    auto app = static_cast<AAnalyserApplication*>(QApplication::instance());
    auto model = new QStandardItemModel(ui->lvTransform);
    for (int i = 0; i < app->signalTransformersCount(); ++i)
    {
        auto item = new QStandardItem(app->signalTransformerName(i));
        item->setData(app->signalTransformerUid(i), UidRole);
        model->appendRow(item);
    }
    ui->lvTransform->setModel(model);
}
