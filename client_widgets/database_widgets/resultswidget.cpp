#include "resultswidget.h"
#include "ui_resultswidget.h"

#include "testsmodel.h"
#include "testproxymodel.h"
#include "aanalyserapplication.h"
#include "metodicsfactory.h"

#include <QPainter>
#include <QSettings>
#include <QDebug>

ResultsWidget::ResultsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultsWidget)
  , m_mdlTest(new TestsModel(this))
  , m_pmdlTest(new TestProxyModel(this))
{
    ui->setupUi(this);

    m_pmdlTest->setSourceModel(m_mdlTest);
    ui->tvTests->setModel(m_pmdlTest);
    ui->tvTests->viewport()->installEventFilter(this);

    restoreSplitterPosition();

    //    m_pmp = new ScaledPixmap(this);
    //    ui->wgtResults->layout()->addWidget(m_pmp);
    //    ui->wgtResults->layout()->setMargin(1);

    //    QPixmap p(":/images/WallPaper.png");
    //    m_pmp->setScaledPixmap(p);
    //    ui->lblNoTest->setVisible(false);

}

ResultsWidget::~ResultsWidget()
{
    delete ui;
}

void ResultsWidget::onDbConnect()
{
    if (m_mdlTest)
        m_mdlTest->load();
    ui->tvTests->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvTests->sortByColumn(TestsModel::ColDateTime, Qt::DescendingOrder);

    connect(m_mdlTest, &TestsModel::rowsInserted, this, &ResultsWidget::onNewTests);
    connect(m_mdlTest, &TestsModel::rowsRemoved, this, &ResultsWidget::onRemoveTests);

}

bool ResultsWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tvTests->viewport())
    {
        if (event->type() == QEvent::Paint)
        {
            // Приводит к частым срабатываниям
            auto idx = m_pmdlTest->mapToSource(ui->tvTests->selectionModel()->currentIndex());
            if (idx.row() != m_selectedRow)
            {
                if (idx.row() > -1)
                    selectTest(idx);
                else
                    closeTest();
            }
            m_selectedRow = idx.row();
        }
    }
    return false;
}

void ResultsWidget::selectTest(const QModelIndex &index)
{
    if (index.isValid())
    {
        auto uid = m_mdlTest->index(index.row(), TestsModel::ColPatient, index.parent()).
                data(TestsModel::TestUidRole).toString();
        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectTest(uid);

        MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
        if (m_wgtResult)
            delete m_wgtResult;
        m_wgtResult = metFactory->visualize(ui->wgtResults, uid);
        ui->lblNoTest->setVisible(!m_wgtResult);
    }
}

void ResultsWidget::closeTest()
{
    if (m_wgtResult)
        delete m_wgtResult;
    m_wgtResult = nullptr;
    ui->lblNoTest->setVisible(!m_wgtResult);
}

void ResultsWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void ResultsWidget::onNewTests(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(last);
    auto index = m_mdlTest->index(first, TestsModel::ColPatient, parent);
    selectTest(index);
    auto indexDT = m_mdlTest->index(index.row(), TestsModel::ColDateTime, index.parent());
    auto proxyIdx = m_pmdlTest->mapFromSource(indexDT);
    ui->tvTests->selectionModel()->select(proxyIdx, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void ResultsWidget::onRemoveTests(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
    m_selectedRow = -1;
    if (m_wgtResult)
        delete m_wgtResult;
    m_wgtResult = nullptr;
    ui->lblNoTest->setVisible(true);
    ui->tvTests->selectionModel()->clearSelection();
}

void ResultsWidget::saveSplitterPosition()
{
    QSettings set(QApplication::instance()->organizationName(),
                  QApplication::instance()->applicationName());
    set.beginGroup("ResultWidget");
    set.setValue("SplitterPosition", ui->splitter->saveState());
    set.endGroup();
}

void ResultsWidget::restoreSplitterPosition()
{
    QSettings set(QApplication::instance()->organizationName(),
                  QApplication::instance()->applicationName());
    set.beginGroup("ResultWidget");
    auto val = set.value("SplitterPosition");
    set.endGroup();
    ui->splitter->restoreState(val.toByteArray());
}

ScaledPixmap::ScaledPixmap(QWidget *parent)
{
    Q_UNUSED(parent);

}

void ScaledPixmap::setScaledPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    update();
}

void ScaledPixmap::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (false == m_pixmap.isNull()) {
      QSize widgetSize = size();
      QPixmap scaledPixmap = m_pixmap.scaled(widgetSize, Qt::KeepAspectRatio);
      QPoint center((widgetSize.width() - scaledPixmap.width())/2,
                    (widgetSize.height() - scaledPixmap.height())/2);
      painter.drawPixmap(center, scaledPixmap);
    }
    QWidget::paintEvent(event);
}

