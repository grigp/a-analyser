#include "testswidget.h"
#include "ui_testswidget.h"

#include <QApplication>
#include <QFile>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QDebug>

#include "aanalyserapplication.h"
#include "executewidget.h"
#include "testsmodel.h"
#include "testproxymodel.h"
#include "settingsprovider.h"
#include "testpropertydialog.h"
#include "dataprovider.h"
#include "metodicsfactory.h"
#include "signalexporter.h"

TestsWidget::TestsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestsWidget)
{
    ui->setupUi(this);

    restoreVisibleWidget();

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::selectTest,
            [=](const QString &testUid)
    {
        m_selectedTestUid = testUid;
    });
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

void TestsWidget::onDbDisconnect()
{
    ui->wgtResult->onDbDisconnect();
    ui->wgtDynamic->onDbDisconnect();
}

void TestsWidget::runTest()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->executeMetodic();
}

void TestsWidget::deleteTest()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->deleteTest();
}

void TestsWidget::editTestProperty()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->editTestProperty();
}

void TestsWidget::printReport()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->printTestReport();
}

void TestsWidget::signalsAnalysis()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->signalsTestAnalysis();
}

void TestsWidget::summaries()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->summaries();
}

void TestsWidget::summaryAddTest()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->summaryAddTest();
}

void TestsWidget::on_selectAllTests()
{
    ui->wgtResult->selectAllTests();
}

void TestsWidget::on_unselectAllTests()
{
    ui->wgtResult->unSelectAllTests();
}

void TestsWidget::setEnabledButtons(const bool isEnabled)
{
    ui->btnTestPrint->setVisible(isEnabled);
    ui->btnSelectAllTests->setVisible(isEnabled);
    ui->btnUnSelectAllTests->setVisible(isEnabled);
    ui->btnSummaryAddTest->setVisible(isEnabled);
}

void TestsWidget::selectResult()
{
    ui->wgtDynamic->setVisible(false);
    ui->wgtResult->setVisible(true);
    setEnabledButtons(true);
    saveVisibleWidget(ui->wgtResult->widgetName());
}

void TestsWidget::selectDynamic()
{
    ui->wgtResult->setVisible(false);
    ui->wgtDynamic->setVisible(true);
    setEnabledButtons(false);
    saveVisibleWidget(ui->wgtDynamic->widgetName());
}

void TestsWidget::signalExport()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->signalExport();
}

void TestsWidget::restoreVisibleWidget()
{
    auto val = SettingsProvider::valueFromRegAppCopy("Geometry/ResultWidget", "SelectionWidget", ui->wgtResult->widgetName()).toString();
    if (val == ui->wgtResult->widgetName())
        ui->wgtDynamic->setVisible(false);
    else
    if (val == ui->wgtDynamic->widgetName())
        ui->wgtResult->setVisible(false);
    setEnabledButtons(val == ui->wgtResult->widgetName());
}

void TestsWidget::saveVisibleWidget(const QString &value)
{
    SettingsProvider::setValueToRegAppCopy("Geometry/ResultWidget", "SelectionWidget", value);
}

