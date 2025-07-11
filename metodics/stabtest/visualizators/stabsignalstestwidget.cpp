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
#include "rombergkoefvaluedelegate.h"
#include "reportelements.h"
#include "stabtestvisualize.h"
#include "stabtestdefines.h"

#include <QTimer>
#include <QJsonArray>
#include <QJsonObject>
#include <QPainter>
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
    m_wgtTab = ui->tabWidget;

    showTable(calculator, testUid);
    showSKG(calculator, testUid);
    showRationalTable(calculator, testUid);
    showRombergNorms(calculator, testUid);
    restoreSplitterPosition();
}

void StabSignalsTestWidget::print(QPrinter *printer, const QString &testUid)
{
    //! Получаем указатель на экземпляр визуализатора
    auto vis = static_cast<AAnalyserApplication*>(QCoreApplication::instance())->getOpenedTest(testUid);
    StabTestVisualize* visualPanel = static_cast<StabTestVisualize*>(vis);
    StabSignalsTestWidget* visual = static_cast<StabSignalsTestWidget*>(visualPanel->widget());

    int tab = visual->m_wgtTab->currentIndex();
    for (int i = 0; i < visual->m_wgtTab->count(); ++i)
        visual->m_wgtTab->setCurrentIndex(i);
    visual->m_wgtTab->setCurrentIndex(tab);

    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
        if (visual->m_mdlTable.columnCount() - 1 == 1)
            printOnePortrait(printer, painter, testUid, paper, visual);
        else
        if (visual->m_mdlTable.columnCount() - 1 == 2)
            printTwoPortrait(printer, painter, testUid, paper, visual);
        else
        if (visual->m_mdlTable.columnCount() - 1 == 3)
            printThreePortrait(printer, painter, testUid, paper, visual);
        else
        if (visual->m_mdlTable.columnCount() - 1 == 5)
            printFivePortrait(printer, painter, testUid, paper, visual);

    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        if (visual->m_mdlTable.columnCount() - 1 == 1)
            printOneLandscape(printer, painter, testUid, paper, visual);
        else
        if (visual->m_mdlTable.columnCount() - 1 == 2)
            printTwoLandscape(printer, painter, testUid, paper, visual);
        else
        if (visual->m_mdlTable.columnCount() - 1 == 3)
            printThreeLandscape(printer, painter, testUid, paper, visual);
        else
        if (visual->m_mdlTable.columnCount() - 1 == 5)
            printFiveLandscape(printer, painter, testUid, paper, visual);
    }

    //! Нижний колонтитул
    ReportElements::drawFooter(painter, testUid, rectFooter(paper));

    painter->end();
}

void StabSignalsTestWidget::paintPreview(QPainter *painter, QRect &rect, const QString &testUid, TestCalculator *calculator)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        bool itr = isRombergTest(ti);
        StabSignalsTestCalculator* calc = static_cast<StabSignalsTestCalculator*>(calculator);
        if (itr && calc->probesCount() == 2)   //! Тест Ромберга
            paintPreviewRomberg(painter, rect, testUid, calc);
        else
        if (calc->probesCount() == 3)          //! Тест с тремя пробами, например, поворот головы
            paintPreviewThree(painter, rect, testUid, calc);
        else
        if (calc->probesCount() == 5)          //! Тест с пятью пробами, например, наклон головы, ОКП
            paintPreviewFive(painter, rect, testUid, calc);
    }
}

void StabSignalsTestWidget::setVisible(bool visible)
{
    QWidget::setVisible(visible);

    if (visible)
    {
        resizeColumnsTable(m_mdlRF, ui->tvRationalFactors, false);// true);
        resizeColumnsTable(m_mdlNorms, ui->tvRombergNorms, false);
        setDiapazones();
    }
}

void StabSignalsTestWidget::resizeEvent(QResizeEvent *event)
{
    resizeColumnsTable(m_mdlNorms, ui->tvRombergNorms, false);
    resizeColumnsTable(m_mdlRF, ui->tvRationalFactors, false);//true);
    QWidget::resizeEvent(event);
}

void StabSignalsTestWidget::showEvent(QShowEvent *event)
{
    resizeColumnsTable(m_mdlNorms, ui->tvRombergNorms, false);
    resizeColumnsTable(m_mdlRF, ui->tvRationalFactors, false);//true);
    QWidget::showEvent(event);
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
}

void StabSignalsTestWidget::showRationalTable(StabSignalsTestCalculator *calculator, const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        bool itr = isRombergTest(ti);
        QString nameTRS = tr("Площадь эллипса");
        QString nameTRKFR = tr("КФР");
        if (itr)
        {
            nameTRS = tr("Коэффициент Ромберга по площади эллипса");
            nameTRKFR = tr("Коэффициент Ромберга по КФР");
        }
        m_mdlRF = new QStandardItemModel(this);
        QStandardItem* itemFctS = new QStandardItem(nameTRS);
        QStandardItem* itemFctKFR = new QStandardItem(nameTRKFR);
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
                DataDefines::NormSideValue nsv = DataDefines::nsvMissing;
                if (itr)
                    itemS = new QStandardItem(getKoefRombResume(rationalFactors->factorValue(i), fi.format(), nsv));
                else
                    itemS = new QStandardItem(QString::number(rationalFactors->factorValue(i), 'f', fi.format()));
                itemS->setData(nsv, StabTestDefines::NormRole);
                itemS->setData(rationalFactors->factorValue(i), StabTestDefines::ValueRole);
                itemS->setData(fi.format(), StabTestDefines::FormatRole);
            }
            else
            if (i % 2 == 1)
            {
                DataDefines::NormSideValue nsv = DataDefines::nsvMissing;
                QStandardItem* itemKFR = nullptr;
                if (itr)
                    itemKFR = new QStandardItem(getKoefRombResume(rationalFactors->factorValue(i), fi.format(), nsv));
                else
                    itemKFR = new QStandardItem(QString::number(rationalFactors->factorValue(i), 'f', fi.format()));
                itemKFR->setData(nsv, StabTestDefines::NormRole);
                itemKFR->setData(rationalFactors->factorValue(i), StabTestDefines::ValueRole);
                itemKFR->setData(fi.format(), StabTestDefines::FormatRole);

                QStandardItem* itemMoX = createItemRationalFactors(calculator, i, ClassicFactorsDefines::MoXUid, 1, 'x');
                QStandardItem* itemMoY = createItemRationalFactors(calculator, i, ClassicFactorsDefines::MoYUid, 1, 'y');
                QStandardItem* itemQX = createItemRationalFactors(calculator, i, ClassicFactorsDefines::QXUid, 0, 'x');
                QStandardItem* itemQY = createItemRationalFactors(calculator, i, ClassicFactorsDefines::QYUid, 0, 'y');

                if (itemS)
                    appendColumnReadOnly(m_mdlRF,
                                         QList<QStandardItem*>()
                                         << itemS << itemKFR
                                         << itemMoX << itemMoY << itemQX << itemQY);
            }

            ui->tvRationalFactors->setItemDelegateForColumn(i+1, new RombergKoefValueDelegate(ui->tvRationalFactors));
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
        ui->tvRationalFactors->header()->setDefaultAlignment(Qt::AlignHCenter);
//        ui->tvRationalFactors->setItemDelegateForColumn(1, new RombergKoefValueDelegate(ui->tvRationalFactors));
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
                    item->setData(nv, StabTestDefines::NormRole);
                    item->setData(value, StabTestDefines::ValueRole);
                    item->setData(fi.format(), StabTestDefines::FormatRole);
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
            ui->tvRombergNorms->header()->setDefaultAlignment(Qt::AlignHCenter);
        }
    }
    m_tvRombergNorms = ui->tvRombergNorms;
}

DataDefines::NormValue StabSignalsTestWidget::getRombergNorm(const int probeNum,
                                                             const QString &factorUid,
                                                             const double value)
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

QString StabSignalsTestWidget::getKoefRombResume(const double value, const int format, DataDefines::NormSideValue& nsv) const
{
    QString sVal = QString::number(value, 'f', format);
    if (value >= 100 && value <= 250)
    {
        nsv = DataDefines::nsvNorm;
        return QString("%1 - " + tr("В норме") + " (100 - 250)").arg(sVal);
    }
    else
    if (value < 100)
    {
        nsv = DataDefines::nsvBelow;
        return QString("%1 - " + tr("Ниже нормы")).arg(sVal);
//        return QString("%1 - " + tr("Ниже нормы (Постуральная слепота)")).arg(sVal);
    }
    else
    {
        nsv = DataDefines::nsvAbove;
        return QString("%1 - " + tr("Выше нормы")).arg(sVal);
//        return QString("%1 - " + tr("Выше нормы (Держится за счет зрения)")).arg(sVal);
    }
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
    m_areasesSKG.clear();


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
                m_areasesSKG << skg;
                skg->setVisibleMarker(false);
                skg->setTitle(pi.name);

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
                    ui->wgtGraph->setLegend(i, QStringList() << tr("Фронталь") << tr("Сагитталь"));
                }
            }
        }
    }

    m_wgtGraph = ui->wgtGraph;
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
    foreach (auto* skg, m_areasesSKG)
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
//    SettingsProvider::setValueToRegAppCopy("Geometry/StabSignalsTestWidget", "SplitterPosition", ui->splitter->saveState());
    QString sSecName = "Geometry/StabSignalsTestWidget_" + QString::number(m_mdlTable.columnCount());
    SettingsProvider::setValueToRegAppCopy(sSecName, "AreaSKGSplitterPosition", ui->splAreaSKG->saveState());
}

void StabSignalsTestWidget::restoreSplitterPosition()
{
//    auto val = SettingsProvider::valueFromRegAppCopy("Geometry/StabSignalsTestWidget", "SplitterPosition").toByteArray();
    QString sSecName = "Geometry/StabSignalsTestWidget_" + QString::number(m_mdlTable.columnCount());
    auto val = SettingsProvider::valueFromRegAppCopy(sSecName, "AreaSKGSplitterPosition").toByteArray();
    ui->splAreaSKG->restoreState(val);
}

void StabSignalsTestWidget::resizeColumnsTable(QStandardItemModel *mdl, QTreeView* tv, const bool toContens)
{
    if (mdl && mdl->columnCount() > 1)
    {
        int w = static_cast<int>(static_cast<double>(tv->geometry().width() - tv->header()->sectionSize(0)) /
                                 static_cast<double>(mdl->columnCount() - 1)) - 1;
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

void StabSignalsTestWidget::printOnePortrait(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual)
{
    Q_UNUSED(testUid);

    if (visual->m_areasesSKG.size() == 1)
    {
        //! СКГ
        auto rectSKG = QRect(paper.x() + paper.width() / 2 - static_cast<int>(paper.width() * 0.3),
                             static_cast<int>(paper.y() + paper.height() / 10 * 1.5),
                             static_cast<int>(paper.width() * 0.6),
                             static_cast<int>(paper.height() * 0.37));
        double ratio = ReportElements::ratio(paper, visual->m_areasesSKG.at(0));
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio);
    }

    QRect rectTable(paper.x() + paper.width() / 10,
                    static_cast<int>(paper.y() + paper.height() / 10 * 5.5),
                    paper.width() / 10 * 8,
                    paper.height() / 2);
    ReportElements::drawTable(painter, &visual->m_mdlTable, rectTable, QList<int>() << 3 << 1, false,
                              ReportElements::Table::tvsCompressed, 10, -1, QFont::Bold);

    //! Нижний колонтитул
    ReportElements::drawFooter(painter, testUid, rectFooter(paper));

    printer->newPage();

    auto rectGraph = QRect(paper.x() + paper.width() / 12,
                           static_cast<int>(paper.y() + paper.height() / 10),
                           static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.5));
    double ratio = ReportElements::ratio(paper, visual->m_wgtGraph);
    ReportElements::drawGraph(painter, rectGraph, testUid, 0, ratio);
}

void StabSignalsTestWidget::printOneLandscape(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual)
{
    Q_UNUSED(testUid);

    if (visual->m_areasesSKG.size() == 1)
    {
        //! СКГ
        auto rectSKG = QRect(paper.x() - paper.width() / 35, static_cast<int>(paper.y() + paper.height() / 10 * 2.5),
                             static_cast<int>(paper.width() * 0.55), static_cast<int>(paper.height() * 0.55));
        double ratio = ReportElements::ratio(paper, visual->m_areasesSKG.at(0));
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio);
    }

    QRect rectTable(static_cast<int>(paper.x() + paper.width() / 10 * 4.5),
                    static_cast<int>(paper.y() + paper.height() / 10 * 2),
                    paper.width() / 10 * 5,
                    static_cast<int>(paper.height() * 0.7));
    ReportElements::drawTable(painter, &visual->m_mdlTable, rectTable, QList<int>() << 5 << 2, false,
                              ReportElements::Table::tvsStretched, 9, -1, QFont::Bold);

    //! Нижний колонтитул
    ReportElements::drawFooter(painter, testUid, rectFooter(paper));

    printer->newPage();

    auto rectGraph = QRect(paper.x() + paper.width() / 12,
                           static_cast<int>(paper.y() + paper.height() / 15),
                           static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85));
    double ratio = ReportElements::ratio(paper, visual->m_wgtGraph);
    ReportElements::drawGraph(painter, rectGraph, testUid, 0, ratio);
}

void StabSignalsTestWidget::printTwoPortrait(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual)
{
    if (visual->m_areasesSKG.size() == 2)
    {
        auto rectSKG = QRect(paper.x() + paper.width() / 4 - static_cast<int>(paper.width() * 0.2),
                             static_cast<int>(paper.y() + paper.height() / 10),
                             static_cast<int>(paper.width() * 0.43), static_cast<int>(paper.height() * 0.43));
        double ratio = ReportElements::ratio(paper, visual->m_areasesSKG.at(0), 5);
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio, visual->m_areasesSKG.at(0)->diap());

        rectSKG = QRect(paper.x() + paper.width() / 4 * 3 - static_cast<int>(paper.width() * 0.25),
                        static_cast<int>(paper.y() + paper.height() / 10),
                        static_cast<int>(paper.width() * 0.43), static_cast<int>(paper.height() * 0.43));
        if (DataProvider::channelExists(testUid, 1, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 1, ratio, visual->m_areasesSKG.at(1)->diap());

        QRect rectTableRF(static_cast<int>(paper.x() + paper.width() / 10),
                          static_cast<int>(paper.y() + paper.height() / 10 * 5),
                          paper.width() / 10 * 8,
                          static_cast<int>(paper.height() * 0.15));
        ReportElements::drawTable(painter, visual->m_mdlRF, rectTableRF, QList<int>() << 5 << 3, false,
                                  ReportElements::Table::tvsStretched, 10, -1, QFont::Bold);

        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(testUid, ti))
        {
            bool isRomb = isRombergTest(ti);
            if (isRomb)
            {
                ReportElements::drawWidget(painter, visual->m_tvRombergNorms,
                                           static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.25),
                                           paper.x() + paper.width() / 10,
                                           static_cast<int>(paper.y() + paper.height() / 10 * 6.5));
            }
        }

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));

        printer->newPage();

        painter->setFont(QFont("Arial", 8, 0, false));
        auto pn = DataProvider::getProbesNames(testUid);
        if (pn.size() < 2) return;
        painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.04), pn.at(0));
        auto rectGraph = QRect(paper.x() + paper.width() / 12,
                               static_cast<int>(paper.y() + paper.height() * 0.045),
                               static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.23));
        ratio = ReportElements::ratio(paper, visual->m_wgtGraph);
        ReportElements::drawGraph(painter, rectGraph, testUid, 0, ratio);
        painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.295), pn.at(1));
        rectGraph = QRect(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() * 0.30),
                          static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.23));
        ReportElements::drawGraph(painter, rectGraph, testUid, 1, ratio);

        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10 * 5.6),
                        paper.width() / 10 * 8,
                        paper.height() / 2);
        ReportElements::drawTable(painter, &visual->m_mdlTable, rectTable, QList<int>() << 3 << 1, false,
                                  ReportElements::Table::tvsCompressed, 10, -1, QFont::Bold);
    }
}

void StabSignalsTestWidget::printTwoLandscape(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual)
{
    if (visual->m_areasesSKG.size() == 2)
    {
        auto rectSKG = QRect(paper.x() + paper.width() / 4 - static_cast<int>(paper.width() * 0.2),
                             static_cast<int>(paper.y() + paper.height() / 10 * 1.5),
                             static_cast<int>(paper.width() * 0.47), static_cast<int>(paper.height() * 0.47));
        double ratio = ReportElements::ratio(paper, visual->m_areasesSKG.at(0), 5);
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio, visual->m_areasesSKG.at(0)->diap());

        rectSKG = QRect(paper.x() + paper.width() / 4 * 3 - static_cast<int>(paper.width() * 0.25),
                        static_cast<int>(paper.y() + paper.height() / 10 * 1.5),
                        static_cast<int>(paper.width() * 0.47), static_cast<int>(paper.height() * 0.47));
        if (DataProvider::channelExists(testUid, 1, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 1, ratio, visual->m_areasesSKG.at(1)->diap());

        QRect rectTableRF(static_cast<int>(paper.x() + paper.width() / 12),
                        static_cast<int>(paper.y() + paper.height() / 10 * 6.5),
                        static_cast<int>(paper.width() / 10 * 4),
                        static_cast<int>(paper.height() * 0.15));
        ReportElements::drawTable(painter, visual->m_mdlRF, rectTableRF, QList<int>() << 5 << 2, false,
                                  ReportElements::Table::tvsStretched, 10, -1, QFont::Bold);

        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(testUid, ti))
        {
            bool isRomb = isRombergTest(ti);
            if (isRomb)
            {
                ReportElements::drawWidget(painter, visual->m_tvRombergNorms,
                                           static_cast<int>(paper.width() * 0.45), static_cast<int>(paper.height() * 0.25),
                                           static_cast<int>(paper.x() + paper.width() / 10 * 2.5),
                                           static_cast<int>(paper.y() + paper.height() / 10 * 8));
            }
        }

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));
        printer->newPage();

        painter->setFont(QFont("Arial", 8, 0, false));
        auto pn = DataProvider::getProbesNames(testUid);
        if (pn.size() < 2) return;
        painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.04), pn.at(0));
        auto rectGraph = QRect(paper.x() + paper.width() / 12,
                               static_cast<int>(paper.y() + paper.height() * 0.05),
                               static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.41));
        ratio = ReportElements::ratio(paper, visual->m_wgtGraph);
        ReportElements::drawGraph(painter, rectGraph, testUid, 0, ratio);
        painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.49), pn.at(1));
        rectGraph = QRect(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() * 0.50),
                          static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.41));
        ratio = ReportElements::ratio(paper, visual->m_wgtGraph);
        ReportElements::drawGraph(painter, rectGraph, testUid, 1, ratio);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));
        printer->newPage();

        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10),
                        paper.width() / 10 * 8,
                        paper.height() / 2);
        ReportElements::drawTable(painter, &visual->m_mdlTable, rectTable, QList<int>() << 3 << 1, false,
                                  ReportElements::Table::tvsCompressed, 10, -1, QFont::Bold);
    }
}

void StabSignalsTestWidget::printThreePortrait(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual)
{
    if (visual->m_areasesSKG.size() == 3)
    {
        auto rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 6 * 1.2 - paper.width() * 0.15),
                             static_cast<int>(paper.y() + paper.height() / 10),
                             static_cast<int>(paper.width() * 0.29), static_cast<int>(paper.height() * 0.29));
        double ratio = ReportElements::ratio(paper, visual->m_areasesSKG.at(0), 5);
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio, visual->m_areasesSKG.at(0)->diap());

        rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 6 * 3 - paper.width() * 0.15),
                        static_cast<int>(paper.y() + paper.height() / 10),
                        static_cast<int>(paper.width() * 0.29), static_cast<int>(paper.height() * 0.29));
        if (DataProvider::channelExists(testUid, 1, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 1, ratio, visual->m_areasesSKG.at(0)->diap());

        rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 6 * 4.8 - paper.width() * 0.15),
                        static_cast<int>(paper.y() + paper.height() / 10),
                        static_cast<int>(paper.width() * 0.29), static_cast<int>(paper.height() * 0.29));
        if (DataProvider::channelExists(testUid, 2, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 2, ratio, visual->m_areasesSKG.at(0)->diap());

        QRect rectTableRF(static_cast<int>(paper.x() + paper.width() / 10),
                        static_cast<int>(paper.y() + paper.height() / 10 * 4.2),
                        paper.width() / 10 * 8,
                        static_cast<int>(paper.height() * 0.12));
        ReportElements::drawTable(painter, visual->m_mdlRF, rectTableRF, QList<int>() << 2 << 2 << 2, false,
                                  ReportElements::Table::tvsStretched, 10, -1, QFont::Bold);

        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10 * 5.6),
                        paper.width() / 10 * 8,
                        paper.height() / 2);
        ReportElements::drawTable(painter, &visual->m_mdlTable, rectTable, QList<int>() << 5 << 1 << 2 << 1, false,
                                  ReportElements::Table::tvsCompressed, 10, -1, QFont::Bold);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));

        printer->newPage();

        painter->setFont(QFont("Arial", 8, 0, false));
        auto pn = DataProvider::getProbesNames(testUid);
        if (pn.size() < 3) return;
        painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.04), pn.at(0));
        auto rectGraph = QRect(paper.x() + paper.width() / 12,
                               static_cast<int>(paper.y() + paper.height() * 0.05),
                               static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.28));
        ratio = ReportElements::ratio(paper, visual->m_wgtGraph);
        ReportElements::drawGraph(painter, rectGraph, testUid, 0, ratio);

        painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.345), pn.at(1));
        rectGraph = QRect(paper.x() + paper.width() / 12,
                               static_cast<int>(paper.y() + paper.height() * 0.35),
                               static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.28));
        ReportElements::drawGraph(painter, rectGraph, testUid, 1, ratio);

        painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.645), pn.at(2));
        rectGraph = QRect(paper.x() + paper.width() / 12,
                               static_cast<int>(paper.y() + paper.height() * 0.65),
                               static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.28));
        ReportElements::drawGraph(painter, rectGraph, testUid, 2, ratio);
    }
}

void StabSignalsTestWidget::printThreeLandscape(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual)
{
    if (visual->m_areasesSKG.size() == 3)
    {
        auto rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 6 * 1.2 - paper.width() * 0.15),
                             static_cast<int>(paper.y() + paper.height() / 10 * 1.5),
                             static_cast<int>(paper.width() * 0.29), static_cast<int>(paper.height() * 0.5));
        double ratio = ReportElements::ratio(paper, visual->m_areasesSKG.at(0), 5);
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio, visual->m_areasesSKG.at(0)->diap());

        rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 6 * 3 - paper.width() * 0.15),
                        static_cast<int>(paper.y() + paper.height() / 10 * 1.5),
                        static_cast<int>(paper.width() * 0.29), static_cast<int>(paper.height() * 0.5));
        if (DataProvider::channelExists(testUid, 1, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 1, ratio, visual->m_areasesSKG.at(1)->diap());

        rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 6 * 4.8 - paper.width() * 0.15),
                        static_cast<int>(paper.y() + paper.height() / 10 * 1.5),
                        static_cast<int>(paper.width() * 0.29), static_cast<int>(paper.height() * 0.5));
        if (DataProvider::channelExists(testUid, 2, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 2, ratio, visual->m_areasesSKG.at(2)->diap());

        QRect rectTableRF(static_cast<int>(paper.x() + paper.width() / 12),
                        static_cast<int>(paper.y() + paper.height() / 10 * 7),
                        static_cast<int>(paper.width() / 10 * 8),
                        static_cast<int>(paper.height() * 0.15));
        ReportElements::drawTable(painter, visual->m_mdlRF, rectTableRF, QList<int>() << 2 << 2 << 2, false,
                                  ReportElements::Table::tvsStretched, 10, -1, QFont::Bold);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));
        printer->newPage();

        painter->setFont(QFont("Arial", 8, 0, false));
        auto pn = DataProvider::getProbesNames(testUid);
        if (pn.size() < 3) return;
        painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.02), pn.at(0));
        auto rectGraph = QRect(static_cast<int>(paper.x() + paper.width() * 0.05),
                               static_cast<int>(paper.y() + paper.height() * 0.025),
                               static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.28));
        ratio = ReportElements::ratio(paper, visual->m_wgtGraph);
        ReportElements::drawGraph(painter, rectGraph, testUid, 0, ratio);

        painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.33), pn.at(1));
        rectGraph = QRect(static_cast<int>(paper.x() + paper.width() * 0.05),
                          static_cast<int>(paper.y() + paper.height() * 0.335),
                          static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.28));
        ratio = ReportElements::ratio(paper, visual->m_wgtGraph);
        ReportElements::drawGraph(painter, rectGraph, testUid, 1, ratio);

        painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.64), pn.at(2));
        rectGraph = QRect(static_cast<int>(paper.x() + paper.width() * 0.05),
                          static_cast<int>(paper.y() + paper.height() * 0.645),
                          static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.28));
        ratio = ReportElements::ratio(paper, visual->m_wgtGraph);
        ReportElements::drawGraph(painter, rectGraph, testUid, 2, ratio);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));
        printer->newPage();

        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10),
                        paper.width() / 10 * 8,
                        paper.height() / 2);
        ReportElements::drawTable(painter, &visual->m_mdlTable, rectTable, QList<int>() << 3 << 1 << 1, false,
                                  ReportElements::Table::tvsCompressed, 10, -1, QFont::Bold);
    }
}

void StabSignalsTestWidget::printFivePortrait(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual)
{
    if (visual->m_areasesSKG.size() == 5)
    {
        //! Получение списка проб
        auto nameProbes = getProbesNameList(testUid, 5);

        //! Вывод СКГ
        painter->setFont(QFont("Arial", 8, 0, false));
        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 2 - paper.width() * 0.1),
                          static_cast<int>(paper.y() + paper.height() / 10 * 1.5), "(1) " + nameProbes.at(0));
        auto rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 10 * 2 - paper.width() * 0.1),
                             static_cast<int>(paper.y() + paper.height() / 10 * 1.2),
                             static_cast<int>(paper.width() * 0.23), static_cast<int>(paper.height() * 0.23));
        double ratio = ReportElements::ratio(paper, visual->m_areasesSKG.at(0), 5);
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio, visual->m_areasesSKG.at(0)->diap());

        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 3.5 - paper.width() * 0.1),
                          static_cast<int>(paper.y() + paper.height() / 10 * 3.29), "(2) " + nameProbes.at(1));
        rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 10 * 3.5 - paper.width() * 0.1),
                        static_cast<int>(paper.y() + paper.height() / 10 * 2.99),
                        static_cast<int>(paper.width() * 0.23), static_cast<int>(paper.height() * 0.23));
        if (DataProvider::channelExists(testUid, 1, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 1, ratio, visual->m_areasesSKG.at(1)->diap());

        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 5 - paper.width() * 0.1),
                          static_cast<int>(paper.y() + paper.height() / 10 * 1.5),  "(3) " + nameProbes.at(2));
        rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 10 * 5 - paper.width() * 0.1),
                        static_cast<int>(paper.y() + paper.height() / 10 * 1.2),
                        static_cast<int>(paper.width() * 0.23), static_cast<int>(paper.height() * 0.23));
        if (DataProvider::channelExists(testUid, 2, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 2, ratio, visual->m_areasesSKG.at(2)->diap());

        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 6.5 - paper.width() * 0.1),
                          static_cast<int>(paper.y() + paper.height() / 10 * 3.29), "(4) " + nameProbes.at(3));
        rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 10 * 6.5 - paper.width() * 0.1),
                        static_cast<int>(paper.y() + paper.height() / 10 * 2.99),
                        static_cast<int>(paper.width() * 0.23), static_cast<int>(paper.height() * 0.23));
        if (DataProvider::channelExists(testUid, 3, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 3, ratio, visual->m_areasesSKG.at(3)->diap());

        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 8 - paper.width() * 0.1),
                          static_cast<int>(paper.y() + paper.height() / 10 * 1.5),  "(5) " + nameProbes.at(4));
        rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 10 * 8 - paper.width() * 0.1),
                        static_cast<int>(paper.y() + paper.height() / 10 * 1.2),
                        static_cast<int>(paper.width() * 0.23), static_cast<int>(paper.height() * 0.23));
        if (DataProvider::channelExists(testUid, 4, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 4, ratio, visual->m_areasesSKG.at(4)->diap());

        QRect rectTableRF(static_cast<int>(paper.x() + paper.width() / 10),
                        static_cast<int>(paper.y() + paper.height() / 10 * 5.1),
                        paper.width() / 10 * 8,
                        static_cast<int>(paper.height() * 0.12));
        ReportElements::drawTable(painter, visual->m_mdlRF, rectTableRF, QList<int>() << 2 << 2 << 2 << 2 << 2, false,
                                  ReportElements::Table::tvsStretched, 8, -1, QFont::Thin);

        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10 * 6.4),
                        paper.width() / 10 * 8,
                        paper.height() / 2);
        ReportElements::drawTable(painter, &visual->m_mdlTable, rectTable, QList<int>() << 5 << 1 << 1 << 1 << 1 << 1, true,
                                  ReportElements::Table::tvsCompressed, 8, -1, QFont::Thin);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));

        printer->newPage();

        //! Печать графиков
        printGraphFive(painter, testUid, paper, visual);
    }
}

void StabSignalsTestWidget::printFiveLandscape(QPrinter *printer, QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual)
{
    if (visual->m_areasesSKG.size() == 5)
    {
        //! Получение списка проб
        auto nameProbes = getProbesNameList(testUid, 5);

        painter->setFont(QFont("Arial", 8, 0, false));
        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 2 - paper.width() * 0.1),
                          static_cast<int>(paper.y() + paper.height() / 10 * 1.6), "(1) " + nameProbes.at(0));
        auto rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 10 * 2 - paper.width() * 0.1),
                             static_cast<int>(paper.y() + paper.height() / 10 * 1.6),
                             static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.3));
        double ratio = ReportElements::ratio(paper, visual->m_areasesSKG.at(0), 5);
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio, visual->m_areasesSKG.at(0)->diap());

        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 3.5 - paper.width() * 0.1),
                          static_cast<int>(paper.y() + paper.height() / 10 * 4.7), "(2) " + nameProbes.at(1));
        rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 10 * 3.5 - paper.width() * 0.1),
                        static_cast<int>(paper.y() + paper.height() / 10 * 4.7),
                        static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.3));
        if (DataProvider::channelExists(testUid, 1, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 1, ratio, visual->m_areasesSKG.at(1)->diap());

        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 5 - paper.width() * 0.1),
                          static_cast<int>(paper.y() + paper.height() / 10 * 1.6), "(3) " + nameProbes.at(2));
        rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 10 * 5 - paper.width() * 0.1),
                        static_cast<int>(paper.y() + paper.height() / 10 * 1.6),
                        static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.3));
        if (DataProvider::channelExists(testUid, 2, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 2, ratio, visual->m_areasesSKG.at(2)->diap());

        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 6.5 - paper.width() * 0.1),
                          static_cast<int>(paper.y() + paper.height() / 10 * 4.7), "(4) " + nameProbes.at(3));
        rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 10 * 6.5 - paper.width() * 0.1),
                        static_cast<int>(paper.y() + paper.height() / 10 * 4.7),
                        static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.3));
        if (DataProvider::channelExists(testUid, 3, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 3, ratio, visual->m_areasesSKG.at(3)->diap());

        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 8 - paper.width() * 0.1),
                          static_cast<int>(paper.y() + paper.height() / 10 * 1.6), "(5) " + nameProbes.at(4));
        rectSKG = QRect(static_cast<int>(paper.x() + paper.width() / 10 * 8 - paper.width() * 0.1),
                        static_cast<int>(paper.y() + paper.height() / 10 * 1.6),
                        static_cast<int>(paper.width() * 0.2), static_cast<int>(paper.height() * 0.3));
        if (DataProvider::channelExists(testUid, 4, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 4, ratio, visual->m_areasesSKG.at(4)->diap());

        QRect rectTableRF(static_cast<int>(paper.x() + paper.width() / 12),
                        static_cast<int>(paper.y() + paper.height() / 10 * 8),
                        static_cast<int>(paper.width() / 10 * 8),
                        static_cast<int>(paper.height() * 0.15));
        ReportElements::drawTable(painter, visual->m_mdlRF, rectTableRF, QList<int>() << 2 << 2 << 2 << 2 << 2, false,
                                  ReportElements::Table::tvsStretched, 10, -1, QFont::Bold);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));
        printer->newPage();

        //! Печать графиков
        printGraphFive(painter, testUid, paper, visual);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter(paper));
        printer->newPage();

        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10),
                        paper.width() / 10 * 8,
                        paper.height() / 2);
        ReportElements::drawTable(painter, &visual->m_mdlTable, rectTable, QList<int>() << 3 << 1 << 1 << 1 << 1, false,
                                  ReportElements::Table::tvsCompressed, 10, -1, QFont::Bold);
    }
}

void StabSignalsTestWidget::printGraphFive(QPainter *painter, const QString &testUid, const QRect paper, StabSignalsTestWidget* visual)
{
    painter->setFont(QFont("Arial", 8, 0, false));
    auto pn = DataProvider::getProbesNames(testUid);
    if (pn.size() < 5) return;
    painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.02), pn.at(0));
    auto rectGraph = QRect(static_cast<int>(paper.x() + paper.width() * 0.05),
                           static_cast<int>(paper.y() + paper.height() * 0.025),
                           static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.16));
    auto ratio = ReportElements::ratio(paper, visual->m_wgtGraph);
    ReportElements::drawGraph(painter, rectGraph, testUid, 0, ratio);

    painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.20), pn.at(1));
    rectGraph = QRect(static_cast<int>(paper.x() + paper.width() * 0.05),
                      static_cast<int>(paper.y() + paper.height() * 0.205),
                      static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.16));
    ReportElements::drawGraph(painter, rectGraph, testUid, 1, ratio);

    painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.38), pn.at(2));
    rectGraph = QRect(static_cast<int>(paper.x() + paper.width() * 0.05),
                      static_cast<int>(paper.y() + paper.height() * 0.385),
                      static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.16));
    ReportElements::drawGraph(painter, rectGraph, testUid, 2, ratio);

    painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.56), pn.at(3));
    rectGraph = QRect(static_cast<int>(paper.x() + paper.width() * 0.05),
                      static_cast<int>(paper.y() + paper.height() * 0.565),
                      static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.16));
    ReportElements::drawGraph(painter, rectGraph, testUid, 3, ratio);

    painter->drawText(paper.x() + paper.width() / 12, static_cast<int>(paper.y() + paper.height() * 0.74), pn.at(4));
    rectGraph = QRect(static_cast<int>(paper.x() + paper.width() * 0.05),
                      static_cast<int>(paper.y() + paper.height() * 0.745),
                      static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.16));
    ReportElements::drawGraph(painter, rectGraph, testUid, 4, ratio);
}

void StabSignalsTestWidget::paintPreviewRomberg(QPainter *painter, QRect &rect, const QString &testUid, StabSignalsTestCalculator *calculator)
{
    Q_UNUSED(testUid);
    auto fc1 = calculator->classicFactors(0);
    auto fc2 = calculator->classicFactors(1);
    auto fv1 = calculator->vectorFactors(0);
    auto fv2 = calculator->vectorFactors(1);
    auto EllS1Val = fc1->factorValue(ClassicFactorsDefines::SquareUid);
    auto EllS2Val = fc2->factorValue(ClassicFactorsDefines::SquareUid);
    auto KFR1Val = fv1->factorValue(VectorFactorsDefines::KFRUid);
    auto KFR2Val = fv2->factorValue(VectorFactorsDefines::KFRUid);
    double krs = EllS2Val / EllS1Val * 100;
    double krkfr = KFR2Val / KFR1Val * 100;
    QString sKRS = QString::number(krs, 'f', 0);
    QString sKRKFR = QString::number(krkfr, 'f', 0);

    painter->save();

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter->setFont(QFont("Sans", 6, QFont::Bold, false));
    painter->drawText(rect.x() + 5, rect.y() + 10, tr("Коэф Ромберга"));
    painter->setFont(QFont("Sans", 7, QFont::Bold, false));

    auto setPenColor = [&](const double value)
    {
        QColor col = Qt::darkGreen;
        if (value < 100)
            col = Qt::darkYellow;
        else
        if (value > 250)
            col = Qt::darkRed;
        painter->setPen(QPen(col, 1, Qt::SolidLine, Qt::FlatCap));
    };

    setPenColor(krs);
    painter->drawText(rect.x() + 5, rect.y() + 22, tr("S") + " (" + sKRS + "%)");
    setPenColor(krkfr);
    painter->drawText(rect.x() + 5, rect.y() + 34, tr("KFR") + " (" + sKRKFR + "%)");

    for (int i = 0; i < calculator->probesCount(); ++i)
    {

        auto drawNormPic = [&](const QString &factorUid, const int h)
        {
            double value = calculator->classicFactors(i)->factorValue(factorUid);
            auto nv = getRombergNorm(i, factorUid, value);

            QColor col = Qt::gray;
            if (nv == DataDefines::NotNormal)
                col = Qt::red;
            else
            if (nv == DataDefines::ConditionNormal)
                col = Qt::darkYellow;
            else
            if (nv == DataDefines::Normal)
                col = Qt::green;

            painter->setPen(QPen(BaseUtils::darkColor(col, 2), 1, Qt::SolidLine, Qt::FlatCap));
            QRadialGradient gradient(QPointF(rect.center().x() + 25 + 30 * i, h), 12, QPointF(rect.center().x() + 25 + 30 * i, h));
            gradient.setColorAt(0, col);
            gradient.setColorAt(1, BaseUtils::lightColor(col, 2));
            painter->setBrush(gradient);
            painter->drawEllipse(QPoint(rect.center().x() + 25 + 30 * i, h), 12, 3);
        };

        drawNormPic(ClassicFactorsDefines::QXUid, rect.y() + 8);
        drawNormPic(ClassicFactorsDefines::QYUid, rect.y() + 16);
        drawNormPic(ClassicFactorsDefines::LUid, rect.y() + 24);
        drawNormPic(ClassicFactorsDefines::SquareUid, rect.y() + 32);
    }

    painter->restore();
}

void StabSignalsTestWidget::paintPreviewThree(QPainter *painter, QRect &rect, const QString &testUid, StabSignalsTestCalculator *calculator)
{
    Q_UNUSED(testUid);
    painter->save();

    auto fc1 = calculator->classicFactors(0);
    auto fc2 = calculator->classicFactors(1);
    auto fc3 = calculator->classicFactors(2);
    auto fv1 = calculator->vectorFactors(0);
    auto fv2 = calculator->vectorFactors(1);
    auto fv3 = calculator->vectorFactors(2);
    auto EllS1Val = fc1->factorValueFormatted(ClassicFactorsDefines::SquareUid);
    auto EllS2Val = fc2->factorValueFormatted(ClassicFactorsDefines::SquareUid);
    auto EllS3Val = fc3->factorValueFormatted(ClassicFactorsDefines::SquareUid);
    auto KFR1Val = fv1->factorValueFormatted(VectorFactorsDefines::KFRUid);
    auto KFR2Val = fv2->factorValueFormatted(VectorFactorsDefines::KFRUid);
    auto KFR3Val = fv3->factorValueFormatted(VectorFactorsDefines::KFRUid);

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter->setFont(QFont("Sans", 7, QFont::Bold, false));

    painter->drawText(rect.x() + 30, rect.y() + 9, "(1)");
    painter->drawText(rect.x() + 70, rect.y() + 9, "(2)");
    painter->drawText(rect.x() + 110, rect.y() + 9, "(3)");

    painter->drawText(rect.x() + 2, rect.y() + 22, tr("S"));
    painter->drawText(rect.x() + 30, rect.y() + 22, EllS1Val);
    painter->drawText(rect.x() + 70, rect.y() + 22, EllS2Val);
    painter->drawText(rect.x() + 110, rect.y() + 22, EllS3Val);

    painter->drawText(rect.x() + 2, rect.y() + 35, tr("КФР"));
    painter->drawText(rect.x() + 30, rect.y() + 35, KFR1Val);
    painter->drawText(rect.x() + 70, rect.y() + 35, KFR2Val);
    painter->drawText(rect.x() + 110, rect.y() + 35, KFR3Val);

    painter->restore();
}

void StabSignalsTestWidget::paintPreviewFive(QPainter *painter, QRect &rect, const QString &testUid, StabSignalsTestCalculator *calculator)
{
    Q_UNUSED(testUid);
    painter->save();

    auto fc1 = calculator->classicFactors(0);
    auto fc2 = calculator->classicFactors(1);
    auto fc3 = calculator->classicFactors(2);
    auto fc4 = calculator->classicFactors(3);
//    auto fc5 = calculator->classicFactors(4);
    auto fv1 = calculator->vectorFactors(0);
    auto fv2 = calculator->vectorFactors(1);
    auto fv3 = calculator->vectorFactors(2);
    auto fv4 = calculator->vectorFactors(3);
    auto fv5 = calculator->vectorFactors(4);
    auto EllS1Val = fc1->factorValueFormatted(ClassicFactorsDefines::SquareUid);
    auto EllS2Val = fc2->factorValueFormatted(ClassicFactorsDefines::SquareUid);
    auto EllS3Val = fc3->factorValueFormatted(ClassicFactorsDefines::SquareUid);
    auto EllS4Val = fc3->factorValueFormatted(ClassicFactorsDefines::SquareUid);
    auto EllS5Val = fc4->factorValueFormatted(ClassicFactorsDefines::SquareUid);
    auto KFR1Val = fv1->factorValueFormatted(VectorFactorsDefines::KFRUid);
    auto KFR2Val = fv2->factorValueFormatted(VectorFactorsDefines::KFRUid);
    auto KFR3Val = fv3->factorValueFormatted(VectorFactorsDefines::KFRUid);
    auto KFR4Val = fv4->factorValueFormatted(VectorFactorsDefines::KFRUid);
    auto KFR5Val = fv5->factorValueFormatted(VectorFactorsDefines::KFRUid);

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter->setFont(QFont("Sans", 6, QFont::Bold, false));

    painter->drawText(rect.x() + 25, rect.y() + 9, "(1)");
    painter->drawText(rect.x() + 50, rect.y() + 9, "(2)");
    painter->drawText(rect.x() + 75, rect.y() + 9, "(3)");
    painter->drawText(rect.x() + 100, rect.y() + 9, "(4)");
    painter->drawText(rect.x() + 125, rect.y() + 9, "(5)");

    painter->drawText(rect.x() + 2, rect.y() + 22, tr("S"));
    painter->drawText(rect.x() + 25, rect.y() + 22, EllS1Val);
    painter->drawText(rect.x() + 50, rect.y() + 22, EllS2Val);
    painter->drawText(rect.x() + 75, rect.y() + 22, EllS3Val);
    painter->drawText(rect.x() + 100, rect.y() + 22, EllS4Val);
    painter->drawText(rect.x() + 125, rect.y() + 22, EllS5Val);

    painter->drawText(rect.x() + 2, rect.y() + 35, tr("КФР"));
    painter->drawText(rect.x() + 25, rect.y() + 35, KFR1Val);
    painter->drawText(rect.x() + 50, rect.y() + 35, KFR2Val);
    painter->drawText(rect.x() + 75, rect.y() + 35, KFR3Val);
    painter->drawText(rect.x() + 100, rect.y() + 35, KFR4Val);
    painter->drawText(rect.x() + 125, rect.y() + 35, KFR5Val);

    painter->restore();
}

QStringList StabSignalsTestWidget::getProbesNameList(const QString &testUid, const int count)
{
    QStringList retval;
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
        foreach (auto probeUid, ti.probes)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(probeUid, pi))
                retval << pi.name;
        }
    if (retval.size() < count)
        for (int i = 0; i < count - retval.size(); ++i)
            retval << "";
    return retval;
}

