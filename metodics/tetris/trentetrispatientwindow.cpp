#include "trentetrispatientwindow.h"
#include "ui_trentetrispatientwindow.h"

TrenTetrisPatientWindow::TrenTetrisPatientWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrenTetrisPatientWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->lblFullPicture->setVisible(false);
}

TrenTetrisPatientWindow::~TrenTetrisPatientWindow()
{
    delete ui;
}

void TrenTetrisPatientWindow::setScene(QGraphicsScene *scene)
{
    ui->gvGame->setScene(scene);
}

double TrenTetrisPatientWindow::prop() const
{
    return  m_prop;
}

double TrenTetrisPatientWindow::propX() const
{
    return  m_propX;
}

double TrenTetrisPatientWindow::propY() const
{
    return m_propY;
}

void TrenTetrisPatientWindow::setScore(const QString &score)
{
    ui->lblGameScore->setText(score);
}

void TrenTetrisPatientWindow::setDeleteRows(const QString &rows)
{
    ui->lblGameScore->setText(rows);
}

void TrenTetrisPatientWindow::setSamplePixmap(const QPixmap &pixmap)
{
    ui->lblFullPicture->setPixmap(pixmap.scaled(ui->frParams->geometry().width(), ui->frParams->geometry().width()));
    ui->lblFullPicture->setVisible(true);
}

QSize TrenTetrisPatientWindow::sceneSize() const
{
    return ui->gvGame->geometry().size();
}

void TrenTetrisPatientWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    int sideSize = ui->gvGame->geometry().size().height();
    if (ui->gvGame->geometry().size().width() < ui->gvGame->geometry().size().height())
        sideSize = ui->gvGame->geometry().size().width();
    if (ui->gvGame->scene())
        ui->gvGame->scene()->setSceneRect(-ui->gvGame->geometry().size().width() / 2,
                                          - ui->gvGame->geometry().size().height() / 2,
                                          ui->gvGame->geometry().size().width(),
                                          ui->gvGame->geometry().size().height());
//        ui->gvGame->scene()->setSceneRect(-ui->gvGame->geometry().size().width() * 0.995 / 2,
//                                          - ui->gvGame->geometry().size().height() * 0.995 / 2,
//                                          ui->gvGame->geometry().size().width() * 0.995,
//                                          ui->gvGame->geometry().size().height() * 0.995);
    m_prop = static_cast<double>(sideSize) / 2000;
    m_propX = static_cast<double>(ui->gvGame->geometry().size().width()) / 2000;
    m_propY = static_cast<double>(ui->gvGame->geometry().size().height()) / 2000;
}
