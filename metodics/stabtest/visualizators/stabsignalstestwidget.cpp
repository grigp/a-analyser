#include "stabsignalstestwidget.h"
#include "ui_stabsignalstestwidget.h"

#include "baseutils.h"
#include "datadefines.h"
#include "channelsdefines.h"
#include "dataprovider.h"
#include "classicfactors.h"
#include "vectorfactors.h"
#include "aanalyserapplication.h"
#include "stabsignalstestcalculator.h"
#include "skgwidget.h"
#include "areagraph.h"
#include "testresultdata.h"
#include "resultinfo.h"
#include "settingsprovider.h"
#include "ratioprobesfactors.h"
#include "stabtesttemplate.h"
#include "rombergnormvaluedelegate.h"
#include "reportelements.h"

#include <QTimer>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

namespace  {

/*!
 * \brief Структура нормы для теста Ромберга NormRomberg struct
 */
struct NormRomberg
{
    double norm;
    double stdDeviation;
    double low;
    double high;
    NormRomberg(double nv, double stdDev, double l, double h)
        : norm(nv), stdDeviation(stdDev), low(l), high(h)
    {}
    NormRomberg() {}
};

static QMap<QString, NormRomberg> NormsRombergOpenEyes {
    std::pair<QString, NormRomberg> (ClassicFactorsDefines::QXUid, NormRomberg(4.35, 0.73, 1.9, 10.24))
  , std::pair<QString, NormRomberg> (ClassicFactorsDefines::QYUid, NormRomberg(3.26, 0.39, 1.53, 5.26))
  , std::pair<QString, NormRomberg> (ClassicFactorsDefines::LUid, NormRomberg(276, 20.9, 159, 378))
  , std::pair<QString, NormRomberg> (ClassicFactorsDefines::SquareUid, NormRomberg(460, 88.07, 128, 966))
};
static QMap<QString, NormRomberg> NormsRombergCloseEyes {
    std::pair<QString, NormRomberg> (ClassicFactorsDefines::QXUid, NormRomberg(6.44, 0.57, 3.83, 9.84))
  , std::pair<QString, NormRomberg> (ClassicFactorsDefines::QYUid, NormRomberg(4.97, 0.47, 2.96, 7.31))
  , std::pair<QString, NormRomberg> (ClassicFactorsDefines::LUid, NormRomberg(482, 36.6, 357, 966))
  , std::pair<QString, NormRomberg> (ClassicFactorsDefines::SquareUid, NormRomberg(1119, 166.9, 585, 2314))
};

QStandardItemModel *mdlTable {nullptr};
QStandardItemModel *mdlRF {nullptr};
QStandardItemModel *mdlNorms {nullptr};
QTreeView *tvRombergNorms {nullptr};
QList<SKGWidget*> areasesSKG;
AreaGraph *wgtGraph;
QTabWidget *wgtTab;
}

StabSignalsTestWidget::StabSignalsTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabSignalsTestWidget)
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    ui->sbSignal->setEnabled(false);
    ui->btnHScalePlus->setEnabled(false);
    ui->btnHScaleMinus->setEnabled(false);
    foreach (auto scaleName, BaseUtils::ScalesStab)
        ui->cbScale->addItem(scaleName, BaseUtils::ScaleKoefitients.value(scaleName));
}

StabSignalsTestWidget::~StabSignalsTestWidget()
{
    if (m_trd)
        delete m_trd;
    if (m_mdlRF)
        delete m_mdlRF;
    if (m_mdlNorms)
        delete m_mdlNorms;
    delete ui;
}

void StabSignalsTestWidget::calculate(StabSignalsTestCalculator *calculator, const QString &testUid)
{
    auto val = SettingsProvider::valueFromRegAppCopy("StabSignalsTestWidget", "CurrentPage").toInt();
    ui->tabWidget->setCurrentIndex(val);
    wgtTab = ui->tabWidget;

    showTable(calculator, testUid);
    showSKG(calculator, testUid);
    showRationalTable(calculator, testUid);
    showRombergNorms(calculator, testUid);
    restoreSplitterPosition();
}

void StabSignalsTestWidget::print(QPrinter *printer, const QString &testUid)
{
    int tab = wgtTab->currentIndex();
    for (int i = 0; i < wgtTab->count(); ++i)
        wgtTab->setCurrentIndex(i);
    wgtTab->setCurrentIndex(tab);

    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
        if (mdlTable->columnCount() - 1 == 1)
            printOnePortrait(printer, painter, testUid, paper);
        else
        if (mdlTable->columnCount() - 1 == 2)
            printTwoPortrait(printer, painter, testUid, paper);
        else
        if (mdlTable->columnCount() - 1 == 3)
            printThreePortrait(printer, painter, testUid, paper);
        else
        if (mdlTable->columnCount() - 1 == 5)
            printFivePortrait(printer, painter, testUid, paper);

    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        if (mdlTable->columnCount() - 1 == 1)
            printOneLandscape(printer, painter, testUid, paper);
        else
        if (mdlTable->columnCount() - 1 == 2)
            printTwoLandscape(printer, painter, testUid, paper);
        else
        if (mdlTable->columnCount() - 1 == 3)
            printThreeLandscape(printer, painter, testUid, paper);
        else
        if (mdlTable->columnCount() - 1 == 5)
            printFiveLandscape(printer, painter, testUid, paper);
    }

    //! Нижний колонтитул
    ReportElements::drawFooter(painter, testUid, rectFooter(paper));

    painter->end();
}

void StabSignalsTestWidget::setVisible(bool visible)
{
    QWidget::setVisible(visible);

    if (visible)
    {
        resizeColumnsTable(m_mdlRF, ui->tvRationalFactors, true);
        resizeColumnsTable(m_mdlNorms, ui->tvRombergNorms, false);
        setDiapazones();
    }
}

void StabSignalsTestWidget::resizeEvent(QResizeEvent *event)
{
    resizeColumnsTable(m_mdlNorms, ui->tvRombergNorms, false);
    resizeColumnsTable(m_mdlRF, ui->tvRationalFactors, true);
    QWidget::resizeEvent(event);
}

void StabSignalsTestWidget::zoomIn()
{
    for (int i = 0; i < ui->wgtSKGAreases->layout()->count(); ++i)
    {
        QLayoutItem* item = ui->wgtSKGAreases->layout()->itemAt(i);
        auto diap = static_cast<SKGWidget*>(item->widget())->diap();
        if (diap > 1)
            static_cast<SKGWidget*>(item->widget())->setDiap(diap / 2);
    }
}

void StabSignalsTestWidget::zoomOut()
{
    for (int i = 0; i < ui->wgtSKGAreases->layout()->count(); ++i)
    {
        QLayoutItem* item = ui->wgtSKGAreases->layout()->itemAt(i);
        auto diap = static_cast<SKGWidget*>(item->widget())->diap();
        if (diap < 128)
            static_cast<SKGWidget*>(item->widget())->setDiap(diap * 2);
    }
}

void StabSignalsTestWidget::zeroing(bool isZeroing)
{
    for (int i = 0; i < ui->wgtSKGAreases->layout()->count(); ++i)
    {
        QLayoutItem* item = ui->wgtSKGAreases->layout()->itemAt(i);
        static_cast<SKGWidget*>(item->widget())->setZeroing(isZeroing);
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
    Q_UNUSED(scaleIdx);
//    int diap = 128;
//    for (int i = 0; i < scaleIdx; ++i)
//        diap = diap / 2;
//    ui->wgtGraph->setDiapazone(-diap, diap);

    ui->wgtGraph->setDiapazone(-BaseUtils::StabDefaultDiap / ui->cbScale->currentData().toDouble(),
                               BaseUtils::StabDefaultDiap / ui->cbScale->currentData().toDouble());
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
    int w = ui->wgtGraph->width() - ui->wgtGraph->leftSpace() - ui->wgtGraph->rightSpace();
    if (ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() > w)
    {
        int p = static_cast<int>((ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() - w)
                                 * pos / 100.0 / ui->wgtGraph->hScale());
        ui->wgtGraph->setStartPoint(p);
    }
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

    mdlTable = &m_mdlTable;
}

void StabSignalsTestWidget::showRationalTable(StabSignalsTestCalculator *calculator, const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        QString metPrefix = "";
        if (isRombergTest(ti))
            metPrefix = tr("Коэффициент Ромберга");
        m_mdlRF = new QStandardItemModel(this);
        QStandardItem* itemFctS = new QStandardItem(metPrefix + " " + tr("по площади эллипса"));
        QStandardItem* itemFctKFR = new QStandardItem(metPrefix + " " + tr("по КФР"));
        QStandardItem* itemFctMoX = new QStandardItem(tr("Смещение по X"));
        QStandardItem* itemFctMoY = new QStandardItem(tr("Смещение по Y"));
        QStandardItem* itemFctQX = new QStandardItem(tr("Разброс по X"));
        QStandardItem* itemFctQY = new QStandardItem(tr("Разброс по Y"));
        appendColumnReadOnly(m_mdlRF,
                             QList<QStandardItem*>() <<
                                    itemFctS << itemFctKFR << itemFctMoX << itemFctMoY << itemFctQX << itemFctQY);

        ui->frRationalFactors->setVisible(ti.probes.size() > 1);
        RatioProbesFactors* rationalFactors = new RatioProbesFactors(testUid);
        QStandardItem* itemS = nullptr;
        for (int i = 0; i < rationalFactors->size(); ++i)
        {
            QString uid = rationalFactors->factorUid(i);
            FactorsDefines::FactorInfo fi = static_cast<AAnalyserApplication*>(QApplication::instance())->
                    getFactorInfo(uid);
            if (i % 2 == 0)
            {
                if (isRombergTest(ti))
                    itemS = new QStandardItem(getKoefRombResume(rationalFactors->factorValue(i), fi.format()));
                else
                    itemS = new QStandardItem(QString::number(rationalFactors->factorValue(i), 'f', fi.format()));
            }
            else
            if (i % 2 == 1)
            {
                QStandardItem* itemKFR = nullptr;
                if (isRombergTest(ti))
                    itemKFR = new QStandardItem(getKoefRombResume(rationalFactors->factorValue(i), fi.format()));
                else
                    itemKFR = new QStandardItem(QString::number(rationalFactors->factorValue(i), 'f', fi.format()));

                QStandardItem* itemMoX = createItemRationalFactors(calculator, i, ClassicFactorsDefines::MoXUid, 1, 'x');
                QStandardItem* itemMoY = createItemRationalFactors(calculator, i, ClassicFactorsDefines::MoYUid, 1, 'y');
                QStandardItem* itemQX = createItemRationalFactors(calculator, i, ClassicFactorsDefines::QXUid, 0, 'x');
                QStandardItem* itemQY = createItemRationalFactors(calculator, i, ClassicFactorsDefines::QYUid, 0, 'y');

                if (itemS)
                    appendColumnReadOnly(m_mdlRF,
                                         QList<QStandardItem*>() <<
                                                itemS << itemKFR << itemMoX << itemMoY << itemQX << itemQY);
            }
        }
        delete rationalFactors;

        //! Заголовок таблицы
        QStringList mdlCaption;
        mdlCaption << tr("Показатель");
        for (int i = 1; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
                mdlCaption << pi.name;
        }
        m_mdlRF->setHorizontalHeaderLabels(mdlCaption);

        ui->tvRationalFactors->setModel(m_mdlRF);
        ui->tvRationalFactors->resizeColumnToContents(0);

        mdlRF = static_cast<QStandardItemModel*>(ui->tvRationalFactors->model());
    }
}

void StabSignalsTestWidget::showRombergNorms(StabSignalsTestCalculator *calculator, const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        bool isRomb = isRombergTest(ti);
        ui->frRombergNorms->setVisible(isRomb);
        if (isRomb)
        {
            m_mdlNorms = new QStandardItemModel(this);

            QStandardItem* itemQX = new QStandardItem(tr("Разброс по X"));
            QStandardItem* itemQY = new QStandardItem(tr("Разброс по Y"));
            QStandardItem* itemL = new QStandardItem(tr("Длина статокинезиграммы"));
            QStandardItem* itemS = new QStandardItem(tr("Площадь статокинезиграммы"));
            appendColumnReadOnly(m_mdlNorms, QList<QStandardItem*>() << itemQX << itemQY << itemL << itemS);

            for (int i = 0; i < calculator->probesCount(); ++i)
            {
                auto getItem = [&](const QString &factorUid) -> QStandardItem*
                {
                    FactorsDefines::FactorInfo fi = static_cast<AAnalyserApplication*>(QApplication::instance())->
                            getFactorInfo(factorUid);
                    double value = calculator->classicFactors(i)->factorValue(factorUid);
                    auto nv = getRombergNorm(i, factorUid, value);
                    auto *item = new QStandardItem("");
                    item->setData(nv, NormRole);
                    item->setData(value, ValueRole);
                    item->setData(fi.format(), FormatRole);
                    return item;
                };

                QStandardItem* itemQX = getItem(ClassicFactorsDefines::QXUid);
                QStandardItem* itemQY = getItem(ClassicFactorsDefines::QYUid);
                QStandardItem* itemL = getItem(ClassicFactorsDefines::LUid);
                QStandardItem* itemS = getItem(ClassicFactorsDefines::SquareUid);
                appendColumnReadOnly(m_mdlNorms, QList<QStandardItem*>() << itemQX << itemQY << itemL << itemS);
            }

            //! Заголовок таблицы
            QStringList mdlCaption;
            mdlCaption << tr("Показатель");
            for (int i = 0; i < ti.probes.size(); ++i)
            {
                DataDefines::ProbeInfo pi;
                if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
                    mdlCaption << pi.name;
            }
            m_mdlNorms->setHorizontalHeaderLabels(mdlCaption);

            ui->tvRombergNorms->setModel(m_mdlNorms);
            ui->tvRombergNorms->setItemDelegateForColumn(1, new RombergNormValueDelegate(ui->tvRombergNorms));
            ui->tvRombergNorms->setItemDelegateForColumn(2, new RombergNormValueDelegate(ui->tvRombergNorms));
            ui->tvRombergNorms->resizeColumnToContents(0);

            mdlNorms = static_cast<QStandardItemModel*>(ui->tvRombergNorms->model());
        }
    }
    tvRombergNorms = ui->tvRombergNorms;
}

DataDefines::NormValue StabSignalsTestWidget::getRombergNorm(const int probeNum,
                                                             const QString &factorUid,
                                                             const double value) const
{
    NormRomberg norm;
    if (probeNum == 0)
        norm = NormsRombergOpenEyes.value(factorUid);
    else
        norm = NormsRombergCloseEyes.value(factorUid);

    if (value <= norm.norm + norm.stdDeviation)
        return DataDefines::Normal;
    else
    if (value > norm.high)
        return DataDefines::NotNormal;
    else
        return DataDefines::ConditionNormal;
//    if (value >= norm.norm - norm.stdDeviation && value <= norm.norm + norm.stdDeviation)
//        return DataDefines::Normal;
//    else
//    if (value < norm.low || value > norm.high)
//        return DataDefines::NotNormal;
//    else
//        return DataDefines::ConditionNormal;
}

QStandardItem* StabSignalsTestWidget::createItemRationalFactors(StabSignalsTestCalculator *calculator,
                                                                const int numProbe,
                                                                const QString &factorUid, const int action, const char chan)
{
    double val = 0;
    QString resume = "";
    if (action == 0)
    {
        val =
            calculator->classicFactors(numProbe / 2 + 1)->factorValue(factorUid) /
            calculator->classicFactors(0)->factorValue(factorUid);
        resume = getDeviationResume(val);
    }
    else
    {
        val =
            calculator->classicFactors(numProbe / 2 + 1)->factorValue(factorUid) -
            calculator->classicFactors(0)->factorValue(factorUid);
        resume = getOffsetResume(val, chan);
    }
    QStandardItem* item = new QStandardItem(resume);
    return item;
}

void StabSignalsTestWidget::appendColumnReadOnly(QStandardItemModel *mdl, QList<QStandardItem *> list)
{
    foreach (auto *item, list)
        item->setEditable(false);
    mdl->appendColumn(list);
}

bool StabSignalsTestWidget::isRombergTest(DataDefines::TestInfo ti)
{
    QList<int> kinds = getProbesKind(ti.params);
    return (kinds.size() == 2 &&
            static_cast<StabTestParams::ProbeKinds>(kinds.at(0)) == StabTestParams::pkBackground &&
            static_cast<StabTestParams::ProbeKinds>(kinds.at(1)) == StabTestParams::pkCloseEyes);
}

QString StabSignalsTestWidget::getKoefRombResume(const double value, const int format) const
{
    QString sVal = QString::number(value, 'f', format);
    if (value >= 100 && value <= 250)
        return QString("%1 " + tr("В норме") + " (100 - 250)").arg(sVal);
    else
    if (value < 100)
        return QString("%1 " + tr("Ниже нормы (Постуральная слепота)")).arg(sVal);
    else
        return QString("%1 " + tr("Выше нормы (Держится за счет зрения)")).arg(sVal);
}

QString StabSignalsTestWidget::getOffsetResume(const double value, const char chan) const
{
    QString sVal = QString::number(fabs(value), 'f', 2);
    QString sDir = "";
    if (value >= 0 && chan == 'x')
        sDir = tr("Вправо");
    else
    if (value < 0 && chan == 'x')
        sDir = tr("Влево");
    else
    if (value >= 0 && chan == 'y')
        sDir = tr("Вперед");
    else
    if (value < 0 && chan == 'y')
        sDir = tr("Назад");

    return QString(sDir + " " + tr("на") + " %1 " + tr("мм")).arg(sVal);
}

QString StabSignalsTestWidget::getDeviationResume(const double value) const
{
    double v = value;
    QString sDir = "";
    if (v >= 1)
        sDir = tr("Рост");
    else
    {
        sDir = tr("Уменьшение");
        v = 1 / v;
    }
    QString sVal = QString::number(v, 'f', 2);
    return QString(sDir + " " + tr("в") + " %1 " + tr("раз")).arg(sVal);
}

QList<int> StabSignalsTestWidget::getProbesKind(const QJsonObject params)
{
    QList<int> retval;
    QJsonArray probes = params["probes"].toArray();
    for (int i = 0; i < probes.size(); ++i)
    {
        QJsonObject probe = probes.at(i).toObject();
        retval << probe["kind"].toInt();
    }
    return retval;
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
    }
    return val;
}

void StabSignalsTestWidget::showSKG(StabSignalsTestCalculator *calculator, const QString &testUid)
{
    Q_UNUSED(calculator);
    areasesSKG.clear();


    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        m_absMax = 0;
        m_trd->openTest(testUid);
        Q_ASSERT(ti.probes.size() == m_trd->probesCount());
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                auto* probe = m_trd->probe(i);
                auto *skg = new SKGWidget(ui->wgtSKGAreases);
                areasesSKG << skg;
                skg->setVisibleMarker(false);

                auto *sig = probe->signal(ChannelsDefines::chanStab);
                if (sig && sig->size() > 0)
                {
                    skg->setSignal(sig);
                    auto max = sig->absMaxValue();
                    if (max > m_absMax)
                        m_absMax = max;

                    auto angle = calculator->classicFactors(i)->ellipse().angle;
                    auto sizeA = calculator->classicFactors(i)->ellipse().sizeA;
                    auto sizeB = calculator->classicFactors(i)->ellipse().sizeB;
                    skg->setEllipse(sizeA, sizeB, angle);

                    ui->wgtSKGAreases->layout()->addWidget(skg);

                    //! Графики сигналов
                    ui->wgtGraph->appendSignal(sig, pi.name);
                    ui->wgtGraph->setLegend(i, QStringList() << "Фронталь" << "Сагитталь");
                }
            }
        }
    }

    wgtGraph = ui->wgtGraph;
}

void StabSignalsTestWidget::setDiapazones()
{
    int diap = 1;
    int step = 0;
    while (diap < m_absMax)
    {
        diap = diap * 2;
        ++step;
    }
    foreach (auto* skg, areasesSKG)
        skg->setDiap(diap);
    ui->wgtGraph->setDiapazone(-diap, diap);
    //! Позиция в переключателе масштаба
    double scM = BaseUtils::StabDefaultDiap / static_cast<double>(diap);
    for (int i = 0; i < ui->cbScale->count(); ++i)
        if (static_cast<int>(ui->cbScale->itemData(i).toDouble() * 10000) == static_cast<int>(scM * 10000)) // * 10000 - уход от double
        {
            ui->cbScale->setCurrentIndex(i);
            break;
        }
}

void StabSignalsTestWidget::saveSplitterPosition()
{
//    SettingsProvider::setValueToRegAppCopy("StabSignalsTestWidget", "SplitterPosition", ui->splitter->saveState());
    QString sSecName = "StabSignalsTestWidget_" + QString::number(m_mdlTable.columnCount());
    SettingsProvider::setValueToRegAppCopy(sSecName, "AreaSKGSplitterPosition", ui->splAreaSKG->saveState());
}

void StabSignalsTestWidget::restoreSplitterPosition()
{
//    auto val = SettingsProvider::valueFromRegAppCopy("StabSignalsTestWidget", "SplitterPosition").toByteArray();
    QString sSecName = "StabSignalsTestWidget_" + QString::number(m_mdlTable.columnCount());
    auto val = SettingsProvider::valueFromRegAppCopy(sSecName, "AreaSKGSplitterPosition").toByteArray();
    ui->splAreaSKG->restoreState(val);
}

void StabSignalsTestWidget::resizeColumnsTable(QStandardItemModel *mdl, QTreeView* tv, const bool toContens)
{
    if (mdl && mdl->columnCount() > 1)
    {
        int w = (tv->geometry().width() - tv->header()->sectionSize(0)) / (mdl->columnCount() - 1);
        for (int i = 1; i < mdl->columnCount(); ++i)
            if (toContens)
                tv->resizeColumnToContents(i);
            else
                tv->header()->resizeSection(i, w);
    }
}

QRect StabSignalsTestWidget::rectFooter(const QRect &paper)
{
    return  QRect(paper.x() + paper.width() / 20,
                  paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                  paper.width() / 20 * 18,
                  static_cast<int>(paper.height() / 30 * 1.5));
}

void StabSignalsTestWidget::printOnePortrait(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper)
{
    Q_UNUSED(testUid);

    if (areasesSKG.size() == 1)
    {
        auto rectSKG = QRect(paper.x() + paper.width() / 2 - static_cast<int>(paper.width() * 0.3),
                             static_cast<int>(paper.y() + paper.height() / 10 * 1.5),
                             static_cast<int>(paper.width() * 0.6),
                             static_cast<int>(paper.height() * 0.3));

        double ratio = static_cast<double>(paper.width()) / static_cast<double>(areasesSKG.at(0)->geometry().width());
        if (static_cast<double>(paper.height()) / static_cast<double>(areasesSKG.at(0)->geometry().height()) < ratio)
            ratio = static_cast<double>(paper.height()) / static_cast<double>(areasesSKG.at(0)->geometry().height());

        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(testUid, ti))
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(0), pi))
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanStab))
                    ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio);
        }

//        ReportElements::drawWidget(painter, areasesSKG.at(0),
//                                   static_cast<int>(paper.width() * 0.6), static_cast<int>(paper.height() * 0.6),
//                                   paper.x() + paper.width() / 2 - static_cast<int>(paper.width() * 0.3),
//                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));
    }

    QRect rectTable(paper.x() + paper.width() / 10,
                    static_cast<int>(paper.y() + paper.height() / 10 * 5.5),
                    paper.width() / 10 * 8,
                    paper.height() / 2);
    ReportElements::drawTable(painter, mdlTable, rectTable, QList<int>() << 3 << 1,
                              ReportElements::Table::tvsCompressed, 10, -1, QFont::Bold);

    //! Нижний колонтитул
    ReportElements::drawFooter(painter, testUid, rectFooter(paper));

    printer->newPage();

    ReportElements::drawWidget(painter, wgtGraph,
                               static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85),
                               paper.x() + paper.width() / 12,
                               static_cast<int>(paper.y() + paper.height() / 10));
}

void StabSignalsTestWidget::printOneLandscape(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper)
{
    Q_UNUSED(testUid);

    if (areasesSKG.size() == 1)
        ReportElements::drawWidget(painter, areasesSKG.at(0),
                                   static_cast<int>(paper.width() * 0.45), static_cast<int>(paper.height() * 0.45),
                                   paper.x() + paper.width() / 15,
                                   static_cast<int>(paper.y() + paper.height() / 10 * 3));

    QRect rectTable(static_cast<int>(paper.x() + paper.width() / 10 * 4.5),
                    static_cast<int>(paper.y() + paper.height() / 10 * 2),
                    paper.width() / 10 * 5,
                    static_cast<int>(paper.height() * 0.7));
    ReportElements::drawTable(painter, mdlTable, rectTable, QList<int>() << 5 << 2,
                              ReportElements::Table::tvsStretched, 9, -1, QFont::Bold);

    //! Нижний колонтитул
    ReportElements::drawFooter(painter, testUid, rectFooter(paper));

    printer->newPage();

    ReportElements::drawWidget(painter, wgtGraph,
                               static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85),
                               paper.x() + paper.width() / 12,
                               static_cast<int>(paper.y() + paper.height() / 10));
}

void StabSignalsTestWidget::printTwoPortrait(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper)
{
    if (areasesSKG.size() == 2)
    {
        ReportElements::drawWidget(painter, areasesSKG.at(0),
                                   static_cast<int>(paper.width() * 0.43), static_cast<int>(paper.height() * 0.43),
                                   paper.x() + paper.width() / 4 - static_cast<int>(paper.width() * 0.2),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));
        ReportElements::drawWidget(painter, areasesSKG.at(1),
                                   static_cast<int>(paper.width() * 0.43), static_cast<int>(paper.height() * 0.43),
                                   paper.x() + paper.width() / 4 * 3 - static_cast<int>(paper.width() * 0.25),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));

        QRect rectTableRF(static_cast<int>(paper.x() + paper.width() / 10),
                        static_cast<int>(paper.y() + paper.height() / 10 * 4.5),
                        paper.width() / 10 * 8,
                        static_cast<int>(paper.height() * 0.15));
        ReportElements::drawTable(painter, mdlRF, rectTableRF, QList<int>() << 5 << 3,
                                  ReportElements::Table::tvsStretched, 10, -1, QFont::Bold);

        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(testUid, ti))
        {
            bool isRomb = isRombergTest(ti);
            if (isRomb)
            {
                ReportElements::drawWidget(painter, tvRombergNorms,
                                           static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.25),
                                           paper.x() + paper.width() / 10,
                                           static_cast<int>(paper.y() + paper.height() / 10 * 6.5));
            }
        }

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));

        printer->newPage();

        ReportElements::drawWidget(painter, wgtGraph,
                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85),
                                   paper.x() + paper.width() / 12,
                                   static_cast<int>(paper.y() + paper.height() / 20));
        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10 * 5.6),
                        paper.width() / 10 * 8,
                        paper.height() / 2);
        ReportElements::drawTable(painter, mdlTable, rectTable, QList<int>() << 3 << 1,
                                  ReportElements::Table::tvsCompressed, 10, -1, QFont::Bold);
    }
}

void StabSignalsTestWidget::printTwoLandscape(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper)
{
    if (areasesSKG.size() == 2)
    {
        ReportElements::drawWidget(painter, areasesSKG.at(0),
                                   static_cast<int>(paper.width() * 0.45), static_cast<int>(paper.height() * 0.45),
                                   paper.x() + paper.width() / 4 - static_cast<int>(paper.width() * 0.2),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));
        ReportElements::drawWidget(painter, areasesSKG.at(1),
                                   static_cast<int>(paper.width() * 0.45), static_cast<int>(paper.height() * 0.45),
                                   paper.x() + paper.width() / 4 * 3 - static_cast<int>(paper.width() * 0.2),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));

        QRect rectTableRF(static_cast<int>(paper.x() + paper.width() / 12),
                        static_cast<int>(paper.y() + paper.height() / 10 * 6.5),
                        static_cast<int>(paper.width() / 10 * 4),
                        static_cast<int>(paper.height() * 0.15));
        ReportElements::drawTable(painter, mdlRF, rectTableRF, QList<int>() << 5 << 2,
                                  ReportElements::Table::tvsStretched, 10, -1, QFont::Bold);

        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(testUid, ti))
        {
            bool isRomb = isRombergTest(ti);
            if (isRomb)
            {
                ReportElements::drawWidget(painter, tvRombergNorms,
                                           static_cast<int>(paper.width() * 0.45), static_cast<int>(paper.height() * 0.25),
                                           static_cast<int>(paper.x() + paper.width() / 10 * 5),
                                           static_cast<int>(paper.y() + paper.height() / 10 * 8));
            }
        }

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));
        printer->newPage();

        ReportElements::drawWidget(painter, wgtGraph,
                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85),
                                   paper.x() + paper.width() / 12,
                                   static_cast<int>(paper.y() + paper.height() / 10));

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));
        printer->newPage();

        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10),
                        paper.width() / 10 * 8,
                        paper.height() / 2);
        ReportElements::drawTable(painter, mdlTable, rectTable, QList<int>() << 3 << 1,
                                  ReportElements::Table::tvsCompressed, 10, -1, QFont::Bold);
    }
}

void StabSignalsTestWidget::printThreePortrait(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper)
{
    if (areasesSKG.size() == 3)
    {
        ReportElements::drawWidget(painter, areasesSKG.at(0),
                                   static_cast<int>(paper.width() * 0.3), static_cast<int>(paper.height() * 0.3),
                                   static_cast<int>(paper.x() + paper.width() / 6 * 1.2 - paper.width() * 0.15),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));
        ReportElements::drawWidget(painter, areasesSKG.at(1),
                                   static_cast<int>(paper.width() * 0.3), static_cast<int>(paper.height() * 0.3),
                                   paper.x() + paper.width() / 6 * 3 - static_cast<int>(paper.width() * 0.15),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));
        ReportElements::drawWidget(painter, areasesSKG.at(2),
                                   static_cast<int>(paper.width() * 0.3), static_cast<int>(paper.height() * 0.3),
                                   static_cast<int>(paper.x() + paper.width() / 6 * 4.8 - paper.width() * 0.15),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));

        QRect rectTableRF(static_cast<int>(paper.x() + paper.width() / 10),
                        static_cast<int>(paper.y() + paper.height() / 10 * 4.2),
                        paper.width() / 10 * 8,
                        static_cast<int>(paper.height() * 0.12));
        ReportElements::drawTable(painter, mdlRF, rectTableRF, QList<int>() << 2 << 2 << 2,
                                  ReportElements::Table::tvsStretched, 10, -1, QFont::Bold);

        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10 * 5.6),
                        paper.width() / 10 * 8,
                        paper.height() / 2);
        ReportElements::drawTable(painter, mdlTable, rectTable, QList<int>() << 5 << 1 << 2 << 1,
                                  ReportElements::Table::tvsCompressed, 10, -1, QFont::Bold);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));

        printer->newPage();

        ReportElements::drawWidget(painter, wgtGraph,
                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85),
                                   paper.x() + paper.width() / 12,
                                   static_cast<int>(paper.y() + paper.height() / 10));
    }
}

void StabSignalsTestWidget::printThreeLandscape(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper)
{
    if (areasesSKG.size() == 3)
    {
        ReportElements::drawWidget(painter, areasesSKG.at(0),
                                   static_cast<int>(paper.width() * 0.3), static_cast<int>(paper.height() * 0.5),
                                   static_cast<int>(paper.x() + paper.width() / 6 * 1.2 - paper.width() * 0.15),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));
        ReportElements::drawWidget(painter, areasesSKG.at(1),
                                   static_cast<int>(paper.width() * 0.3), static_cast<int>(paper.height() * 0.5),
                                   paper.x() + paper.width() / 6 * 3 - static_cast<int>(paper.width() * 0.15),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));
        ReportElements::drawWidget(painter, areasesSKG.at(2),
                                   static_cast<int>(paper.width() * 0.3), static_cast<int>(paper.height() * 0.5),
                                   static_cast<int>(paper.x() + paper.width() / 6 * 4.8 - paper.width() * 0.15),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));

        QRect rectTableRF(static_cast<int>(paper.x() + paper.width() / 12),
                        static_cast<int>(paper.y() + paper.height() / 10 * 7),
                        static_cast<int>(paper.width() / 10 * 8),
                        static_cast<int>(paper.height() * 0.15));
        ReportElements::drawTable(painter, mdlRF, rectTableRF, QList<int>() << 2 << 2 << 2 ,
                                  ReportElements::Table::tvsStretched, 10, -1, QFont::Bold);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));
        printer->newPage();

        ReportElements::drawWidget(painter, wgtGraph,
                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85),
                                   paper.x() + paper.width() / 12,
                                   static_cast<int>(paper.y() + paper.height() / 10));

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));
        printer->newPage();

        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10),
                        paper.width() / 10 * 8,
                        paper.height() / 2);
        ReportElements::drawTable(painter, mdlTable, rectTable, QList<int>() << 3 << 1 << 1,
                                  ReportElements::Table::tvsCompressed, 10, -1, QFont::Bold);
    }
}

void StabSignalsTestWidget::printFivePortrait(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper)
{
    if (areasesSKG.size() == 5)
    {
        ReportElements::drawWidget(painter, areasesSKG.at(0),
                                   static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.2),
                                   static_cast<int>(paper.x() + paper.width() / 10 * 2 - paper.width() * 0.1),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));
        ReportElements::drawWidget(painter, areasesSKG.at(1),
                                   static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.2),
                                   static_cast<int>(paper.x() + paper.width() / 10 * 5 - paper.width() * 0.1),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));
        ReportElements::drawWidget(painter, areasesSKG.at(2),
                                   static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.2),
                                   static_cast<int>(paper.x() + paper.width() / 10 * 8 - paper.width() * 0.1),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.5));
        ReportElements::drawWidget(painter, areasesSKG.at(3),
                                   static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.2),
                                   static_cast<int>(paper.x() + paper.width() / 10 * 3.5 - paper.width() * 0.1),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 3.21));
        ReportElements::drawWidget(painter, areasesSKG.at(4),
                                   static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.2),
                                   static_cast<int>(paper.x() + paper.width() / 10 * 6.5 - paper.width() * 0.1),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 3.21));

        QRect rectTableRF(static_cast<int>(paper.x() + paper.width() / 10),
                        static_cast<int>(paper.y() + paper.height() / 10 * 5.1),
                        paper.width() / 10 * 8,
                        static_cast<int>(paper.height() * 0.12));
        ReportElements::drawTable(painter, mdlRF, rectTableRF, QList<int>() << 2 << 2 << 2 << 2 << 2,
                                  ReportElements::Table::tvsStretched, 8, -1, QFont::Thin);

        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10 * 6.4),
                        paper.width() / 10 * 8,
                        paper.height() / 2);
        ReportElements::drawTable(painter, mdlTable, rectTable, QList<int>() << 5 << 1 << 1 << 1 << 1 << 1,
                                  ReportElements::Table::tvsCompressed, 8, -1, QFont::Thin);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));

        printer->newPage();

        ReportElements::drawWidget(painter, wgtGraph,
                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85),
                                   paper.x() + paper.width() / 12,
                                   static_cast<int>(paper.y() + paper.height() / 10));
    }
}

void StabSignalsTestWidget::printFiveLandscape(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper)
{
    if (areasesSKG.size() == 5)
    {
        ReportElements::drawWidget(painter, areasesSKG.at(0),
                                   static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.3),
                                   static_cast<int>(paper.x() + paper.width() / 10 * 2 - paper.width() * 0.1),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.4));
        ReportElements::drawWidget(painter, areasesSKG.at(1),
                                   static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.3),
                                   static_cast<int>(paper.x() + paper.width() / 10 * 5 - paper.width() * 0.1),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.4));
        ReportElements::drawWidget(painter, areasesSKG.at(2),
                                   static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.3),
                                   static_cast<int>(paper.x() + paper.width() / 10 * 8 - paper.width() * 0.1),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 1.4));
        ReportElements::drawWidget(painter, areasesSKG.at(3),
                                   static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.3),
                                   static_cast<int>(paper.x() + paper.width() / 10 * 3.5 - paper.width() * 0.1),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 4.5));
        ReportElements::drawWidget(painter, areasesSKG.at(4),
                                   static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.3),
                                   static_cast<int>(paper.x() + paper.width() / 10 * 6.5 - paper.width() * 0.1),
                                   static_cast<int>(paper.y() + paper.height() / 10 * 4.5));

        QRect rectTableRF(static_cast<int>(paper.x() + paper.width() / 12),
                        static_cast<int>(paper.y() + paper.height() / 10 * 8),
                        static_cast<int>(paper.width() / 10 * 8),
                        static_cast<int>(paper.height() * 0.15));
        ReportElements::drawTable(painter, mdlRF, rectTableRF, QList<int>() << 2 << 2 << 2 << 2 << 2 ,
                                  ReportElements::Table::tvsStretched, 10, -1, QFont::Bold);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));
        printer->newPage();

        ReportElements::drawWidget(painter, wgtGraph,
                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85),
                                   paper.x() + paper.width() / 12,
                                   static_cast<int>(paper.y() + paper.height() / 10));

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));
        printer->newPage();

        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10),
                        paper.width() / 10 * 8,
                        paper.height() / 2);
        ReportElements::drawTable(painter, mdlTable, rectTable, QList<int>() << 3 << 1 << 1 << 1 << 1,
                                  ReportElements::Table::tvsCompressed, 10, -1, QFont::Bold);
    }
}

