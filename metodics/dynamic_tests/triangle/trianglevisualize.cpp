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
#include "skgdefines.h"
#include "trianglefactors.h"
#include "triangleconslutionfactors.h"
#include "baseutils.h"
#include "skgwidget.h"

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
    , BaseDefines::FctTblPair(TriangleFactorsDefines::Training::AngleUid, TriangleFactorsDefines::Analysis::AngleUid)

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


///< Список показателей, выводимых в таблицу эффективности
///< Соответствие показателя для этапов обучения и анализа
static QList<BaseDefines::FctTblPair> FactorsEffectiveness = {
      BaseDefines::FctTblPair(TriangleConslutionFactorsDefines::KorrKognTstCntUid, TriangleConslutionFactorsDefines::KorrKognAnlCntUid)
    , BaseDefines::FctTblPair(TriangleConslutionFactorsDefines::KorrMotorTstCntUid, TriangleConslutionFactorsDefines::KorrMotorAnlCntUid)
    , BaseDefines::FctTblPair(TriangleConslutionFactorsDefines::ErrCntKognTstUid, TriangleConslutionFactorsDefines::ErrCntKognAnlUid)
    , BaseDefines::FctTblPair(TriangleConslutionFactorsDefines::ErrCntMotorTstUid, TriangleConslutionFactorsDefines::ErrCntMotorAnlUid)
};

}

TriangleVisualize::TriangleVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TriangleVisualize)
{
    ui->setupUi(this);

    restoreSplitterPositionDiag();
}

TriangleVisualize::~TriangleVisualize()
{
    delete ui;
}

void TriangleVisualize::setTest(const QString &testUid)
{
    auto val = SettingsProvider::valueFromRegAppCopy("Geometry/TriangleResultWidget", "CurrentPage").toInt();
    ui->twPages->setCurrentIndex(val);
    m_twPages = ui->twPages;

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
        showConslution();

        m_wgtSKGTraining = ui->wgtSKGTraining;
        m_wgtSKGAnalysis = ui->wgtSKGAnalysis;
    }
}

void TriangleVisualize::print(QPrinter *printer, const QString &testUid)
{
    //! Получаем указатель на экземпляр визуализатора
    auto vis = static_cast<AAnalyserApplication*>(QCoreApplication::instance())->getOpenedTest(testUid);
    TriangleVisualize* visual = static_cast<TriangleVisualize*>(vis);

    int tab = visual->m_twPages->currentIndex();
    for (int i = 0; i < visual->m_twPages->count(); ++i)
        visual->m_twPages->setCurrentIndex(i);
    visual->m_twPages->setCurrentIndex(tab);

    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);
    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));

    //! Показатели для заключения
    auto drawConslutionFactors = [&](const int start, const int offset)
    {
        int pos = start;
        painter->setFont(QFont("Sans", 14, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, tr("Результативность"));
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        pos += offset;


        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sAccRepeat);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sAccForm);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sAccMidX);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sAccMidY);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sMidSquareErrTst);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sMidTimeErrAnl);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sMidSquareErrAnl);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sMidPosErrAnl);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sMidAmplErrAnl);
        pos += offset;

        painter->setFont(QFont("Sans", 14, QFont::Bold, false));
        pos += (offset + 1);
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, tr("Эффективность"));
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        pos += offset;
        QRect rectDT(paper.x() + paper.width() / 10,
                     paper.y() + paper.height() / 90 * pos,
                     paper.width() / 10 * 9,
                     paper.height() / 90 * (offset + 5));
        ReportElements::drawTable(painter, visual->m_mdlEffectiveness, rectDT, QList<int>() << 3 << 2 << 2,
                                  false, ReportElements::Table::tvsStretched, 8, -1, QFont::Bold);
        pos += offset + 5;

        painter->setFont(QFont("Sans", 14, QFont::Bold, false));
        pos += (offset + 1);
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, tr("Стратегия"));
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, tr("Этап обучения"));
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sCorrectionPredominaceTrain);
        painter->setPen(visual->m_sCorrectionResumeColorTrain);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sCorrectionResumeTrain);
        pos += offset;

        //! Диаграмма преобладания коррекций.
        auto rectDSDDomCorrT = QRect(static_cast<int>(paper.x() + paper.width() * 0.1),
                                     static_cast<int>(paper.y() + paper.height() / 90 * pos - paper.height() * 0.02),
                                     static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.1));
        auto ratio = ReportElements::ratio(paper, visual->m_wgtCorrectionDiagTrain, 4);
        auto dsdDomCorrT = DualStateDiagramPainter(painter, rectDSDDomCorrT);
        auto valCDT = visual->m_calculator->factorConslutionValue(TriangleConslutionFactorsDefines::KorrDominTstUid);
        dsdDomCorrT.setValue(valCDT);
        dsdDomCorrT.setDescriptionLeft(tr("Быстрые коррекции"));
        dsdDomCorrT.setDescriptionRight(tr("Медленные коррекции"));
        dsdDomCorrT.doPaint(ratio);

        pos += offset + 7;

        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->setPen(Qt::black);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, tr("Этап анализа"));
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sCorrectionPredominaceAnal);
        painter->setPen(visual->m_sCorrectionResumeColorAnal);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sCorrectionResumeAnal);
        pos += offset;

        //! Диаграмма преобладания коррекций.
        auto rectDSDDomCorrA = QRect(static_cast<int>(paper.x() + paper.width() * 0.1),
                                     static_cast<int>(paper.y() + paper.height() / 90 * pos - paper.height() * 0.02),
                                     static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.1));
        ratio = ReportElements::ratio(paper, visual->m_wgtCorrectionDiagTrain, 4);
        auto dsdDomCorrA = DualStateDiagramPainter(painter, rectDSDDomCorrA);
        auto valCDA = visual->m_calculator->factorConslutionValue(TriangleConslutionFactorsDefines::KorrDominAnlUid);
        dsdDomCorrA.setValue(valCDA);
        dsdDomCorrA.setDescriptionLeft(tr("Быстрые коррекции"));
        dsdDomCorrA.setDescriptionRight(tr("Медленные коррекции"));
        dsdDomCorrA.doPaint(ratio);
    };

    int trainingLength = -1;
    int signalLength = -1;
    SKGDefines::BrokenLine blTrnOrigin;
    SKGDefines::BrokenLine blTrnTren;
    SKGDefines::BrokenLine blTrnAnal;
    if (visual->m_calculator)
    {
        trainingLength = visual->m_calculator->trainingLength();
        signalLength = visual->m_calculator->signalLength();
        auto assignTriangle = [&](SKGDefines::BrokenLine& bl, TriangleDefines::Triangle& triangle, const QColor color)
        {
            bl.polygon << QPointF(triangle.topCorner().x(), triangle.topCorner().y())
                       << QPointF(triangle.leftDownCorner().x(), triangle.leftDownCorner().y())
                       << QPointF(triangle.rightDownCorner().x(), triangle.rightDownCorner().y());
            bl.color = color;
            bl.width = 3;
        };
        auto trnOrigin = visual->m_calculator->triangleOriginal();
        assignTriangle(blTrnOrigin, trnOrigin, Qt::darkYellow);
        auto trnTren = visual->m_calculator->triangleTraining();
        assignTriangle(blTrnTren, trnTren, Qt::darkGreen);
        auto trnAnal = visual->m_calculator->triangleAnalysis();
        assignTriangle(blTrnAnal, trnAnal, Qt::darkGreen);
    }

    if (printer->orientation() == QPrinter::Portrait)
    {
        //! СКГ
        auto rectSKG = QRect(paper.x() + paper.width()/20, static_cast<int>(paper.y() + paper.height()/9),
                             static_cast<int>(paper.width() * 0.42), static_cast<int>(paper.height() * 0.42));
        double ratio = ReportElements::ratio(paper, visual->m_wgtSKGTraining, 5);
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio, -1, 0, trainingLength - 1,
                                    QList<SKGDefines::BrokenLine>() << blTrnOrigin << blTrnTren);

        rectSKG = QRect(paper.x() + paper.width()/20 * 10, static_cast<int>(paper.y() + paper.height()/9),
                        static_cast<int>(paper.width() * 0.42), static_cast<int>(paper.height() * 0.42));
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio, -1, trainingLength, signalLength,
                                    QList<SKGDefines::BrokenLine>() << blTrnOrigin << blTrnAnal);

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectDT(paper.x() + paper.width() / 10,
                        paper.y() + paper.height() / 14 * 7,
                        paper.width() / 10 * 9,
                        paper.height() / 3);
        ReportElements::drawTable(painter, visual->m_mdlDiagTable, rectDT, QList<int>() << 3 << 2 << 2,
                                  false, ReportElements::Table::tvsStretched, 8, -1, QFont::Bold);

        painter->setFont(QFont("Sans", 10, 0, false));
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 14 * 12, visual->m_sLatentMoving);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter);

        //!------------------- Страница 2
        printer->newPage();

        drawConslutionFactors(7, 2);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter);

        //!------------------- Страница 3
        printer->newPage();

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectMT(paper.x() + paper.width() / 10,
                        paper.y() + paper.height() / 10,
                        paper.width() / 10 * 9,
                        paper.height() / 10 * 8);
        ReportElements::drawTable(painter, visual->m_mdlMainTable, rectMT, QList<int>() << 3 << 2 << 2,
                                  false, ReportElements::Table::tvsStretched, 8, -1, QFont::Bold);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        //! СКГ
        auto rectSKG = QRect(static_cast<int>(paper.x() + paper.width()/10 * 0.8), paper.y() + paper.height()/7,
                             static_cast<int>(paper.width() * 0.4), static_cast<int>(paper.height() * 0.4));
        double ratio = ReportElements::ratio(paper, visual->m_wgtSKGTraining, 5);
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio, -1, 0, trainingLength - 1,
                                    QList<SKGDefines::BrokenLine>() << blTrnOrigin << blTrnTren);

        rectSKG = QRect(static_cast<int>(paper.x() + paper.width()/10 * 5), paper.y() + paper.height()/7,
                        static_cast<int>(paper.width() * 0.4), static_cast<int>(paper.height() * 0.4));
        if (DataProvider::channelExists(testUid, 0, ChannelsDefines::chanStab))
            ReportElements::drawSKG(painter, rectSKG, testUid, 0, ratio, -1, trainingLength, signalLength,
                                    QList<SKGDefines::BrokenLine>() << blTrnOrigin << blTrnAnal);

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectDT(paper.x() + paper.width() / 10,
                        paper.y() + paper.height() / 40 * 24,
                        paper.width() / 10 * 9,
                        paper.height() / 3);
        ReportElements::drawTable(painter, visual->m_mdlDiagTable, rectDT, QList<int>() << 3 << 2 << 2,
                                  false, ReportElements::Table::tvsStretched, 8, -1, QFont::Bold);

        painter->setFont(QFont("Sans", 10, 0, false));
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 56 * 53, visual->m_sLatentMoving);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter);

        //!------------------- Страница 2
        printer->newPage();

        drawConslutionFactors(4, 2);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter);

        //!------------------- Страница 3
        printer->newPage();

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectMT(paper.x() + paper.width() / 10,
                        paper.y() + paper.height() / 10,
                        paper.width() / 10 * 9,
                        paper.height() / 10 * 8);
        ReportElements::drawTable(painter, visual->m_mdlMainTable, rectMT, QList<int>() << 3 << 2 << 2,
                                  false, ReportElements::Table::tvsStretched, 8, -1, QFont::Bold);
    }

    //! Нижний колонтитул
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

void TriangleVisualize::paintPreview(QPainter *painter, QRect &rect, const QString &testUid, TriangleCalculator *calculator)
{
    Q_UNUSED(testUid);

    painter->save();

    int size = 0;
    int z = 6;
    if (rect.width() > rect.height())
        size = (rect.height() - z) / 2;
    else
        size = (rect.width() - z) / 2;

    //! Значения показателей
    painter->setFont(QFont("Sans", 7, QFont::Bold, false));
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(TriangleConslutionFactorsDefines::AccRepeatUid);
    auto sv = calculator->factorConslutionValueFormatted(TriangleConslutionFactorsDefines::AccRepeatUid);
    painter->drawText(rect.x() + 4, rect.y() + 7, fi.shortName() + ", " + fi.measure());
    painter->drawText(rect.x() + 100, rect.y() + 7, sv);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(TriangleConslutionFactorsDefines::AccFormUid);
    sv = calculator->factorConslutionValueFormatted(TriangleConslutionFactorsDefines::AccFormUid);
    painter->drawText(rect.x() + 4, rect.y() + 17, fi.shortName() + ", " + fi.measure());
    painter->drawText(rect.x() + 100, rect.y() + 17, sv);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(TriangleConslutionFactorsDefines::AccMidXUid);
    sv = calculator->factorConslutionValueFormatted(TriangleConslutionFactorsDefines::AccMidXUid);
    painter->drawText(rect.x() + 4, rect.y() + 27, fi.shortName() + ", " + fi.measure());
    painter->drawText(rect.x() + 100, rect.y() + 27, sv);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(TriangleConslutionFactorsDefines::AccMidYUid);
    sv = calculator->factorConslutionValueFormatted(TriangleConslutionFactorsDefines::AccMidYUid);
    painter->drawText(rect.x() + 4, rect.y() + 37, fi.shortName() + ", " + fi.measure());
    painter->drawText(rect.x() + 100, rect.y() + 37, sv);

    painter->restore();
}

void TriangleVisualize::curPageChanged(int pageIdx)
{
    SettingsProvider::setValueToRegAppCopy("Geometry/TriangleResultWidget", "CurrentPage", pageIdx);
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
        ui->tbShowAllTraining->setToolTip(tr("Показать треугольники по отдельности"));
    }
    else
    {
        ui->wgtSKGTraining->setSection(m_calculator->triangleSection(m_curTriangleTraining).begin,
                                       m_calculator->triangleSection(m_curTriangleTraining).end);
        m_triangleDiagIdxTraining = addTriangleDiag(ui->wgtSKGTraining, m_calculator->triangle(m_curTriangleTraining), Qt::darkCyan);
        ui->tbShowAllTraining->setToolTip(tr("Показать усредненный треугольник"));
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
        ui->tbShowAllAnalysis->setToolTip(tr("Показать треугольники по отдельности"));
    }
    else
    {
        ui->wgtSKGAnalysis->setSection(m_calculator->triangleSection(m_curTriangleAnalysis).begin,
                                       m_calculator->triangleSection(m_curTriangleAnalysis).end);
        m_triangleDiagIdxAnalysis = addTriangleDiag(ui->wgtSKGAnalysis, m_calculator->triangle(m_curTriangleAnalysis), Qt::darkCyan);
        ui->tbShowAllAnalysis->setToolTip(tr("Показать усредненный треугольник"));
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

void TriangleVisualize::showSKG(SKGWidget *area, BaseDefines::Section section)
{
    area->setSignal(m_stab, 0, section.begin, section.end);
    addTriangleDiag(area, m_calculator->triangleOriginal(), Qt::darkYellow);
}

int TriangleVisualize::addTriangleDiag(SKGWidget *area, TriangleDefines::Triangle triangle, QColor color)
{
    SKGDefines::BrokenLine blTrngl;
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

    m_mdlMainTable = static_cast<QStandardItemModel*>(ui->tvFactors->model());
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
        diag->setKind(DynamicDiagramDefines::KindGraph);
        diag->setVolume(DynamicDiagramDefines::Volume3D);
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

    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(TriangleFactorsDefines::LatentMovingUid);
    m_sLatentMoving = fi.name() + ", " + fi.measure() + "    " + m_calculator->factorValueFormatted(TriangleFactorsDefines::LatentMovingUid);
    ui->lblLatentMoving->setText(m_sLatentMoving);

    m_mdlDiagTable = static_cast<QStandardItemModel*>(ui->tvDiagTable->model());
}

void TriangleVisualize::showConslution()
{
    auto writeFactor = [&](QLabel* label, const QString uid, QString& value)
    {
        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uid);
        auto sv = m_calculator->factorConslutionValueFormatted(uid);
        value = fi.name() + " : " + sv + " " + fi.measure();
        label->setText(value);
    };

    writeFactor(ui->lblAccRepeat, TriangleConslutionFactorsDefines::AccRepeatUid, m_sAccRepeat);
    writeFactor(ui->lblAccForm, TriangleConslutionFactorsDefines::AccFormUid, m_sAccForm);
    writeFactor(ui->lblAccMidX, TriangleConslutionFactorsDefines::AccMidXUid, m_sAccMidX);
    writeFactor(ui->lblAccMidY, TriangleConslutionFactorsDefines::AccMidYUid, m_sAccMidY);
    writeFactor(ui->lblMidSquareErrTst, TriangleConslutionFactorsDefines::MidSquareErrTstUid, m_sMidSquareErrTst);
    writeFactor(ui->lblMidTimeErrAnl, TriangleConslutionFactorsDefines::MidTimeErrAnlUid, m_sMidTimeErrAnl);
    writeFactor(ui->lblMidSquareErrAnl, TriangleConslutionFactorsDefines::MidSquareErrAnlUid, m_sMidSquareErrAnl);
    writeFactor(ui->lblMidPosErrAnl, TriangleConslutionFactorsDefines::MidPosErrAnlUid, m_sMidPosErrAnl);
    writeFactor(ui->lblMidAmplErrAnl, TriangleConslutionFactorsDefines::MidAmplErrAnlUid, m_sMidAmplErrAnl);

    showFactorsEffectiveness();
    showConslutionStrategy();
}

void TriangleVisualize::showFactorsEffectiveness()
{
    auto *model = new QStandardItemModel(ui->tvEffectiveness);

    foreach (auto uids, FactorsEffectiveness)
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
        auto *itemT = new QStandardItem(m_calculator->factorConslutionValueFormatted(uidFctT));
        itemT->setEditable(false);
        auto *itemA = new QStandardItem(m_calculator->factorConslutionValueFormatted(uidFctA));
        itemA->setEditable(false);
        model->appendRow(QList<QStandardItem*>() << itemName << itemT << itemA);
    }

    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Этап обучения") << tr("Этап анализа"));
    ui->tvEffectiveness->setModel(model);
    ui->tvEffectiveness->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvEffectiveness->header()->resizeSection(0, 430);

    m_mdlEffectiveness = static_cast<QStandardItemModel*>(ui->tvEffectiveness->model());
}

void TriangleVisualize::showConslutionStrategy()
{
    auto showStrategy = [&](const QString uid,
                            QString& value, QString& resumeTxt, QColor& resumeColor,
                            DualStateDiagram* diag,
                            QLabel* lblValue,
                            QLabel* lblResume)
    {
        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uid);
        double v = m_calculator->factorConslutionValue(uid);
        value = fi.name() + " : " + QString::number(v, 'f', fi.format()) + " " + tr("%");
        lblValue->setText(value);

        QString resume = "";
        QString colorResume = "";
        BaseUtils::setCorrectionsDominanceResume(v, resume, colorResume);
        resumeTxt = resume;
        lblResume->setText(resume);
        lblResume->setStyleSheet(colorResume);
        QPalette pal = ui->lblCorrectionResumeTrain->palette();
        resumeColor = pal.color(QPalette::WindowText);

        diag->setValue(v);
        diag->setDescriptionLeft(tr("Быстрые коррекции"));
        diag->setDescriptionRight(tr("Медленные коррекции"));
        m_wgtCorrectionDiagTrain = ui->wgtCorrectionDiagTrain;
    };

    showStrategy(TriangleConslutionFactorsDefines::KorrDominTstUid,
                 m_sCorrectionPredominaceTrain, m_sCorrectionResumeTrain, m_sCorrectionResumeColorTrain,
                 ui->wgtCorrectionDiagTrain, ui->lblCorrectionPredominaceTrain, ui->lblCorrectionResumeTrain);
    m_wgtCorrectionDiagTrain = ui->wgtCorrectionDiagTrain;

    showStrategy(TriangleConslutionFactorsDefines::KorrDominAnlUid,
                 m_sCorrectionPredominaceAnal, m_sCorrectionResumeAnal, m_sCorrectionResumeColorAnal,
                 ui->wgtCorrectionDiagAnal, ui->lblCorrectionPredominaceAnal, ui->lblCorrectionResumeAnal);
    m_wgtCorrectionDiagAnal = ui->wgtCorrectionDiagAnal;
}

void TriangleVisualize::saveSplitterPositionDiag()
{
    SettingsProvider::setValueToRegAppCopy("Geometry/TriangleResultWidget", "SplitterDiagramPosition", ui->splDiag->saveState());
    SettingsProvider::setValueToRegAppCopy("Geometry/TriangleResultWidget", "SplitterDiagramAreasesPosition", ui->splDiagAreases->saveState());
}

void TriangleVisualize::restoreSplitterPositionDiag()
{
    auto valDiag = SettingsProvider::valueFromRegAppCopy("Geometry/TriangleResultWidget", "SplitterDiagramPosition").toByteArray();
    ui->splDiag->restoreState(valDiag);
    auto valDiagAreases = SettingsProvider::valueFromRegAppCopy("Geometry/TriangleResultWidget", "SplitterDiagramAreasesPosition").toByteArray();
    ui->splDiagAreases->restoreState(valDiagAreases);
}

