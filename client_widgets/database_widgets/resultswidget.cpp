#include "resultswidget.h"
#include "ui_resultswidget.h"

#include "testsmodel.h"
#include "testproxymodel.h"
#include "aanalyserapplication.h"
#include "metodicsfactory.h"

#include <QPainter>
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
    ui->tvTests->sortByColumn(TestsModel::colDateTime, Qt::DescendingOrder);
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
                selectTest(idx);
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

