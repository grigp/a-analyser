#include "trentakeputvisualize.h"
#include "ui_trentakeputvisualize.h"

#include "aanalyserapplication.h"
#include "trentakeputtestcalculator.h"
#include "factorsfactory.h"

TrenTakePutVisualize::TrenTakePutVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrenTakePutVisualize)
{
    ui->setupUi(this);
}

TrenTakePutVisualize::~TrenTakePutVisualize()
{
    if (m_calculator)
        delete m_calculator;
    delete ui;
}

void TrenTakePutVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
        m_calculator = new TrenTakePutTestCalculator(testUid);

    m_calculator->calculate();

    auto model = new QStandardItemModel(ui->tvFactorsTrenResult);
    for (int i = 0; i < m_calculator->factorsCount(); ++i)
    {
        auto factorUid = m_calculator->factor(i).uid();
        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factorUid);
        auto itemName = new QStandardItem(fi.name());
        itemName->setEditable(false);
        auto itemValue = new QStandardItem(QString::number(m_calculator->factor(i).value()));
        itemValue->setEditable(false);

        model->appendRow(QList<QStandardItem*>() << itemName << itemValue);
    }
    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Значение"));
    ui->tvFactorsTrenResult->setModel(model);
    ui->tvFactorsTrenResult->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
