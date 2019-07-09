#include "testswidget.h"
#include "ui_testswidget.h"

#include <QApplication>
#include <QSettings>
#include <QFile>

#include "aanalyserapplication.h"
#include "executewidget.h"
#include "testsmodel.h"
#include "testproxymodel.h"

TestsWidget::TestsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestsWidget)
{
    ui->setupUi(this);

    restoreVisibleWidget();
}

TestsWidget::~TestsWidget()
{
    delete ui;
}

void TestsWidget::onDbConnect()
{
    ui->wgtResult->onDbConnect();
    ui->wgtDynamic->onDbConnect();
}

void TestsWidget::runTest()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->executeMetodic();
}

void TestsWidget::selectResult()
{
    ui->wgtDynamic->setVisible(false);
    ui->wgtResult->setVisible(true);
    saveVisibleWidget(ui->wgtResult->widgetName());
}

void TestsWidget::selectDynamic()
{
    ui->wgtResult->setVisible(false);
    ui->wgtDynamic->setVisible(true);
    saveVisibleWidget(ui->wgtDynamic->widgetName());
}

void TestsWidget::restoreVisibleWidget()
{
    QSettings set(QApplication::instance()->organizationName(),
                  QApplication::instance()->applicationName());
    set.beginGroup("ResultWidget");
    auto val = set.value("SelectionWidget", ui->wgtResult->widgetName()).toString();
    set.endGroup();
    if (val == ui->wgtResult->widgetName())
        ui->wgtDynamic->setVisible(false);
    else
    if (val == ui->wgtDynamic->widgetName())
        ui->wgtResult->setVisible(false);
}

void TestsWidget::saveVisibleWidget(const QString &value)
{
    QSettings set(QApplication::instance()->organizationName(),
                  QApplication::instance()->applicationName());
    set.beginGroup("ResultWidget");
    set.setValue("SelectionWidget", value);
    set.endGroup();
}

