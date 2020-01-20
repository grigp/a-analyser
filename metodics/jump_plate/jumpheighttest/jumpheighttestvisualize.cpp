#include "jumpheighttestvisualize.h"
#include "ui_jumpheighttestvisualize.h"

#include "jumpheighttestcalculator.h"
#include "datadefines.h"
#include "dataprovider.h"

#include <QStandardItemModel>
#include <QDebug>

JumpHeightTestVisualize::JumpHeightTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JumpHeightTestVisualize)
{
    ui->setupUi(this);
}

JumpHeightTestVisualize::~JumpHeightTestVisualize()
{
    delete ui;
}

void JumpHeightTestVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new JumpHeightTestCalculator(testUid, this);
        m_calculator->calculate();
    }

    if (m_calculator->primaryFactorsCount() >= 4)
    {
        getStrategyParams(testUid);

        auto fctJumpsCount = m_calculator->primaryFactor(0);
        auto fctTimeTest = m_calculator->primaryFactor(1);
        auto fctHeightAvg = m_calculator->primaryFactor(2);
        auto fctTimeAvg = m_calculator->primaryFactor(3);
        ui->lblJumpsCount->setText(QString(tr("Количество прыжков") + " %1").arg(fctJumpsCount->value()));
        ui->lblTimeTest->setText(QString(tr("Общее время прыжков") + " %1 " + tr("сек")).arg(fctTimeTest->value()));
        ui->lblHeightAvg->setText(QString(tr("Средняя высота прыжков") + " %1 " + tr("см")).arg(fctHeightAvg->value()));
        ui->lblTimeAvg->setText(QString(tr("Среднее время на платформе") + " %1 " + tr("сек")).arg(fctTimeAvg->value()));

        auto* model = new QStandardItemModel(ui->tvJumps);

        for (int i = 0; i < m_calculator->jumpsCount(); ++i)
        {
            auto jump = m_calculator->jump(i);

            auto *itemN = new QStandardItem(QString::number(i + 1));
            itemN->setEditable(false);
            auto *itemH = new QStandardItem(QString::number(jump.height));
            itemH->setEditable(false);
            auto *itemT = new QStandardItem(QString::number(jump.timeJump));
            itemT->setEditable(false);

            model->appendRow(QList<QStandardItem*>() << itemN << itemH << itemT);
        }

        ui->tvJumps->setModel(model);
        model->setHorizontalHeaderLabels(QStringList() << tr("N") << tr("Высота прыжка, см") << tr("Время контактной фазы, сек"));
        ui->tvJumps->header()->resizeSections(QHeaderView::ResizeToContents);
        ui->tvJumps->header()->resizeSection(0, 100);
    }
}

void JumpHeightTestVisualize::getStrategyParams(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        auto sStrg = ti.params["strategy"].toString();
        m_strategy = JumpHeightTestDefines::StrategyIndex.value(sStrg);
        if (m_strategy == JumpHeightTestDefines::jhsMinContactTime)
        {
            m_contactTimeBound = ti.params["contact_time_bound"].toInt() / 1000.0;
            ui->lblContactTimeBound->setText(QString(tr("Порог времени на платформе") + " %1 " + tr("сек")).arg(m_contactTimeBound));
        }
        ui->lblContactTimeBound->setVisible(m_strategy == JumpHeightTestDefines::jhsMinContactTime);
    }
}
