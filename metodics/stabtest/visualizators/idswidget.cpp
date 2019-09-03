#include "idswidget.h"
#include "ui_idswidget.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "aanalyserapplication.h"
#include "settingsprovider.h"

#include "idscalculator.h"
#include "idsfactors.h"

#include <QDebug>

IDSWidget::IDSWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IDSWidget)
{
    ui->setupUi(this);
    restoreSplitterPosition();
}

IDSWidget::~IDSWidget()
{
    delete ui;
}

void IDSWidget::calculate(IDSCalculator *calculator, const QString &testUid)
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

    addFactorsFromMultifactor(calculator);

//    for (int i = 0; i < calculator->factors(0)->fdsSize(); ++i)
//        qDebug() << calculator->factors(0)->fds(i);


    m_mdlTable.setHorizontalHeaderLabels(headerLabels);
    ui->tvFactors->setModel(&m_mdlTable);
    for (int i = 0; i < m_mdlTable.columnCount(); ++i)
        ui->tvFactors->resizeColumnToContents(i);
}

void IDSWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void IDSWidget::addFactorsFromMultifactor(IDSCalculator *calculator)
{
    auto *app = static_cast<AAnalyserApplication*>(QApplication::instance());
    for (int i = 0; i < calculator->factors(0)->size(); ++i)
    {
        QList<QStandardItem*> items;
        auto fi = app->getFactorInfo(calculator->factors(0)->factorUid(i));
        auto fctName = fi.name();
        if (fi.measure() != "")
            fctName = fctName + ", " + fi.measure();
        auto *itemFactor = new QStandardItem(fctName);
        itemFactor->setEditable(false);
        items << itemFactor;

        for (int j = 0; j < calculator->probesCount(); ++j)
        {
            auto value = calculator->factors(j)->factorValue(i);
            auto *item = new QStandardItem(QString::number(value, 'f', fi.format()));
            item->setEditable(false);
            items << item;
        }

        m_mdlTable.appendRow(items);
    }
}

void IDSWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("IDSTestWidget", "SplitterDiagramPosition", ui->splDiagrams->saveState());
    SettingsProvider::setValueToRegAppCopy("IDSTestWidget", "SplitterTableDiagPosition", ui->splTblDiag->saveState());
}

void IDSWidget::restoreSplitterPosition()
{
    auto valDiag = SettingsProvider::valueFromRegAppCopy("IDSTestWidget", "SplitterDiagramPosition").toByteArray();
    ui->splDiagrams->restoreState(valDiag);
    auto valTblDiag = SettingsProvider::valueFromRegAppCopy("IDSTestWidget", "SplitterTableDiagPosition").toByteArray();
    ui->splTblDiag->restoreState(valTblDiag);
}
