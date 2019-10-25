#include "resultswidget.h"
#include "ui_resultswidget.h"

#include "testsmodel.h"
#include "testproxymodel.h"
#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "settingsprovider.h"

#include <QPainter>
#include <QException>
#include <QTimer>
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
    connect(ui->tvTests->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ResultsWidget::on_selectionChanged);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
            &AAnalyserApplication::selectPatient, this, &ResultsWidget::on_selectPatient);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
            &AAnalyserApplication::selectMetodic, this, &ResultsWidget::on_selectMetodic);

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

void ResultsWidget::onDbDisconnect()
{
    closeTest();
    disconnect(m_mdlTest, &TestsModel::rowsInserted, this, &ResultsWidget::onNewTests);
    disconnect(m_mdlTest, &TestsModel::rowsRemoved, this, &ResultsWidget::onRemoveTests);
    m_mdlTest->clear();
}

void ResultsWidget::on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
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

void ResultsWidget::selectTest(const QModelIndex &index)
{
    try
    {
//        qDebug() << "1";
//        int v = 0;
//        v = 23 / v;
//        qDebug() << v;
        if (index.isValid())
        {
            auto uid = m_mdlTest->index(index.row(), TestsModel::ColPatient, index.parent()).
                    data(TestsModel::TestUidRole).toString();

            MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
            if (m_wgtResult)
                delete m_wgtResult;
            m_wgtResult = metFactory->visualize(ui->wgtResults, uid);
            ui->lblNoTest->setVisible(!m_wgtResult);

            static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectTest(uid);
        }
    }
    catch(QException &e)
    {
        qDebug() << "exception select test" << e.what();
        throw 1;
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

void ResultsWidget::on_selectPatient(const QString &patientUid)
{
    Q_UNUSED(patientUid);
    closeTestIfNotSelection();
    QTimer::singleShot(100, [=]
    {
        ui->tvTests->header()->resizeSections(QHeaderView::ResizeToContents);
    });
}

void ResultsWidget::on_selectMetodic(const QString &metodicUid)
{
    Q_UNUSED(metodicUid);
    closeTestIfNotSelection();
    QTimer::singleShot(100, [=]
    {
        ui->tvTests->header()->resizeSections(QHeaderView::ResizeToContents);
    });
}

void ResultsWidget::closeTestIfNotSelection()
{
    if (ui->tvTests->selectionModel()->selectedIndexes().size() == 0)
        closeTest();
}

void ResultsWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("ResultWidget", "SplitterPosition", ui->splitter->saveState());
}

void ResultsWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("ResultWidget", "SplitterPosition").toByteArray();
    ui->splitter->restoreState(val);
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

