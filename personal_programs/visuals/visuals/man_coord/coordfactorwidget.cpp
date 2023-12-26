#include "coordfactorwidget.h"
#include "ui_coordfactorwidget.h"

CoordFactorWidget::CoordFactorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoordFactorWidget)
{
    ui->setupUi(this);
    m_mdlComponents.clear();
    m_mdlComponents.setHorizontalHeaderLabels(QStringList() << tr("Параметр") << tr("Процент") << tr("Значение") << tr("Min") << tr("Max"));
    ui->tvComponents->setModel(&m_mdlComponents);
//    ui->tvComponents->header()->resizeSection(0, 300);
    ui->tvComponents->setVisible(false);
}

CoordFactorWidget::~CoordFactorWidget()
{
    delete ui;
}

void CoordFactorWidget::setFactor(const int number, const QString &name, const double value)
{
    ui->lblFactor->setText("(" + QString::number(number) + ") " + name + "   " + QString::number(value, 'f', 0) + " %");
}

void CoordFactorWidget::setComponent(const QString &name, const QString &value, const QString &percent, const QString &min, const QString &max)
{
    auto itemParam = new QStandardItem(name);
    itemParam->setEditable(false);

    auto itemPercent = new QStandardItem(percent);
    itemPercent->setEditable(false);

    auto itemValue = new QStandardItem(value);
    itemValue->setEditable(false);

    auto itemMin = new QStandardItem(min);
    itemMin->setEditable(false);

    auto itemMax = new QStandardItem(max);
    itemMax->setEditable(false);

    m_mdlComponents.appendRow(QList<QStandardItem*>() << itemParam << itemPercent << itemValue << itemMin << itemMax);
    ui->tvComponents->resizeColumnToContents(0);
}

void CoordFactorWidget::on_expanding()
{
    ui->tvComponents->setVisible(! ui->tvComponents->isVisible());
    if (ui->tvComponents->isVisible())
        ui->btnExpand->setIcon(QIcon(":/images/Collapse.png"));
    else
        ui->btnExpand->setIcon(QIcon(":/images/Expand.png"));
}
