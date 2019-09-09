#include "stabsignalstestwidget.h"
#include "ui_stabsignalstestwidget.h"

#include "datadefines.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "classicfactors.h"
#include "vectorfactors.h"
#include "aanalyserapplication.h"
#include "stabsignalstestcalculator.h"
#include "areaskg.h"
#include "areagraph.h"
#include "testresultdata.h"
#include "resultinfo.h"
#include "settingsprovider.h"

#include <QTimer>
#include <QDebug>

StabSignalsTestWidget::StabSignalsTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabSignalsTestWidget)
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    auto val = SettingsProvider::valueFromRegAppCopy("StabSignalsTestWidget", "CurrentPage").toInt();
    ui->tabWidget->setCurrentIndex(val);

    restoreSplitterPosition();
    ui->sbSignal->setEnabled(false);
    ui->btnHScalePlus->setEnabled(false);
    ui->btnHScaleMinus->setEnabled(false);
    ui->cbScale->addItems(QStringList() << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128");
}

StabSignalsTestWidget::~StabSignalsTestWidget()
{
    if (m_trd)
        delete m_trd;
    delete ui;
}

void StabSignalsTestWidget::calculate(StabSignalsTestCalculator *calculator, const QString &testUid)
{
    showTable(calculator, testUid);
    showSKG(calculator, testUid);
}

void StabSignalsTestWidget::zoomIn()
{
    for (int i = 0; i < ui->wgtSKGAreases->layout()->count(); ++i)
    {
        QLayoutItem* item = ui->wgtSKGAreases->layout()->itemAt(i);
        auto diap = static_cast<AreaSKG*>(item->widget())->diap();
        if (diap > 1)
            static_cast<AreaSKG*>(item->widget())->setDiap(diap / 2);
    }
}

void StabSignalsTestWidget::zoomOut()
{
    for (int i = 0; i < ui->wgtSKGAreases->layout()->count(); ++i)
    {
        QLayoutItem* item = ui->wgtSKGAreases->layout()->itemAt(i);
        auto diap = static_cast<AreaSKG*>(item->widget())->diap();
        if (diap < 128)
            static_cast<AreaSKG*>(item->widget())->setDiap(diap * 2);
    }
}

void StabSignalsTestWidget::zeroing(bool isZeroing)
{
    for (int i = 0; i < ui->wgtSKGAreases->layout()->count(); ++i)
    {
        QLayoutItem* item = ui->wgtSKGAreases->layout()->itemAt(i);
        static_cast<AreaSKG*>(item->widget())->setZeroing(isZeroing);
    }
}

void StabSignalsTestWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void StabSignalsTestWidget::curPageChanged(int pageIdx)
{
    SettingsProvider::setValueToRegAppCopy("StabSignalsTestWidget", "CurrentPage", pageIdx);
}

void StabSignalsTestWidget::scaleChange(int scaleIdx)
{
    int diap = 128;
    for (int i = 0; i < scaleIdx; ++i)
        diap = diap / 2;
    ui->wgtGraph->setDiapazone(-diap, diap);
}

void StabSignalsTestWidget::fullSignalChanged(bool isFullSignal)
{
    ui->sbSignal->setEnabled(!isFullSignal);
    ui->btnHScalePlus->setEnabled(!isFullSignal);
    ui->btnHScaleMinus->setEnabled(!isFullSignal);
    if (isFullSignal)
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_fullSignal);
    else
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_scrolling);
}

void StabSignalsTestWidget::zeroingChanged(bool isZeroing)
{
    ui->wgtGraph->setIsZeroing(isZeroing);
}

void StabSignalsTestWidget::signalScroll(int pos)
{
    ui->wgtGraph->setStartPoint(ui->wgtGraph->area(0)->signal()->size() * pos / 100);
}

void StabSignalsTestWidget::hScaleZoomIn()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale < 8)
        ui->wgtGraph->setHScale(hScale * 2);
}

void StabSignalsTestWidget::hScaleZoomOut()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale > 1)
        ui->wgtGraph->setHScale(hScale / 2);
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

    addFactorsFromMultifactor(calculator, fgiClassic);
    addFactorsFromMultifactor(calculator, fgiVector);

    m_mdlTable.setHorizontalHeaderLabels(headerLabels);
    ui->tvFactors->setModel(&m_mdlTable);
    for (int i = 0; i < m_mdlTable.columnCount(); ++i)
        ui->tvFactors->resizeColumnToContents(i);
}

void StabSignalsTestWidget::addFactorsFromMultifactor(StabSignalsTestCalculator *calculator, const FactorGroupId fgi)
{
    auto *app = static_cast<AAnalyserApplication*>(QApplication::instance());
    for (int i = 0; i < factorCount(calculator, fgi); ++i)
    {
        QList<QStandardItem*> items;

        auto fi = app->getFactorInfo(factorUid(calculator, fgi, i));
        auto fctName = fi.name();
        if (fi.measure() != "")
            fctName = fctName + ", " + fi.measure();
        auto *itemFactor = new QStandardItem(fctName);
        itemFactor->setEditable(false);
        items << itemFactor;

        for (int j = 0; j < calculator->probesCount(); ++j)
        {
            auto value = factorValue(calculator, fgi, j, i);
            auto *item = new QStandardItem(QString::number(value, 'f', fi.format()));
            item->setEditable(false);
            items << item;
        }

        m_mdlTable.appendRow(items);
    }
}

int StabSignalsTestWidget::factorCount(StabSignalsTestCalculator *calculator,
                                       const StabSignalsTestWidget::FactorGroupId fgi) const
{
    int n = 0;
    switch (fgi) {
    case fgiClassic:
        n = calculator->classicFactors(0)->size();
        break;
    case fgiVector:
        n = calculator->vectorFactors(0)->size();
        break;
    default:
        break;
    }
    return n;
}

QString StabSignalsTestWidget::factorUid(StabSignalsTestCalculator *calculator,
                                         const StabSignalsTestWidget::FactorGroupId fgi,
                                         const int factorNum) const
{
    QString fUid = "";
    switch (fgi) {
    case fgiClassic:
        fUid = calculator->classicFactors(0)->factorUid(factorNum);
        break;
    case fgiVector:
        fUid = calculator->vectorFactors(0)->factorUid(factorNum);
        break;
    default:
        break;
    }
    return fUid;
}

double StabSignalsTestWidget::factorValue(StabSignalsTestCalculator *calculator,
                                          const StabSignalsTestWidget::FactorGroupId fgi,
                                          const int probeNum, const int factorNum) const
{
    double val = 0;
    switch (fgi) {
    case fgiClassic:
        val = calculator->classicFactors(probeNum)->factorValue(factorNum);
        break;
    case fgiVector:
        val = calculator->vectorFactors(probeNum)->factorValue(factorNum);
        break;
    default:
        break;
    }
    return val;
}

void StabSignalsTestWidget::showSKG(StabSignalsTestCalculator *calculator, const QString &testUid)
{
    Q_UNUSED(calculator);
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        double absMax = 0;
        QList<AreaSKG*> skgList;
        m_trd->openTest(testUid);
        Q_ASSERT(ti.probes.size() == m_trd->probesCount());
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                auto* probe = m_trd->probe(i);
                auto *skg = new AreaSKG(ui->wgtSKGAreases);
                skgList << skg;
                skg->setVisibleMarker(false);

                auto *sig = probe->signal(ChannelsDefines::chanStab);
                skg->setSignal(sig);
                auto max = sig->absMaxValue();
                if (max > absMax)
                    absMax = max;

                auto angle = calculator->classicFactors(i)->ellipse().angle;
                auto sizeA = calculator->classicFactors(i)->ellipse().sizeA;
                auto sizeB = calculator->classicFactors(i)->ellipse().sizeB;
                skg->setEllipse(sizeA, sizeB, angle);

                ui->wgtSKGAreases->layout()->addWidget(skg);

                //! Графики сигналов
                ui->wgtGraph->appendSignal(sig, pi.name);
            }
        }

        //! Установка диапазонов для всех СКГ
        QTimer::singleShot(0, [=]()
        {
            int diap = 1;
            int step = 0;
            while (diap < absMax)
            {
                diap = diap * 2;
                ++step;
            }
            foreach (auto* skg, skgList)
                skg->setDiap(diap);
            ui->wgtGraph->setDiapazone(-diap, diap);
            ui->cbScale->setCurrentIndex(7 - step);
        });
    }
}

void StabSignalsTestWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("StabSignalsTestWidget", "SplitterPosition", ui->splitter->saveState());
}

void StabSignalsTestWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("StabSignalsTestWidget", "SplitterPosition").toByteArray();
    ui->splitter->restoreState(val);
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
