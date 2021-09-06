#include "trenvisualize.h"
#include "ui_trenvisualize.h"

#include "aanalyserapplication.h"
#include "trentestcalculator.h"
#include "trenresultfactors.h"
#include "factorsfactory.h"
#include "baseutils.h"

TrenVisualize::TrenVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrenVisualize)
{
    ui->setupUi(this);
}

TrenVisualize::~TrenVisualize()
{
    if (m_calculator)
        delete m_calculator;
    delete ui;
}

void TrenVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
        m_calculator = new TrenTestCalculator(testUid);

    m_calculator->calculate();

    auto model = new QStandardItemModel(ui->tvFactorsTrenResult);
    for (int i = 0; i < m_calculator->factorsCount(); ++i)
    {
        auto factorUid = m_calculator->factor(i).uid();
        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factorUid);
        auto itemName = new QStandardItem(fi.name());
        itemName->setEditable(false);
        QString sval = "";
        //! Время обрабатываем отдельно
        if (m_calculator->factor(i).uid() == TrenResultFactorsDefines::TimeUid)
            sval = BaseUtils::getTimeBySecCount(m_calculator->factor(i).value());
        else
            sval = QString::number(m_calculator->factor(i).value());
        auto itemValue = new QStandardItem(sval);
        itemValue->setEditable(false);

        model->appendRow(QList<QStandardItem*>() << itemName << itemValue);
    }
    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Значение"));
    ui->tvFactorsTrenResult->setModel(model);
    ui->tvFactorsTrenResult->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
