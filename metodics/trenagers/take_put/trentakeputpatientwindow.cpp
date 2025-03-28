#include "trentakeputpatientwindow.h"
#include "ui_trentakeputpatientwindow.h"

#include <QDebug>

TrenTakePutPatientWindow::TrenTakePutPatientWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrenTakePutPatientWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->lblFullPicture->setVisible(false);
}

TrenTakePutPatientWindow::~TrenTakePutPatientWindow()
{
    delete ui;
}

void TrenTakePutPatientWindow::setScene(QGraphicsScene *scene)
{
    ui->gvGame->setScene(scene);
}

double TrenTakePutPatientWindow::prop() const
{
    return m_prop;
}

double TrenTakePutPatientWindow::propX() const
{
    return m_propX;
}

double TrenTakePutPatientWindow::propY() const
{
    return m_propY;
}

void TrenTakePutPatientWindow::setScore(const QString &score)
{
    ui->lblGameScore->setText(score);
}

void TrenTakePutPatientWindow::setErrors(const QString &errors)
{
    ui->lblGameErrors->setText(errors);
}

void TrenTakePutPatientWindow::setSamplePixmap(const QPixmap &pixmap)
{
    ui->lblFullPicture->setPixmap(pixmap.scaled(ui->frParams->geometry().width(), ui->frParams->geometry().width()));
    ui->lblFullPicture->setVisible(true);
}

QSize TrenTakePutPatientWindow::sceneSize() const
{
    return ui->gvGame->geometry().size();
}

void TrenTakePutPatientWindow::resizeEvent(QResizeEvent *event)
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
