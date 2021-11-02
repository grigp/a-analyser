#include "crossvisualize.h"
#include "ui_crossvisualize.h"

#include <QStandardItemModel>
#include <QPainter>
#include <QDebug>

#include "aanalyserapplication.h"
#include "crosscalculator.h"

namespace
{
    DiagCross *wgtDiag {nullptr};

}

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

        wgtDiag = ui->wgtDiag;
    }
}

void CrossVisualize::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();
    qDebug() << "print" << paper << printer->pageSize();

    painter->begin(printer);
    painter->setPen(Qt::black);
    painter->setBrush(QBrush(Qt::red));
    painter->drawRect(10, 10, 410, 410);
    painter->setFont(QFont("Sans",64,0,0));
    painter->drawText(QRect(0,0,3000,800), Qt::AlignLeft | Qt::AlignTop, "page1");

    double xscale = (paper.width() * 0.8) / static_cast<double>(wgtDiag->width());
    double yscale = (paper.height() * 0.8) / static_cast<double>(wgtDiag->height());
    double scale = qMin(xscale, yscale);
    painter->translate(paper.x() + paper.width()/10,
                       paper.y() + paper.height()/4);
    painter->scale(scale, scale);

    wgtDiag->render(painter);
    painter->end();

}
