#include "stabsignalstestwidget.h"
#include "ui_stabsignalstestwidget.h"

#include "datadefines.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "classicfactors.h"
#include "aanalyserapplication.h"
#include "stabsignalstestcalculator.h"
#include "areaskg.h"
#include "testresultdata.h"
#include "resultinfo.h"

#include <QDebug>

StabSignalsTestWidget::StabSignalsTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabSignalsTestWidget)
  , m_trd(new TestResultData())
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
    {
        m_trd->openTest(testUid);
        Q_ASSERT(ti.probes.size() == m_trd->probesCount());
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                auto* probe = m_trd->probe(i);
                auto *skg = new AreaSKG(ui->widget);
                skg->setSignal(probe->signal(ChannelsDefines::chanStab));
                ui->widget->layout()->addWidget(skg);
            }
        }
    }
}

//    m_trd->openTest(testUid);
//    for (int i = 0; i < m_trd->probesCount(); ++i)
//    {
//        auto* probe = m_trd->probe(i);

//        QTextEdit *edit = new QTextEdit(ui->wgtUndef);
//        ui->wgtUndef->layout()->addWidget(edit);

//        for (int j = 0; j < probe->signalsCount(); ++j)
//        {
//            auto *signal = probe->signal(j);
//            for (int k = 0; k < signal->size(); ++k)
//            {
//                if (signal->channelId() == ChannelsDefines::chanStab)
//                {
//                     auto x = signal->value(0, k);
//                     auto y = signal->value(1, k);

//                     auto sx = QString::number(x);
//                     auto sy = QString::number(y);
//                     edit->append(sx + "     " + sy);
//                }
//                else
//                if (signal->channelId() == ChannelsDefines::chanZ)
//                {
//                    auto z = signal->value(0, k);
//                    auto sz = QString::number(z);
//                    edit->append(sz);
//                }
//            }
//            delete signal;
//        }

//        delete probe;
//    }
