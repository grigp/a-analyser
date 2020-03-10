#include "teppingtestvisualize.h"
#include "ui_teppingtestvisualize.h"

#include "teppingtestcalculator.h"

#include <QDebug>

TeppingTestVisualize::TeppingTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeppingTestVisualize)
{
    ui->setupUi(this);
}

TeppingTestVisualize::~TeppingTestVisualize()
{
    delete ui;
}

void TeppingTestVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new TeppingTestCalculator(testUid, this);
        m_calculator->calculate();
    }
    if (m_calculator->primaryFactorsCount() >= 4)
    {
        auto fctJumpsCount = m_calculator->primaryFactor(0);
        auto fctTimeTest = m_calculator->primaryFactor(1);
        auto fctHeightAvg = m_calculator->primaryFactor(2);
        auto fctTimeAvg = m_calculator->primaryFactor(3);
        ui->lblStepsCount->setText(QString(tr("Количество шагов") + " %1").arg(fctJumpsCount->value()));
        ui->lblTimeTest->setText(QString(tr("Общее время шагов") + " %1 " + tr("сек")).arg(fctTimeTest->value()));
        ui->lblTimeAvgLeftLeg->setText(QString(tr("Быстрота одиночного движения левой ноги") + " %1 " + tr("сек")).arg(fctHeightAvg->value()));
        ui->lblTimeAvgRightLeg->setText(QString(tr("Быстрота одиночного движения правой ноги") + " %1 " + tr("сек")).arg(fctTimeAvg->value()));

        auto* model = new QStandardItemModel(ui->tvSteps);

        int steps = m_calculator->stepsCount(BaseUtils::Left) > m_calculator->stepsCount(BaseUtils::Right) ?
                    m_calculator->stepsCount(BaseUtils::Left) : m_calculator->stepsCount(BaseUtils::Right);
        for (int i = 0; i < steps; ++i)
        {
            SignalsDefines::TeppingStepRec stepLeft(0);
            SignalsDefines::TeppingStepRec stepRight(0);
            if (i < m_calculator->stepsCount(BaseUtils::Left))
                stepLeft = m_calculator->step(BaseUtils::Left, i);
            if (i < m_calculator->stepsCount(BaseUtils::Right))
                stepRight = m_calculator->step(BaseUtils::Right, i);

            QString sLeft = "-";
            if (stepLeft.timeContact > 0)
                sLeft = QString::number(stepLeft.timeContact);
            QString sRight = "-";
            if (stepRight.timeContact > 0)
                sRight = QString::number(stepRight.timeContact);

            auto *itemN = new QStandardItem(QString::number(i + 1));
            itemN->setEditable(false);
            auto *itemL = new QStandardItem(sLeft);
            itemL->setEditable(false);
            auto *itemR = new QStandardItem(sRight);
            itemR->setEditable(false);

            model->appendRow(QList<QStandardItem*>() << itemN << itemL << itemR);
        }

        ui->tvSteps->setModel(model);
        model->setHorizontalHeaderLabels(QStringList() << tr("N") << tr("Левая нога") << tr("Правая нога"));
        ui->tvSteps->header()->resizeSection(0, 80);
        ui->tvSteps->header()->resizeSection(1, 150);
        ui->tvSteps->header()->resizeSection(2, 150);
    }
}
