#include "trenagerpatientwindow.h"
#include "ui_trenagerpatientwindow.h"

#include <QDebug>

TrenagerPatientWindow::TrenagerPatientWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrenagerPatientWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
}

TrenagerPatientWindow::~TrenagerPatientWindow()
{
    delete ui;
}

void TrenagerPatientWindow::setScene(QGraphicsScene *scene)
{
    ui->gvGame->setScene(scene);
}

double TrenagerPatientWindow::prop() const
{
    return  m_prop;
}

double TrenagerPatientWindow::propX() const
{
    return  m_propX;
}

double TrenagerPatientWindow::propY() const
{
    return m_propY;
}

QSize TrenagerPatientWindow::sceneSize() const
{
    return ui->gvGame->geometry().size();
}

void TrenagerPatientWindow::setGameParamLabel(const QString text, const QString styleSheet)
{
    auto* label = new QLabel(ui->frGameParams);
    label->setText(text);
    label->setStyleSheet(styleSheet);
    ui->frGameParams->layout()->addWidget(label);
    m_gameParamLabels << label;
}

void TrenagerPatientWindow::setGameParamLabelValue(const int idxParam, const QString value)
{
    Q_ASSERT(idxParam >= 0 && idxParam < m_gameParamLabels.size());
    m_gameParamLabels.at(idxParam)->setText(value);
}

void TrenagerPatientWindow::addHintWidget(QWidget *widget)
{
    widget->setParent(ui->frGameHints);
    ui->frGameHints->layout()->addWidget(widget);
}

int TrenagerPatientWindow::getFrameParamsWidth()
{
    return  ui->frParams->width();
}

void TrenagerPatientWindow::resizeEvent(QResizeEvent *event)
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
    m_prop = static_cast<double>(sideSize) / 2000;
    m_propX = static_cast<double>(ui->gvGame->geometry().size().width()) / 2000;
    m_propY = static_cast<double>(ui->gvGame->geometry().size().height()) / 2000;
}
