#include "testswidget.h"
#include "ui_testswidget.h"

#include <QApplication>
#include <QFile>
#include "aanalyserapplication.h"
#include "executewidget.h"

TestsWidget::TestsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestsWidget)
{
    ui->setupUi(this);
}

TestsWidget::~TestsWidget()
{
    delete ui;
}

void TestsWidget::runTest()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->executeMetodic();
}

