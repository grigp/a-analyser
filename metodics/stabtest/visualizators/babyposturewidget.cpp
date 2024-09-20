#include "babyposturewidget.h"
#include "ui_babyposturewidget.h"

#include "babyposturecalculator.h"

#include <QDebug>

BabyPostureWidget::BabyPostureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BabyPostureWidget)
{
    ui->setupUi(this);
}

BabyPostureWidget::~BabyPostureWidget()
{
    delete ui;
}

bool isExtendFactors(BabyPostureCalculator *calculator)
{
    return calculator->factors1Count() > 0 && calculator->factors2Count() > 0 &&
            calculator->factors1Count() == calculator->factors2Count();
}

void BabyPostureWidget::calculate(BabyPostureCalculator *calculator, const QString &testUid)
{
    Q_UNUSED(testUid);
    ui->teResume->setStyleSheet("font-size: 14pt; color: rgb(32,88,103);");

    m_mdlTable.clear();

    int n = 0;
    if (isExtendFactors(calculator))
        n = calculator->factors1Count();
    else
        n = calculator->factorsCount();

    for (int i = 0; i < n; ++i)
    {
        BabyPostureCalculator::FactorInfo factor;
        BabyPostureCalculator::FactorInfo factor2;
        if (!isExtendFactors(calculator))
            factor = calculator->factor(i);
        else
        {
            factor = calculator->factor1(i);
            factor2 = calculator->factor2(i);
        }
        ui->teResume->append(factor.name + " " + factor.shortName + " : " + factor.valueFmt);

        auto itemName = new QStandardItem(factor.name + "             ");
        itemName->setEditable(false);

        auto itemShortName = new QStandardItem(factor.shortName + "       ");
        itemShortName->setEditable(false);

        auto itemValue = new QStandardItem(factor.valueFmt);
        itemValue->setEditable(false);

        if (isExtendFactors(calculator))
        {
            auto item2Value = new QStandardItem(factor2.valueFmt);
            item2Value->setEditable(false);
            m_mdlTable.appendRow(QList<QStandardItem*>() << itemName <<itemShortName << item2Value << itemValue);
        }
        else
            m_mdlTable.appendRow(QList<QStandardItem*>() << itemName <<itemShortName << itemValue);
    }
    if (!isExtendFactors(calculator))
        m_mdlTable.setHorizontalHeaderLabels(QStringList() << tr("Показатель")  << tr("") << tr("Значение"));
    else
        m_mdlTable.setHorizontalHeaderLabels(QStringList() << tr("Показатель")  << tr("") << tr("МБН") << tr("А-Мед"));

    ui->tvTable->setModel(&m_mdlTable);

    for (int i = 0; i < m_mdlTable.columnCount(); ++i)
        ui->tvTable->resizeColumnToContents(i);
}

void BabyPostureWidget::print(QPrinter *printer, const QString &testUid)
{

}

void BabyPostureWidget::paintPreview(QPainter *painter, QRect &rect, const QString &testUid, TestCalculator *calculator)
{

}
