#include "testswidget.h"
#include "ui_testswidget.h"

#include <QApplication>
#include <QFile>

#include "aanalyserapplication.h"
#include "executewidget.h"
#include "testsmodel.h"

TestsWidget::TestsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestsWidget)
  , m_mdlTest(new TestsModel(this))
{
    ui->setupUi(this);

    ui->tvTests->setModel(m_mdlTest);
}

TestsWidget::~TestsWidget()
{
    delete ui;
}

void TestsWidget::onDbConnect()
{
    if (m_mdlTest)
        m_mdlTest->load();
    ui->tvTests->header()->resizeSections(QHeaderView::ResizeToContents);
}

void TestsWidget::runTest()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->executeMetodic();
}

