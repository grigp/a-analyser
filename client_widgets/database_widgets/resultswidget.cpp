#include "resultswidget.h"
#include "ui_resultswidget.h"

#include "testsmodel.h"
#include "testproxymodel.h"
#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "settingsprovider.h"
#include "dataprovider.h"
#include "databasewigetdefines.h"

#include <QPainter>
#include <QException>
#include <QTimer>
#include <QDebug>

namespace
{
static const int WidthColumn0 = 250;
static const int WidthColumn1 = 50;

}

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
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
            &AAnalyserApplication::updatePatientData, m_mdlTest, &TestsModel::on_updatePatientData);

    restoreSplitterPosition();
    ui->wgtBugTest->setVisible(false);

    ui->tvTests->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tvTests, &QTreeView::customContextMenuRequested, this, &ResultsWidget::on_popupMenuRequested);


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
    ui->tvTests->header()->resizeSection(0, WidthColumn0);
    ui->tvTests->header()->resizeSection(1, WidthColumn1);
    ui->tvTests->sortByColumn(DatabaseWidgetDefines::TestsModel::ColDateTime, Qt::DescendingOrder);

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

void ResultsWidget::selectAllTests()
{
    ui->tvTests->selectAll();
    calculateSelected();
}

void ResultsWidget::unSelectAllTests()
{
    ui->tvTests->selectionModel()->clearSelection();
    calculateSelected();
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
        calculateSelected();

//        qDebug() << "1";
//        int v = 0;
//        v = 23 / v;
//        qDebug() << v;
        if (index.isValid())
        {
            auto uid = m_mdlTest->index(index.row(), DatabaseWidgetDefines::TestsModel::ColPatient, index.parent()).
                    data(DatabaseWidgetDefines::TestsModel::TestUidRole).toString();
            openTest(uid);
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
    m_selectedRow = -1;
    ui->wgtNoTest->setVisible(!m_wgtResult);
    static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectTest("");
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
    auto index = m_mdlTest->index(first, DatabaseWidgetDefines::TestsModel::ColPatient, parent);
    selectTest(index);
    auto indexDT = m_mdlTest->index(index.row(), DatabaseWidgetDefines::TestsModel::ColDateTime, index.parent());
    auto proxyIdx = m_pmdlTest->mapFromSource(indexDT);
    ui->tvTests->selectionModel()->select(proxyIdx, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void ResultsWidget::onRemoveTests(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
//    m_selectedRow = -1;
    closeTest();
//    ui->tvTests->selectionModel()->clearCurrentIndex();
}

void ResultsWidget::on_selectPatient(const QString &patientUid)
{
    Q_UNUSED(patientUid);
    closeTestIfNotSelection();
    QTimer::singleShot(100, [=]
    {
        ui->tvTests->header()->resizeSections(QHeaderView::ResizeToContents);
        ui->tvTests->header()->resizeSection(0, WidthColumn0);
        ui->tvTests->header()->resizeSection(1, WidthColumn1);
    });
}

void ResultsWidget::on_selectMetodic(const QString &metodicUid)
{
    Q_UNUSED(metodicUid);
    closeTestIfNotSelection();
    QTimer::singleShot(100, [=]
    {
        ui->tvTests->header()->resizeSections(QHeaderView::ResizeToContents);
        ui->tvTests->header()->resizeSection(0, WidthColumn0);
        ui->tvTests->header()->resizeSection(1, WidthColumn1);
        ui->tvTests->selectionModel()->clearSelection();
    });
}

void ResultsWidget::onEntered(QModelIndex index)
{
    if (index.isValid())
    {
        auto idx = m_pmdlTest->mapToSource(ui->tvTests->model()->index(index.row(),
                                                                       DatabaseWidgetDefines::TestsModel::ColPatient,
                                                                       index.parent()));
        auto uid = m_mdlTest->index(idx.row(),
                                    DatabaseWidgetDefines::TestsModel::ColPatient,
                                    idx.parent()).data(DatabaseWidgetDefines::TestsModel::TestUidRole).toString();

        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(uid, ti))
        {
            QString condition = "";
            DataDefines::TestConditionInfo ci;
            if (static_cast<AAnalyserApplication*>(QApplication::instance())->getTestConditionInfo(ti.condition, ci))
                condition = ci.name;
            auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(ti.metodUid);

            QString resume = tr("Комментарий") + ":\n" + ti.comment;
            if (mi.buildNorms)
                resume = resume + "\n\n" + tr("Условия проведения") + ": " + condition;

            ui->teComment->clear();
            ui->teComment->append(resume);
//            ui->tvTests->setToolTip(tr("Комментарий") + ":\n" + ti.comment + "\n\n" + tr("Условия проведения") + ": " + condition);
        }
    }
}

void ResultsWidget::onPressButtonOpenTestAgain()
{
    DataProvider::setTestIsOpening(m_uidOpenedTest, false);  //! Помечаем тест, что тест открыт успешно
    openTest(m_uidOpenedTest);                               //! Открываем тест
}

void ResultsWidget::onPressButtonRemoveBadTest()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->deleteTest();
}

void ResultsWidget::on_popupMenuRequested(QPoint pos)
{
    if (!m_menu)
    {
        m_menu = new QMenu(this);

        QAction *newTest = new QAction(QIcon(":/images/ResultsRun.png"), tr("Провести тест"), this);
        connect(newTest, &QAction::triggered, this, &ResultsWidget::runTest);
        m_menu->addAction(newTest);

        QAction *delTest = new QAction(QIcon(":/images/ResultDelete.png"), tr("Удалить тест..."), this);
        connect(delTest, &QAction::triggered, this, &ResultsWidget::deleteTest);
        m_menu->addAction(delTest);

        QAction *editTestProperty = new QAction(QIcon(":/images/TestProperty.png"), tr("Свойства теста..."), this);
        connect(editTestProperty, &QAction::triggered, this, &ResultsWidget::editTestProperty);
        m_menu->addAction(editTestProperty);

        m_menu->addSeparator();

        QAction *printTest = new QAction(QIcon(":/images/Print.png"), tr("Печать результатов теста..."), this);
        connect(printTest, &QAction::triggered, this, &ResultsWidget::printTest);
        m_menu->addAction(printTest);

        QAction *sigAnal = new QAction(QIcon(":/images/SignalAnalysis.png"), tr("Анализ сигналов теста..."), this);
        connect(sigAnal, &QAction::triggered, this, &ResultsWidget::signalsAnalysis);
        m_menu->addAction(sigAnal);

        m_menu->addSeparator();

        QAction *addToSummary = new QAction(QIcon(":/images/SummaryAddTest.png"), tr("Добавить показатели в сводку..."), this);
        connect(addToSummary, &QAction::triggered, this, &ResultsWidget::addToSummary);
        m_menu->addAction(addToSummary);

        QAction *signalsExport = new QAction(QIcon(":/images/SignalExport.png"), tr("Экспорт сигналов..."), this);
        connect(signalsExport, &QAction::triggered, this, &ResultsWidget::signalsExport);
        m_menu->addAction(signalsExport);

        m_menu->addSeparator();

        QAction *selectAll = new QAction(QIcon(""), tr("Пометить все..."), this);
        connect(selectAll, &QAction::triggered, this, &ResultsWidget::selectAll);
        m_menu->addAction(selectAll);

        QAction *unSelectAll = new QAction(QIcon(""), tr("Снять пометку со всех..."), this);
        connect(unSelectAll, &QAction::triggered, this, &ResultsWidget::unSelectAll);
        m_menu->addAction(unSelectAll);
    }
    m_menu->popup(ui->tvTests->mapToGlobal(pos));
}

void ResultsWidget::runTest()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->executeMetodic();
}

void ResultsWidget::deleteTest()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->deleteTest();
}

void ResultsWidget::editTestProperty()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->editTestProperty();
}

void ResultsWidget::printTest()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->printTestReport();
}

void ResultsWidget::signalsAnalysis()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->signalsTestAnalysis();
}

void ResultsWidget::addToSummary()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->summaryAddTest();
}

void ResultsWidget::signalsExport()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->signalExport();
}

void ResultsWidget::selectAll()
{
    selectAllTests();
}

void ResultsWidget::unSelectAll()
{
    unSelectAllTests();
}

void ResultsWidget::openTest(const QString testUid)
{
    MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
    if (m_wgtResult)
        delete m_wgtResult;
    m_wgtResult = nullptr;

    m_uidOpenedTest = testUid;

    //! Если тест с этим uid находится в процессе открытия isTestOpened(uid) == true, то
    //! есть подозрения, что он прошлый раз открылся со сбоем и его открывать опасно.
    //! Поэтому его не открываем
    if (!DataProvider::isTestOpening(testUid))
    {
        DataProvider::setTestIsOpening(testUid, true);   //! Помечаем тест, как в процессе открытия

        m_wgtResult = metFactory->visualize(ui->wgtResults, testUid);
        ui->wgtNoTest->setVisible(!m_wgtResult);
        ui->wgtBugTest->setVisible(false);

        //! Запомнить указатель на виджет визуализатора
        static_cast<AAnalyserApplication*>(QCoreApplication::instance())->clearOpenedTestList();
        static_cast<AAnalyserApplication*>(QCoreApplication::instance())->addOpenedTest(testUid, m_wgtResult);

        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectTest(testUid);
        DataProvider::setTestIsOpening(testUid, false);  //! Помечаем тест, что он открыт успешно
    }
    else
    {
        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectBugTest(testUid);
        ui->wgtBugTest->setVisible(true);
    }
}

void ResultsWidget::closeTestIfNotSelection()
{
    closeTest();
//    if (ui->tvTests->selectionModel()->selectedIndexes().size() == 0) Непонятно, для чего это вводилось, но при этом результаты не очищались
//        closeTest();
}

void ResultsWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("Geometry/ResultWidget", "SplitterPosition", ui->splCommon->saveState());
    SettingsProvider::setValueToRegAppCopy("Geometry/ResultWidget", "SplitterTestPosition", ui->splTest->saveState());
}

void ResultsWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("Geometry/ResultWidget", "SplitterPosition").toByteArray();
    if (val == "")
        val = QByteArray::fromRawData("\x00\x00\x00\xFF\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x01\x17\x00\x00\x01\xEB\x01\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x01\x00", 31);
    ui->splCommon->restoreState(val);

    auto valTst = SettingsProvider::valueFromRegAppCopy("Geometry/ResultWidget", "SplitterTestPosition").toByteArray();
    ui->splTest->restoreState(valTst);
}

void ResultsWidget::calculateSelected()
{
    auto indexes = ui->tvTests->selectionModel()->selectedIndexes();
    QStringList tests;
    foreach (auto index, indexes)
    {
        auto idx = m_pmdlTest->mapToSource(index);
        auto uid = m_mdlTest->index(idx.row(), DatabaseWidgetDefines::TestsModel::ColPatient, idx.parent()).
                data(DatabaseWidgetDefines::TestsModel::TestUidRole).toString();
        if (!tests.contains(uid))
            tests << uid;
        DataDefines::TestInfo ti;
        DataProvider::getTestInfo(uid, ti);
    }
    static_cast<AAnalyserApplication*>(QApplication::instance())->setSelectedTests(tests);
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

