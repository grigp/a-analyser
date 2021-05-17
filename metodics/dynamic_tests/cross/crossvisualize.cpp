#include "crossvisualize.h"
#include "ui_crossvisualize.h"

#include <QStandardItemModel>

#include "aanalyserapplication.h"
#include "crosscalculator.h"


CrossVisualize::CrossVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CrossVisualize)
{
    ui->setupUi(this);
}

CrossVisualize::~CrossVisualize()
{
    delete ui;
}

void CrossVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new CrossCalculator(testUid, this);
        m_calculator->calculate();

        auto model = new QStandardItemModel();
        for (int i = 0; i < m_calculator->primaryFactorsCount(); ++i)
        {
            auto factor = m_calculator->primaryFactor(i);
            auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factor->uid());
            auto *itemName = new QStandardItem(fi.name());
            itemName->setEditable(false);
            auto *itemValue = new QStandardItem(QString::number(factor->value()));
            itemValue->setEditable(false);
            model->appendRow(QList<QStandardItem*>() << itemName << itemValue);
        }

        model->setHorizontalHeaderLabels(QStringList() << tr("Показатель")
                                                       << tr("Значение"));
        ui->tvFactors->setModel(model);
        ui->tvFactors->header()->resizeSections(QHeaderView::ResizeToContents);
        ui->tvFactors->header()->resizeSection(0, 450);
    }
}
