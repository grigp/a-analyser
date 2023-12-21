#include "coordfactorwidget.h"
#include "ui_coordfactorwidget.h"

CoordFactorWidget::CoordFactorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoordFactorWidget)
{
    ui->setupUi(this);
    m_mdlComponents.clear();
    ui->tvComponents->setModel(&m_mdlComponents);
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

}

void CoordFactorWidget::on_expand_collapse()
{
    ui->tvComponents->setVisible(! ui->tvComponents->isVisible());

}
