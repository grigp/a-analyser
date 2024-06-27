#include "octaedronvisualize.h"
#include "ui_octaedronvisualize.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "basedefines.h"
#include "octaedroncalculator.h"
#include "reportelements.h"
#include "octaedronpainter.h"


OctaedronVisualize::OctaedronVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OctaedronVisualize)
{
    ui->setupUi(this);
}

OctaedronVisualize::~OctaedronVisualize()
{
    delete ui;
}

void OctaedronVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new OctaedronCalculator(testUid, this);
        m_calculator->calculate();

        auto crm = BaseDefines::CirceRoundRuleModeValueIndex.value(m_calculator->circeRoundRuleMode());
        ui->wgtDiag->setCirceRoundRuleMode(crm);
        auto dm = BaseDefines::DirectionModeValueIndex.value(m_calculator->directionMode());
        ui->wgtDiag->setDirection(dm);

        if (m_calculator->stageFinishMode() == BaseDefines::sfmFixedTime)
        {
            for (int i = 0; i < 8; ++i)
                ui->wgtDiag->setData(i, static_cast<int>(m_calculator->getValue(i)));

            m_sAverageQuality = tr("Успешность выполнения задания") + " " +
                    QString::number(m_calculator->getAverageValue(), 'f', 0) + " " + tr("%");
            ui->lblAverageQuality->setText(m_sAverageQuality);
            ui->lblAverageQuality->setStyleSheet("font-size: 18pt;");
        }
        else
        if (m_calculator->stageFinishMode() == BaseDefines::sfmFixingOnTarget)
        {
            ui->wgtDiag->setLabelMode(OctaedronPainter::lmIndexPlus1);
            for (int i = 0; i < 8; ++i)
            {
                int v = static_cast<int>((m_calculator->stageTime() - m_calculator->time(i)) / m_calculator->stageTime() * 100);
                ui->wgtDiag->setData(i, v);
            }
            showTableFactors();
        }

        ui->wgtDiag->doUpdate();
        ui->lblAverageQuality->setVisible(m_calculator->stageFinishMode() == BaseDefines::sfmFixedTime);
        ui->tvFactors->setVisible(m_calculator->stageFinishMode() == BaseDefines::sfmFixingOnTarget);

        m_wgtDiag = ui->wgtDiag;
    }
}

void OctaedronVisualize::print(QPrinter *printer, const QString &testUid)
{
    //! Получаем указатель на элземпляр визуализатора
    auto vis = static_cast<AAnalyserApplication*>(QCoreApplication::instance())->getOpenedTest(testUid);
    OctaedronVisualize* visual = static_cast<OctaedronVisualize*>(vis);

    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
        //! Диаграмма
        auto rectDiag = QRect(static_cast<int>(paper.x() + paper.width() * 0.1),
                              static_cast<int>(paper.y() + paper.height() * 0.2),
                              static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.width() * 0.8));
        double ratio = ReportElements::ratio(paper, visual->m_wgtDiag, 3);
        OctaedronPainter cp(painter, rectDiag);
        auto crm = BaseDefines::CirceRoundRuleModeValueIndex.value(visual->m_calculator->circeRoundRuleMode());
        cp.setCirceRoundRuleMode(crm);
        auto dm = BaseDefines::DirectionModeValueIndex.value(visual->m_calculator->directionMode());
        cp.setDirection(dm);

        if (visual->m_calculator->stageFinishMode() == BaseDefines::sfmFixedTime)
        {
            for (int i = 0; i < 8; ++i)
                cp.setData(i, static_cast<int>(visual->m_calculator->getValue(i)));
        }
        else
        if (visual->m_calculator->stageFinishMode() == BaseDefines::sfmFixingOnTarget)
        {
            cp.setLabelMode(OctaedronPainter::lmIndexPlus1);
            for (int i = 0; i < 8; ++i)
            {
                int v = static_cast<int>((visual->m_calculator->stageTime() - visual->m_calculator->time(i)) / visual->m_calculator->stageTime() * 100);
                cp.setData(i, v);
            }
        }
        cp.doPaint(ratio);

        if (visual->m_calculator->stageFinishMode() == BaseDefines::sfmFixedTime)
        {
            painter->setFont(QFont("Sans", 14, QFont::Bold, false));
            painter->setPen(Qt::darkCyan);
            painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 16 * 13), visual->m_sAverageQuality);
        }
        else
        if (visual->m_calculator->stageFinishMode() == BaseDefines::sfmFixingOnTarget)
        {
            //! Таблица показателей. Берется модель таблицы из визуализатора
            QRect rectTable(paper.x() + paper.width() / 10,
                            static_cast<int>(paper.y() + paper.height() / 7 * 5.5),
                            paper.width() / 10 * 8,
                            paper.height() / 10);
            ReportElements::drawTable(painter, visual->m_model, rectTable, QList<int>() << 3 << 2 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1,
                                      false, ReportElements::Table::tvsStretched, 12, -1, QFont::Bold);
        }
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        //! Диаграмма
        auto rectDiag = QRect(static_cast<int>(paper.x() + paper.width()/5), paper.y() + paper.height()/7,
                              static_cast<int>(paper.height() * 0.8), static_cast<int>(paper.height() * 0.8));
        double ratio = ReportElements::ratio(paper, visual->m_wgtDiag, 3);
        OctaedronPainter cp(painter, rectDiag);
        auto crm = BaseDefines::CirceRoundRuleModeValueIndex.value(visual->m_calculator->circeRoundRuleMode());
        cp.setCirceRoundRuleMode(crm);
        auto dm = BaseDefines::DirectionModeValueIndex.value(visual->m_calculator->directionMode());
        cp.setDirection(dm);

        if (visual->m_calculator->stageFinishMode() == BaseDefines::sfmFixedTime)
        {
            for (int i = 0; i < 8; ++i)
                cp.setData(i, static_cast<int>(visual->m_calculator->getValue(i)));
        }
        else
        if (visual->m_calculator->stageFinishMode() == BaseDefines::sfmFixingOnTarget)
        {
            cp.setLabelMode(OctaedronPainter::lmIndexPlus1);
            for (int i = 0; i < 8; ++i)
            {
                int v = static_cast<int>((visual->m_calculator->stageTime() - visual->m_calculator->time(i)) / visual->m_calculator->stageTime() * 100);
                cp.setData(i, v);
            }
        }
        cp.doPaint(ratio);

        if (visual->m_calculator->stageFinishMode() == BaseDefines::sfmFixedTime)
        {
            painter->setFont(QFont("Sans", 14, QFont::Bold, false));
            painter->setPen(Qt::darkCyan);
            painter->drawText(paper.x() + paper.width() / 5, static_cast<int>(paper.y() + paper.height() / 16 * 14), visual->m_sAverageQuality);
        }
        else
        if (visual->m_calculator->stageFinishMode() == BaseDefines::sfmFixingOnTarget)
        {
            //! Таблица показателей. Берется модель таблицы из визуализатора
            QRect rectTable(paper.x() + paper.width() / 10,
                            static_cast<int>(paper.y() + paper.height() / 7 * 6),
                            paper.width() / 10 * 8,
                            paper.height() / 10);
            ReportElements::drawTable(painter, visual->m_model, rectTable, QList<int>() << 3 << 2 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1,
                                      false, ReportElements::Table::tvsStretched, 12, -1, QFont::Bold);
        }
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

void OctaedronVisualize::showTableFactors()
{
    QList<QStandardItem*> lineTime;
    lineTime.clear();
    QList<QStandardItem*> lineLatent;
    lineLatent.clear();

    QStringList titles;
    titles.clear();
    titles << "" << tr("Среднее");

    auto itemTime = new QStandardItem("Время реакции");
    itemTime->setEditable(false);
    auto itemTimeAvg = new QStandardItem(QString::number(m_calculator->timeAvg(), 'f', 2));
    itemTimeAvg->setEditable(false);
    lineTime << itemTime << itemTimeAvg;

    auto itemLatent = new QStandardItem("Латентный период");
    itemLatent->setEditable(false);
    auto itemLatentAvg = new QStandardItem(QString::number(m_calculator->latentAvg(), 'f', 2));
    itemLatentAvg->setEditable(false);
    lineLatent << itemLatent << itemLatentAvg;

    for (int i = 0; i < 8; ++i)
    {
        titles << QString::number(i + 1);

        auto itemTime = new QStandardItem(QString::number(m_calculator->time(i), 'f', 2));
        itemTime->setEditable(false);
        lineTime << itemTime;

        auto itemLatent = new QStandardItem(QString::number(m_calculator->latent(i), 'f', 2));
        itemLatent->setEditable(false);
        lineLatent << itemLatent;
    }

    m_model = new QStandardItemModel(ui->tvFactors);
    m_model->appendRow(lineTime);
    m_model->appendRow(lineLatent);
    m_model->setHorizontalHeaderLabels(titles);
    ui->tvFactors->setModel(m_model);
    ui->tvFactors->resizeColumnToContents(0);
    for (int i = 1; i < m_model->columnCount(); ++i)
        ui->tvFactors->header()->resizeSection(i, 80);
}
