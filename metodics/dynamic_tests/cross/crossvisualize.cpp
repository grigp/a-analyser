#include "crossvisualize.h"
#include "ui_crossvisualize.h"

#include <QStandardItemModel>
#include <QPainter>
#include <QDebug>

#include "aanalyserapplication.h"
#include "crosscalculator.h"
#include "reportelements.h"
#include "crosspainter.h"


CrossVisualize::CrossVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CrossVisualize)
{
    ui->setupUi(this);
}

CrossVisualize::~CrossVisualize()
{
    delete ui;
}

void CrossVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new CrossCalculator(testUid, this);
        m_calculator->calculate();

        auto model = new QStandardItemModel();
        for (int i = 0; i < m_calculator->primaryFactorsCount(); ++i)
        {
            auto factor = m_calculator->primaryFactor(i);
            auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factor->uid());
            QString fn = fi.name();
            if (fi.measure() != "")
                fn = fn + ", " + fi.measure();
            auto *itemName = new QStandardItem(fn);
            itemName->setEditable(false);
            auto *itemValue = new QStandardItem(QString::number(factor->value(), 'f', fi.format()));
            itemValue->setEditable(false);
            model->appendRow(QList<QStandardItem*>() << itemName << itemValue);
        }

        model->setHorizontalHeaderLabels(QStringList() << tr("Показатель")
                                                       << tr("Значение"));
        ui->tvFactors->setModel(model);
        ui->tvFactors->header()->resizeSections(QHeaderView::ResizeToContents);
        ui->tvFactors->header()->resizeSection(0, 450);

        ui->wgtDiag->setDiap(m_calculator->diap());
        ui->wgtDiag->setValueUp(static_cast<int>(m_calculator->valueUp()));
        ui->wgtDiag->setValueDown(static_cast<int>(m_calculator->valueDown()));
        ui->wgtDiag->setValueRight(static_cast<int>(m_calculator->valueRight()));
        ui->wgtDiag->setValueLeft(static_cast<int>(m_calculator->valueLeft()));

        //! Запомнить указатели для печати
        m_wgtDiag = ui->wgtDiag;
        m_mdlFactors = static_cast<QStandardItemModel*>(ui->tvFactors->model());
    }
}

void CrossVisualize::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    //! Получаем указатель на элземпляр визуализатора
    auto vis = static_cast<AAnalyserApplication*>(QCoreApplication::instance())->getOpenedTest(testUid);
    CrossVisualize* visual = static_cast<CrossVisualize*>(vis);

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
        //! Диаграмма Cross
        auto rectDiag = QRect(static_cast<int>(paper.x() + paper.width() / 2 - paper.height() * 0.25),
                              paper.y() + paper.height()/7,
                             static_cast<int>(paper.height() * 0.5), static_cast<int>(paper.height() * 0.5));
        double ratio = ReportElements::ratio(paper, visual->m_wgtDiag, 3);
        CrossPainter cp(painter, rectDiag);
        cp.setDiap(static_cast<int>(visual->m_calculator->diap()));
        cp.setValueUp(static_cast<int>(visual->m_calculator->valueUp()));
        cp.setValueDown(static_cast<int>(visual->m_calculator->valueDown()));
        cp.setValueLeft(static_cast<int>(visual->m_calculator->valueLeft()));
        cp.setValueRight(static_cast<int>(visual->m_calculator->valueRight()));
        cp.doPaint(ratio);

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectTable(paper.x() + paper.width() / 10,
                        paper.y() + paper.height() / 3 * 2,
                        paper.width() / 10 * 8,
                        paper.height() / 4);
        ReportElements::drawTable(painter, visual->m_mdlFactors, rectTable, QList<int>() << 3 << 1,
                                  false, ReportElements::Table::tvsStretched, 12, -1, QFont::Bold);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        //! Диаграмма Cross
        auto rectDiag = QRect(paper.x() + paper.width()/25, paper.y() + paper.height()/5,
                             static_cast<int>(paper.height() * 0.7), static_cast<int>(paper.height() * 0.7));
        double ratio = ReportElements::ratio(paper, visual->m_wgtDiag, 3);
        CrossPainter cp(painter, rectDiag);
        cp.setDiap(static_cast<int>(visual->m_calculator->diap()));
        cp.setValueUp(static_cast<int>(visual->m_calculator->valueUp()));
        cp.setValueDown(static_cast<int>(visual->m_calculator->valueDown()));
        cp.setValueLeft(static_cast<int>(visual->m_calculator->valueLeft()));
        cp.setValueRight(static_cast<int>(visual->m_calculator->valueRight()));
        cp.doPaint(ratio);

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectTable(paper.x() + paper.width() / 7 * 4,
                        static_cast<int>(paper.y() + paper.height() / 3.5),
                        paper.width() / 7 * 3,
                        paper.height() / 2);
        ReportElements::drawTable(painter, visual->m_mdlFactors, rectTable, QList<int>() << 3 << 1, false,
                                  ReportElements::Table::tvsStretched, 12, -1, QFont::Bold);
    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

void CrossVisualize::paintPreview(QPainter *painter, QRect &rect, const QString &testUid, CrossCalculator *calculator)
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
    double diap = calculator->diap();
    double vUp = calculator->valueUp();
    double vDn = calculator->valueDown();
    double vLf = calculator->valueLeft();
    double vRt = calculator->valueRight();

    //! Фон
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(rect.center().x() - z/2, rect.center().y() - z/2 - size, z, size);
    painter->drawRect(rect.center().x() - z/2, rect.center().y() + z/2, z, size);
    painter->drawRect(rect.center().x() - z/2 - size, rect.center().y() - z/2, size, z);
    painter->drawRect(rect.center().x() + z/2, rect.center().y() - z/2, size, z);

    //! Значения диаграммы
    painter->setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::FlatCap));
    painter->setBrush(QBrush(Qt::green));
    int sz = static_cast<int>(vUp / diap * size);
    painter->drawRect(rect.center().x() - z/2, rect.center().y() - z/2 - sz, z, sz);
    sz = static_cast<int>(vDn / diap * size);
    painter->drawRect(rect.center().x() - z/2, rect.center().y() + z/2, z, sz);
    sz = static_cast<int>(vLf / diap * size);
    painter->drawRect(rect.center().x() - z/2 - sz, rect.center().y() - z/2, sz, z);
    sz = static_cast<int>(vRt / diap * size);
    painter->drawRect(rect.center().x() + z/2, rect.center().y() - z/2, sz, z);

    //! Рамка
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter->setBrush(QBrush());
    painter->drawRect(rect.center().x() - z/2, rect.center().y() - z/2 - size, z, size);
    painter->drawRect(rect.center().x() - z/2, rect.center().y() + z/2, z, size);
    painter->drawRect(rect.center().x() - z/2 - size, rect.center().y() - z/2, size, z);
    painter->drawRect(rect.center().x() + z/2, rect.center().y() - z/2, size, z);

    painter->setFont(QFont("Sans", 6, QFont::Bold, false));
    painter->drawText(rect.center().x() + z/2 + 3, rect.center().y() - z/2 - size + 6, QString::number(vUp, 'f', 0));
    painter->drawText(rect.center().x() + z/2 + 3, rect.center().y() + z/2 + size, QString::number(vDn, 'f', 0));
    painter->drawText(rect.center().x() - z/2 - size, rect.center().y() + z/2 + 8, QString::number(vLf, 'f', 0));
    painter->drawText(rect.center().x() + z/2 + size - 10, rect.center().y() + z/2 + 8, QString::number(vRt, 'f', 0));

    painter->restore();
}
