#include "babyposturewidget.h"
#include "ui_babyposturewidget.h"

#include "babyposturecalculator.h"

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

void BabyPostureWidget::calculate(BabyPostureCalculator *calculator, const QString &testUid)
{
    Q_UNUSED(testUid);
    ui->teResume->setStyleSheet("font-size: 14pt; color: rgb(32,88,103);");

    m_mdlTable.clear();

    for (int i = 0; i < calculator->factorsCount(); ++i)
    {
        auto factor = calculator->factor(i);
        ui->teResume->append(factor.name + " " + factor.shortName + " : " + factor.valueFmt);

        auto itemName = new QStandardItem(factor.name + "             ");
        itemName->setEditable(false);

        auto itemShortName = new QStandardItem(factor.shortName + "       ");
        itemShortName->setEditable(false);

        auto itemValue = new QStandardItem(factor.valueFmt);
        itemValue->setEditable(false);

        m_mdlTable.appendRow(QList<QStandardItem*>() << itemName <<itemShortName << itemValue);
    }
    m_mdlTable.setHorizontalHeaderLabels(QStringList() << tr("Показатель")  << tr("") << tr("Значение"));

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
