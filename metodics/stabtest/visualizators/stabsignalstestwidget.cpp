#include "stabsignalstestwidget.h"
#include "ui_stabsignalstestwidget.h"

#include "datadefines.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "classicfactors.h"
#include "aanalyserapplication.h"

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

//void StabSignalsTestWidget::calculate(const QString &testUid)
//{
//    QStringList headerLabels;
//    headerLabels << tr("Показатель");
//    auto *app = static_cast<AAnalyserApplication*>(QApplication::instance());

//    DataDefines::TestInfo ti;
//    if (DataProvider::getTestInfo(testUid, ti))
//    {
//        for (int i = 0; i < ti.probes.size(); ++i)
//        {
//            DataDefines::ProbeInfo pi;
//            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
//            {
//                ClassicFactors fct(testUid, pi.uid, ChannelsDefines::chanStab);
//                headerLabels << pi.name;

//                QList<QStandardItem*> itemsV, itemsN;

//                for (int j = 0; j < fct.size(); ++j)
//                {
//                    if (i == 0)
//                    {
//                        auto fi = app->getFactorInfo(fct.factorUid(i));
//                        auto *itemFactor = new QStandardItem(fi.name());
//                        itemFactor->setEditable(false);
//                        itemsN << itemFactor;
//                    }

//                    auto value = fct.factorValue(j);
//                    auto *item = new QStandardItem(QString::number(value));
//                    item->setEditable(false);
//                    itemsV << item;
//                }

//                if (i == 0)
//                    m_mdlTable.appendColumn(itemsN);
//                m_mdlTable.appendColumn(itemsV);
//            }
//        }
//    }
//    m_mdlTable.setHorizontalHeaderLabels(headerLabels);
//}

void StabSignalsTestWidget::calculate(const QString &testUid)
{
    QList<ClassicFactors*> fctClass;
    QStringList headerLabels;
    headerLabels << tr("Показатель");

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        foreach (auto probeUid, ti.probes)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(probeUid, pi))
            {
                auto *fct = new ClassicFactors(testUid, pi.uid, ChannelsDefines::chanStab);
                fctClass << fct;
                headerLabels << pi.name;
            }
        }
    }

    auto *app = static_cast<AAnalyserApplication*>(QApplication::instance());

    if (fctClass.size() > 0)
    {
        for (int i = 0; i < fctClass.at(0)->size(); ++i)
        {
            QList<QStandardItem*> items;

            auto fi = app->getFactorInfo(fctClass.at(0)->factorUid(i));
            auto *itemFactor = new QStandardItem(fi.name());
            itemFactor->setEditable(false);
            items << itemFactor;

            foreach (auto fct, fctClass)
            {
                auto value = fct->factorValue(i);
                auto *item = new QStandardItem(QString::number(value));
                item->setEditable(false);
                items << item;
            }
            m_mdlTable.appendRow(items);
        }
    }

    m_mdlTable.setHorizontalHeaderLabels(headerLabels);
    ui->treeView->setModel(&m_mdlTable);

}
