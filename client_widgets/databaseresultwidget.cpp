#include "databaseresultwidget.h"
#include "ui_databaseresultwidget.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "metodicsfactory.h"

#include <QPainter>
#include <QDebug>

DataBaseResultWidget::DataBaseResultWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::DataBaseResultWidget)
{
    ui->setupUi(this);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::selectTest,
            this, &DataBaseResultWidget::onSelectTest);

//    m_pmp = new ScaledPixmap(this);
//    ui->wgtResults->layout()->addWidget(m_pmp);
//    ui->wgtResults->layout()->setMargin(1);

//    QPixmap p(":/images/WallPaper.png");
//    m_pmp->setScaledPixmap(p);
//    ui->lblNoTest->setVisible(false);

}

DataBaseResultWidget::~DataBaseResultWidget()
{
    delete ui;
}

QString DataBaseResultWidget::uid()
{
    return ClientWidgets::uidDatabaseResultWidgetUid;
}

QString DataBaseResultWidget::name()
{
    return tr("База данных");
}

void DataBaseResultWidget::onDBConnect()
{
    ui->wgtPatients->onDbConnect();
    ui->wgtMetods->onDbConnect();
    ui->wgtTests->onDbConnect();
}

void DataBaseResultWidget::onDBDisconnect()
{

}

void DataBaseResultWidget::onSelectTest(const QString &testUid)
{
    MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
    if (m_wgtResult)
        delete m_wgtResult;
    m_wgtResult = metFactory->visualize(ui->wgtResults, testUid);
    ui->lblNoTest->setVisible(!m_wgtResult);
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
