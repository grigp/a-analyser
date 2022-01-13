#include "trianglevisualize.h"
#include "ui_trianglevisualize.h"

#include <QPainter>
#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "settingsprovider.h"
#include "trianglecalculator.h"
#include "reportelements.h"
#include "stabilogram.h"
#include "areaskgdefines.h"
#include "trianglefactors.h"

namespace
{

///< Список показателей, выводимых в таблицу основных показателей
///< Соответствие показателя для этапов обучения и анализа
static QList<BaseDefines::FctTblPair> FactorsMain = {
      BaseDefines::FctTblPair(TriangleFactorsDefines::Training::TimeUid, TriangleFactorsDefines::Analysis::TimeUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::MXUid, TriangleFactorsDefines::Analysis::MXUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::MYUid, TriangleFactorsDefines::Analysis::MYUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::SquareUid, TriangleFactorsDefines::Analysis::SquareUid)
};

///< Список показателей, выводимых в таблицу показателей диаграмм
///< Соответствие показателя для этапов обучения и анализа
static QList<BaseDefines::FctTblPair> FactorsDiags = {
      BaseDefines::FctTblPair(TriangleFactorsDefines::Training::TimeUid, TriangleFactorsDefines::Analysis::TimeUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::TimeQUid, TriangleFactorsDefines::Analysis::TimeQUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::SquareUid, TriangleFactorsDefines::Analysis::SquareUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::SquareQUid, TriangleFactorsDefines::Analysis::SquareQUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::SpeedUid, TriangleFactorsDefines::Analysis::SpeedUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::SpeedQUid, TriangleFactorsDefines::Analysis::SpeedQUid)

    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::UpErrSysXUid, TriangleFactorsDefines::Analysis::UpErrSysXUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::UpErrSysYUid, TriangleFactorsDefines::Analysis::UpErrSysYUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::UpErrRndXUid, TriangleFactorsDefines::Analysis::UpErrRndXUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::UpErrRndYUid, TriangleFactorsDefines::Analysis::UpErrRndYUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::RightErrSysXUid, TriangleFactorsDefines::Analysis::RightErrSysXUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::RightErrSysYUid, TriangleFactorsDefines::Analysis::RightErrSysYUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::RightErrRndXUid, TriangleFactorsDefines::Analysis::RightErrRndXUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::RightErrRndYUid, TriangleFactorsDefines::Analysis::RightErrRndYUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::LeftErrSysXUid, TriangleFactorsDefines::Analysis::LeftErrSysXUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::LeftErrSysYUid, TriangleFactorsDefines::Analysis::LeftErrSysYUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::LeftErrRndXUid, TriangleFactorsDefines::Analysis::LeftErrRndXUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::LeftErrRndYUid, TriangleFactorsDefines::Analysis::LeftErrRndYUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::MidErrSysXUid, TriangleFactorsDefines::Analysis::MidErrSysXUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::MidErrSysYUid, TriangleFactorsDefines::Analysis::MidErrSysYUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::MidErrRndXUid, TriangleFactorsDefines::Analysis::MidErrRndXUid)
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::MidErrRndYUid, TriangleFactorsDefines::Analysis::MidErrRndYUid)



};


}

TriangleVisualize::TriangleVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TriangleVisualize)
{
    ui->setupUi(this);

    auto val = SettingsProvider::valueFromRegAppCopy("TriangleResultWidget", "CurrentPage").toInt();
    ui->twPages->setCurrentIndex(val);

    restoreSplitterPositionDiag();
}

TriangleVisualize::~TriangleVisualize()
{
    delete ui;
}

void TriangleVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new TriangleCalculator(testUid, this);
        m_calculator->calculate();

        getSignal(testUid);

        showSKG(ui->wgtSKGTraining, BaseDefines::Section(0, m_calculator->trainingLength()));
        showSKG(ui->wgtSKGAnalysis, BaseDefines::Section(m_calculator->trainingLength(), m_calculator->signalLength()));
        m_triangleDiagIdxTraining = addTriangleDiag(ui->wgtSKGTraining, m_calculator->triangleTraining(), Qt::darkCyan);
        m_triangleDiagIdxAnalysis = addTriangleDiag(ui->wgtSKGAnalysis, m_calculator->triangleAnalysis(), Qt::darkCyan);

        m_curTriangleAnalysis = m_calculator->firstAnalysisTriangle();
        setBtnPNTrainingEnabled();
        setBtnPNAnalysisEnabled();

        showAllFactors();
        showMainDiagrams();
        showMainResultFactors();
        showDiagsResultFactors();
    }
}

void TriangleVisualize::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
//        //! Диаграмма. Копируется из виджета
//        ReportElements::drawWidget(painter, wgtDiag,
//                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.85),
//                                   paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height()/4));
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
//        //! Диаграмма. Копируется из виджета
//        ReportElements::drawWidget(painter, wgtDiag,
//                                   static_cast<int>(paper.width() * 0.75), static_cast<int>(paper.height() * 0.75),
//                                   static_cast<int>(paper.x() + paper.width()/5.5), paper.y() + paper.height()/6);
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

void TriangleVisualize::curPageChanged(int pageIdx)
{
    SettingsProvider::setValueToRegAppCopy("TriangleResultWidget", "CurrentPage", pageIdx);
}

void TriangleVisualize::splitterDiagMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPositionDiag();
    //    setSKGSize();
}

void TriangleVisualize::btnAllTrainingClicked(bool pressed)
{
    if (m_triangleDiagIdxTraining > -1)
        ui->wgtSKGTraining->deleteBrokenLine(m_triangleDiagIdxTraining);

    if (pressed)
    {
        ui->wgtSKGTraining->setSection(0, m_calculator->trainingLength());
        m_triangleDiagIdxTraining = addTriangleDiag(ui->wgtSKGTraining, m_calculator->triangleTraining(), Qt::darkCyan);
    }
    else
    {
        ui->wgtSKGTraining->setSection(m_calculator->triangleSection(m_curTriangleTraining).begin,
                                       m_calculator->triangleSection(m_curTriangleTraining).end);
        m_triangleDiagIdxTraining = addTriangleDiag(ui->wgtSKGTraining, m_calculator->triangle(m_curTriangleTraining), Qt::darkCyan);
    }
    setBtnPNTrainingEnabled();
}

void TriangleVisualize::btnAllAnalysisClicked(bool pressed)
{
    if (m_triangleDiagIdxAnalysis > -1)
        ui->wgtSKGAnalysis->deleteBrokenLine(m_triangleDiagIdxAnalysis);

    if (pressed)
    {
        ui->wgtSKGAnalysis->setSection(m_calculator->trainingLength(), m_calculator->signalLength());
        m_triangleDiagIdxAnalysis = addTriangleDiag(ui->wgtSKGAnalysis, m_calculator->triangleAnalysis(), Qt::darkCyan);
    }
    else
    {
        ui->wgtSKGAnalysis->setSection(m_calculator->triangleSection(m_curTriangleAnalysis).begin,
                                       m_calculator->triangleSection(m_curTriangleAnalysis).end);
        m_triangleDiagIdxAnalysis = addTriangleDiag(ui->wgtSKGAnalysis, m_calculator->triangle(m_curTriangleAnalysis), Qt::darkCyan);
    }
    setBtnPNAnalysisEnabled();
}

void TriangleVisualize::btnPrevTrainingClicked()
{
    if (m_triangleDiagIdxTraining > -1)
        ui->wgtSKGTraining->deleteBrokenLine(m_triangleDiagIdxTraining);

    if (m_curTriangleTraining > 0)
    {
        --m_curTriangleTraining;
        ui->wgtSKGTraining->setSection(m_calculator->triangleSection(m_curTriangleTraining).begin,
                                       m_calculator->triangleSection(m_curTriangleTraining).end);
        m_triangleDiagIdxTraining = addTriangleDiag(ui->wgtSKGTraining, m_calculator->triangle(m_curTriangleTraining), Qt::darkCyan);
    }
    setBtnPNTrainingEnabled();
}

void TriangleVisualize::btnNextTrainingClicked()
{
    if (m_triangleDiagIdxTraining > -1)
        ui->wgtSKGTraining->deleteBrokenLine(m_triangleDiagIdxTraining);

    if (m_curTriangleTraining < m_calculator->firstAnalysisTriangle() - 1)
    {
        ++m_curTriangleTraining;
        ui->wgtSKGTraining->setSection(m_calculator->triangleSection(m_curTriangleTraining).begin,
                                       m_calculator->triangleSection(m_curTriangleTraining).end);
        m_triangleDiagIdxTraining = addTriangleDiag(ui->wgtSKGTraining, m_calculator->triangle(m_curTriangleTraining), Qt::darkCyan);
    }
    setBtnPNTrainingEnabled();
}

void TriangleVisualize::setBtnPNTrainingEnabled()
{
    ui->tbPrevTraining->setEnabled(!ui->tbShowAllTraining->isChecked() &&
                                   m_curTriangleTraining > 0);
    ui->tbNextTraining->setEnabled(!ui->tbShowAllTraining->isChecked() &&
                                   m_curTriangleTraining < m_calculator->firstAnalysisTriangle() - 1);
    ui->lblTrnglNumTraining->setText(QString::number(m_curTriangleTraining + 1));
}

void TriangleVisualize::btnPrevAnalysisClicked()
{
    if (m_triangleDiagIdxAnalysis > -1)
        ui->wgtSKGAnalysis->deleteBrokenLine(m_triangleDiagIdxAnalysis);

    if (m_curTriangleAnalysis > m_calculator->firstAnalysisTriangle())
    {
        --m_curTriangleAnalysis;
        ui->wgtSKGAnalysis->setSection(m_calculator->triangleSection(m_curTriangleAnalysis).begin,
                                       m_calculator->triangleSection(m_curTriangleAnalysis).end);
        m_triangleDiagIdxAnalysis = addTriangleDiag(ui->wgtSKGAnalysis, m_calculator->triangle(m_curTriangleAnalysis), Qt::darkCyan);
    }
    setBtnPNAnalysisEnabled();
}

void TriangleVisualize::btnNextAnalysisClicked()
{
    if (m_triangleDiagIdxAnalysis > -1)
        ui->wgtSKGAnalysis->deleteBrokenLine(m_triangleDiagIdxAnalysis);

    if (m_curTriangleAnalysis < m_calculator->trianglesCount() - 1)
    {
        ++m_curTriangleAnalysis;
        ui->wgtSKGAnalysis->setSection(m_calculator->triangleSection(m_curTriangleAnalysis).begin,
                                       m_calculator->triangleSection(m_curTriangleAnalysis).end);
        m_triangleDiagIdxAnalysis = addTriangleDiag(ui->wgtSKGAnalysis, m_calculator->triangle(m_curTriangleAnalysis), Qt::darkCyan);
    }
    setBtnPNAnalysisEnabled();
}

void TriangleVisualize::setBtnPNAnalysisEnabled()
{
    ui->tbPrevAnalysis->setEnabled(!ui->tbShowAllAnalysis->isChecked() &&
                                   m_curTriangleAnalysis > m_calculator->firstAnalysisTriangle());
    ui->tbNextAnalysis->setEnabled(!ui->tbShowAllAnalysis->isChecked() &&
                                   m_curTriangleAnalysis < m_calculator->trianglesCount() - 1);
    ui->lblTrnglNumAnalysis->setText(QString::number(m_curTriangleAnalysis - m_calculator->firstAnalysisTriangle() + 1));
}

void TriangleVisualize::cbShowTraectoryTrainingClicked(bool isChecked)
{
    ui->wgtSKGTraining->setVisibleSKG(isChecked);
}

void TriangleVisualize::cbShowTraectoryAnalysisClicked(bool isChecked)
{
    ui->wgtSKGAnalysis->setVisibleSKG(isChecked);
}

void TriangleVisualize::tbPlusTrainingClicked()
{
    auto diap = ui->wgtSKGTraining->diap();
    if (diap > 1)
        ui->wgtSKGTraining->setDiap(diap / 2);
}

void TriangleVisualize::tbMinusTrainingClicked()
{
    auto diap = ui->wgtSKGTraining->diap();
    if (diap < 128)
        ui->wgtSKGTraining->setDiap(diap * 2);
}

void TriangleVisualize::tbPlusAnalysisClicked()
{
    auto diap = ui->wgtSKGAnalysis->diap();
    if (diap > 1)
        ui->wgtSKGAnalysis->setDiap(diap / 2);
}

void TriangleVisualize::tbMinusAnalysisClicked()
{
    auto diap = ui->wgtSKGAnalysis->diap();
    if (diap < 128)
        ui->wgtSKGAnalysis->setDiap(diap * 2);
}

void TriangleVisualize::getSignal(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
                if (DataProvider::channelExists(pi.uid, ChannelsDefines::chanStab) &&
                    DataProvider::channelExists(pi.uid, ChannelsDefines::chanTriangleResult))
                {
                    QByteArray baStab;
                    if (DataProvider::getChannel(pi.uid, ChannelsDefines::chanStab, baStab))
                        m_stab = new Stabilogram(baStab);
                }
        }
}

void TriangleVisualize::showSKG(AreaSKG *area, BaseDefines::Section section)
{
    area->setSignal(m_stab, section.begin, section.end);
    addTriangleDiag(area, m_calculator->triangleOriginal(), Qt::darkYellow);
}

int TriangleVisualize::addTriangleDiag(AreaSKG *area, TriangleDefines::Triangle triangle, QColor color)
{
    AreaSKGDefines::BrokenLine blTrngl;
    blTrngl.polygon << QPointF(triangle.topCorner().x(), triangle.topCorner().y())
                    << QPointF(triangle.leftDownCorner().x(), triangle.leftDownCorner().y())
                    << QPointF(triangle.rightDownCorner().x(), triangle.rightDownCorner().y());
    blTrngl.color = color;
    blTrngl.width = 3;
    return area->addBrokenLine(blTrngl);
}

void TriangleVisualize::showAllFactors()
{
    auto *model = new QStandardItemModel(ui->tvFactors);

    for (int i = 0; i < m_calculator->getFactorsOfStagesCount(); ++i)
    {
        QString fUidT {""};
        QString fUidA {""};

        if (m_calculator->getFactorOfStages(i, fUidT, fUidA))
        {
            auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(fUidT);
            QString n = fi.name();
            auto fn = n.left(n.lastIndexOf('(') - 1);
            if (fi.measure() != "")
                fn = fn + ", " + fi.measure();
            auto *itemName = new QStandardItem(fn);
            itemName->setEditable(false);

            auto *itemT = new QStandardItem(m_calculator->factorValueFormatted(fUidT));
            itemT->setEditable(false);

            auto *itemA = new QStandardItem(m_calculator->factorValueFormatted(fUidA));
            itemA->setEditable(false);

            model->appendRow(QList<QStandardItem*>() << itemName << itemT << itemA);
        }
    }

    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Этап обучения") << tr("Этап анализа"));
    ui->tvFactors->setModel(model);
    ui->tvFactors->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvFactors->header()->resizeSection(0, 430);
}

void TriangleVisualize::showMainDiagrams()
{
    double minTime = INT_MAX;
    double maxTime = -INT_MAX;
    double minMX = INT_MAX;
    double maxMX = -INT_MAX;
    double minMY = INT_MAX;
    double maxMY = -INT_MAX;
    double minSquare = INT_MAX;
    double maxSquare = -INT_MAX;
    auto setBounds = [&](const double val, double& min, double& max)
    {
        if (val < min)
            min = val;
        if (val > max)
            max = val;
    };

    for (int i = 0; i < m_calculator->trianglesCount(); ++i)
    {
        double time = m_calculator->triangle(i).time();
        setBounds(time, minTime, maxTime);
        double mx = m_calculator->triangle(i).mx();
        setBounds(mx, minMX, maxMX);
        double my = m_calculator->triangle(i).my();
        setBounds(my, minMY, maxMY);
        double square = m_calculator->triangle(i).square();
        setBounds(square, minSquare, maxSquare);

        if (i < m_calculator->firstAnalysisTriangle())
        {
            auto itemT = new DiagItem(time, tr(""));
            ui->wgtTimeTraining->appendItem(itemT);
            auto itemMX = new DiagItem(mx, tr(""));
            ui->wgtMXTraining->appendItem(itemMX);
            auto itemMY = new DiagItem(my, tr(""));
            ui->wgtMYTraining->appendItem(itemMY);
            auto itemS = new DiagItem(square, tr(""));
            ui->wgtSquareTraining->appendItem(itemS);
        }
        else
        {
            auto itemT = new DiagItem(time, tr(""));
            ui->wgtTimeAnalysis->appendItem(itemT);
            auto itemMX = new DiagItem(mx, tr(""));
            ui->wgtMXAnalysis->appendItem(itemMX);
            auto itemMY = new DiagItem(my, tr(""));
            ui->wgtMYAnalysis->appendItem(itemMY);
            auto itemS = new DiagItem(square, tr(""));
            ui->wgtSquareAnalysis->appendItem(itemS);
        }

    }

    auto setupDiag = [&](DynamicDiagram* diag, const QString title, const double min, const double max)
    {
        diag->setKind(DynamicDiagram::KindGraph);
        diag->setVolume(DynamicDiagram::Volume3D);
        diag->setTitle(title);
        diag->setAxisSpaceLeft(30);
        diag->setAxisSpaceBottom(10);
        diag->setDiap(min - (max - min) * 0.1, max + (max - min) * 0.1);
    };
    setupDiag(ui->wgtTimeTraining, tr("Длительность прохождения"), minTime, maxTime);
    setupDiag(ui->wgtTimeAnalysis, tr("Длительность прохождения"), minTime, maxTime);
    setupDiag(ui->wgtMXTraining, tr("Cмещение по фронтали"), minMX, maxMX);
    setupDiag(ui->wgtMXAnalysis, tr("Cмещение по фронтали"), minMX, maxMX);
    setupDiag(ui->wgtMYTraining, tr("Cмещение по сагиттали"), minMY, maxMY);
    setupDiag(ui->wgtMYAnalysis, tr("Cмещение по сагиттали"), minMY, maxMY);
    setupDiag(ui->wgtSquareTraining, tr("Площадь треугольников"), minSquare, maxSquare);
    setupDiag(ui->wgtSquareAnalysis, tr("Площадь треугольников"), minSquare, maxSquare);

    auto spTraining = ui->frTraining->sizePolicy();
    auto spAnalysis = ui->frAnalysis->sizePolicy();
    spTraining.setHorizontalStretch(m_calculator->firstAnalysisTriangle());
    spAnalysis.setHorizontalStretch(m_calculator->trianglesCount() - m_calculator->firstAnalysisTriangle());
    ui->frTraining->setSizePolicy(spTraining);
    ui->frAnalysis->setSizePolicy(spAnalysis);
}

void TriangleVisualize::showMainResultFactors()
{
    auto *model = new QStandardItemModel(ui->tvMainResFactors);

    foreach (auto uids, FactorsMain)
    {
        auto uidFctT = uids.first;
        auto uidFctA = uids.second;

        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uidFctT);
        QString n = fi.name();
        auto fn = n.left(n.lastIndexOf('(') - 1);
        if (fi.measure() != "")
            fn = fn + ", " + fi.measure();
        auto *itemName = new QStandardItem(fn);
        itemName->setEditable(false);
        auto *itemT = new QStandardItem(m_calculator->factorValueFormatted(uidFctT));
        itemT->setEditable(false);
        auto *itemA = new QStandardItem(m_calculator->factorValueFormatted(uidFctA));
        itemA->setEditable(false);
        model->appendRow(QList<QStandardItem*>() << itemName << itemT << itemA);
    }

    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Этап обучения") << tr("Этап анализа"));
    ui->tvMainResFactors->setModel(model);
    ui->tvMainResFactors->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvMainResFactors->header()->resizeSection(0, 430);
}

void TriangleVisualize::showDiagsResultFactors()
{
    auto *model = new QStandardItemModel(ui->tvDiagTable);

    foreach (auto uids, FactorsDiags)
    {
        auto uidFctT = uids.first;
        auto uidFctA = uids.second;

        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uidFctT);
        QString n = fi.name();
        auto fn = n.left(n.lastIndexOf('(') - 1);
        if (fi.measure() != "")
            fn = fn + ", " + fi.measure();
        auto *itemName = new QStandardItem(fn);
        itemName->setEditable(false);
        auto *itemT = new QStandardItem(m_calculator->factorValueFormatted(uidFctT));
        itemT->setEditable(false);
        auto *itemA = new QStandardItem(m_calculator->factorValueFormatted(uidFctA));
        itemA->setEditable(false);
        model->appendRow(QList<QStandardItem*>() << itemName << itemT << itemA);
    }

    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Этап обучения") << tr("Этап анализа"));
    ui->tvDiagTable->setModel(model);
    ui->tvDiagTable->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvDiagTable->header()->resizeSection(0, 430);
}

void TriangleVisualize::saveSplitterPositionDiag()
{
    SettingsProvider::setValueToRegAppCopy("TriangleResultWidget", "SplitterDiagramPosition", ui->splDiag->saveState());
    SettingsProvider::setValueToRegAppCopy("TriangleResultWidget", "SplitterDiagramAreasesPosition", ui->splDiagAreases->saveState());
}

void TriangleVisualize::restoreSplitterPositionDiag()
{
    auto valDiag = SettingsProvider::valueFromRegAppCopy("TriangleResultWidget", "SplitterDiagramPosition").toByteArray();
    ui->splDiag->restoreState(valDiag);
    auto valDiagAreases = SettingsProvider::valueFromRegAppCopy("TriangleResultWidget", "SplitterDiagramAreasesPosition").toByteArray();
    ui->splDiagAreases->restoreState(valDiagAreases);
}

