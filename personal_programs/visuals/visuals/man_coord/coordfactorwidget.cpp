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

    ui->pbValue->setStyleSheet("QProgressBar {"
                               "border: 2px solid rgb(0, 150, 150);" //grey;"
                               "border-radius: 4px;"
                               "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(210, 210, 210), stop: 0.1 rgb(150, 150, 150), stop: 0.9 rgb(100, 100, 100), stop: 1 rgb(50, 50, 50));"
                           "}"
                           "QProgressBar::chunk {"
                               "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(0, 240, 240), stop: 0.1 rgb(0, 210, 210), stop: 0.9 rgb(0, 180, 180), stop: 1 rgb(0, 130, 130));"
                               "width: 20px;"
                           "}");
}

CoordFactorWidget::~CoordFactorWidget()
{
    delete ui;
}

void CoordFactorWidget::setFactor(const int number, const QString &name, const double value)
{
    ui->lblFactor->setText("(" + QString::number(number) + ") " + name + "   " + QString::number(value, 'f', 0) + " %");
    ui->pbValue->setValue(static_cast<int>(value));
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
