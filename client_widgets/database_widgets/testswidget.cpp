#include "testswidget.h"
#include "ui_testswidget.h"

#include <QApplication>
#include <QFile>

#include "aanalyserapplication.h"
#include "executewidget.h"
#include "testsmodel.h"
#include "testproxymodel.h"

TestsWidget::TestsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestsWidget)
  , m_mdlTest(new TestsModel(this))
  , m_pmdlTest(new TestProxyModel(this))
{
    ui->setupUi(this);

    m_pmdlTest->setSourceModel(m_mdlTest);
    ui->tvTests->setModel(m_pmdlTest);
    ui->tvTests->viewport()->installEventFilter(this);
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
    ui->tvTests->sortByColumn(TestsModel::colDateTime, Qt::DescendingOrder);
}

bool TestsWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tvTests->viewport())
    {
        if (event->type() == QEvent::Paint)
        {
            // Приводит к частым срабатываниям
            auto idx = m_pmdlTest->mapToSource(ui->tvTests->selectionModel()->currentIndex());
            selectTest(idx);
        }
    }
    return false;
}

void TestsWidget::runTest()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->executeMetodic();
}

void TestsWidget::selectTest(const QModelIndex &index)
{
    if (index.isValid())
    {
        auto uid = m_mdlTest->index(index.row(), TestsModel::ColPatient, index.parent()).
                data(TestsModel::TestUidRole).toString();
        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectTest(uid);
    }
}

