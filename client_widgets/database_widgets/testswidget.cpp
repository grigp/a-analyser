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
        QMessageBox::information(nullptr, tr("Предупрежение"), tr("Не выбран тест"));
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
        QMessageBox::information(nullptr, tr("Предупрежение"), tr("Не выбран тест"));
}

void TestsWidget::print( QPrinter* printer)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();
    qDebug() << "print" << paper << printer->pageSize() << printer->paperRect();

    painter->begin(printer);
    painter->setPen(Qt::black);
    painter->setBrush(QBrush(Qt::red));
    painter->drawRect(10, 10, 410, 410);
    painter->setFont(QFont("Sans",64,0,0));
    painter->drawText(QRect(0,0,3000,800), Qt::AlignLeft | Qt::AlignTop, "page1");
    painter->end();
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
    auto val = SettingsProvider::valueFromRegAppCopy("ResultWidget", "SelectionWidget", ui->wgtResult->widgetName()).toString();
    if (val == ui->wgtResult->widgetName())
        ui->wgtDynamic->setVisible(false);
    else
    if (val == ui->wgtDynamic->widgetName())
        ui->wgtResult->setVisible(false);
}

void TestsWidget::saveVisibleWidget(const QString &value)
{
    SettingsProvider::setValueToRegAppCopy("ResultWidget", "SelectionWidget", value);
}

