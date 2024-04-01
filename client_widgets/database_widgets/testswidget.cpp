#include "testswidget.h"
#include "ui_testswidget.h"

#include <QApplication>
#include <QFile>
#include <QMessageBox>
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
    if (m_selectedTestUid != "")
    {
        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(m_selectedTestUid, ti))
        {
            MetodicDefines::MetodicInfo metInfo =
                    static_cast<AAnalyserApplication*>(QApplication::instance())->
                    getMetodics()->metodic(ti.metodUid);

            TestPropertyDialog dialog(this);
            dialog.setComment(ti.comment);
            dialog.setCondition(ti.condition);
            dialog.setNormContained(ti.isNormContained);
            dialog.setNormVisible(metInfo.buildNorms);
            if (dialog.exec() == QDialog::Accepted)
            {
                DataProvider::setTestProperty(m_selectedTestUid,
                                              dialog.comment(),
                                              dialog.condition(),
                                              dialog.normContained());
            }
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
}

void TestsWidget::printReport()
{
    if (m_selectedTestUid != "")
    {
        QPrinter printer(QPrinter::HighResolution);
        printer.setFullPage(true);
        QPrintPreviewDialog preview(&printer, this);
        preview.setWindowTitle(tr("Печать результатов теста"));
        preview.setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
        preview.setWindowState(Qt::WindowMaximized);
        connect(&preview, &QPrintPreviewDialog::paintRequested, this, &TestsWidget::print);
        preview.exec();
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
}

void TestsWidget::signalsAnalysis()
{
    if (!("" == m_selectedTestUid && 0 == static_cast<AAnalyserApplication*>(QApplication::instance())->saOpenedTestCount()))
        static_cast<AAnalyserApplication*>(QApplication::instance())->signalsAnalysis();
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест и в окне анализа сигналов отсутствуют ранее открытые тесты"));
}

void TestsWidget::summaries()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->summaries();
}

void TestsWidget::summaryAddTest()
{
    if (static_cast<AAnalyserApplication*>(QApplication::instance())->selectedTestsCount() > 0)
    {
        static_cast<AAnalyserApplication*>(QApplication::instance())->summaryAddTest();
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
}

void TestsWidget::on_selectAllTests()
{
    ui->wgtResult->selectAllTests();
}

void TestsWidget::on_unselectAllTests()
{
    ui->wgtResult->unSelectAllTests();
}

void TestsWidget::print(QPrinter* printer)
{
    MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
    metFactory->print(printer, m_selectedTestUid);
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
    if (static_cast<AAnalyserApplication*>(QApplication::instance())->selectedTestsCount() > 0)
    {
        QStringList tests;
        for (int i = 0; i < static_cast<AAnalyserApplication*>(QApplication::instance())->selectedTestsCount(); ++i)
            tests << static_cast<AAnalyserApplication*>(QApplication::instance())->selectedTest(i);
        auto exp = new SignalExporter(tests);
        delete exp;
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
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

