#include "stabsignalstestwidget.h"
#include "ui_stabsignalstestwidget.h"

#include "datadefines.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "classicfactors.h"
#include "aanalyserapplication.h"
#include "stabsignalstestcalculator.h"
#include "areaskg.h"

#include <QDebug>

StabSignalsTestWidget::StabSignalsTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabSignalsTestWidget)
{
    ui->setupUi(this);
}

StabSignalsTestWidget::~StabSignalsTestWidget()
{
    delete ui;
}

void StabSignalsTestWidget::calculate(StabSignalsTestCalculator *calculator, const QString &testUid)
{
    showTable(calculator, testUid);
    showSKG(calculator, testUid);
}

void StabSignalsTestWidget::showTable(StabSignalsTestCalculator *calculator, const QString &testUid)
{
    QStringList headerLabels;
    headerLabels << tr("Показатель");

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
        foreach (auto probeUid, ti.probes)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(probeUid, pi))
                headerLabels << pi.name;
        }

    auto *app = static_cast<AAnalyserApplication*>(QApplication::instance());

    for (int i = 0; i < calculator->factorsCount(); ++i)
    {
        QList<QStandardItem*> items;

        auto fi = app->getFactorInfo(calculator->classicFactors(0)->factorUid(i));
        auto *itemFactor = new QStandardItem(fi.name());
        itemFactor->setEditable(false);
        items << itemFactor;

        for (int j = 0; j < calculator->probesCount(); ++j)
        {
            auto value = calculator->classicFactors(j)->factorValue(i);
            auto *item = new QStandardItem(QString::number(value));
            item->setEditable(false);
            items << item;
        }

        m_mdlTable.appendRow(items);
    }

    m_mdlTable.setHorizontalHeaderLabels(headerLabels);
    ui->treeView->setModel(&m_mdlTable);
    for (int i = 0; i < m_mdlTable.columnCount(); ++i)
        ui->treeView->resizeColumnToContents(i);
}

void StabSignalsTestWidget::showSKG(StabSignalsTestCalculator *calculator, const QString &testUid)
{
    Q_UNUSED(calculator);
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
        foreach (auto probeUid, ti.probes)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(probeUid, pi))
            {
                auto *skg = new AreaSKG(ui->widget);
                ui->widget->layout()->addWidget(skg);
            }
        }
}
