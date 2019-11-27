#include "trentakeputpatientwindow.h"
#include "ui_trentakeputpatientwindow.h"

#include <QDebug>

TrenTakePutPatientWindow::TrenTakePutPatientWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrenTakePutPatientWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
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

void TrenTakePutPatientWindow::setScore(const QString &score)
{
    ui->lblGameScore->setText(score);
}

void TrenTakePutPatientWindow::setErrors(const QString &errors)
{
    ui->lblGameErrors->setText(errors);
}

void TrenTakePutPatientWindow::resizeEvent(QResizeEvent *event)
{
    int size = event->size().height();
    if (event->size().width() < event->size().height())
        size = event->size().width();
    if (ui->gvGame->scene())
        ui->gvGame->scene()->setSceneRect(-size / 2, - size / 2, size, size);
    m_prop = static_cast<double>(size) / 2000;
}
