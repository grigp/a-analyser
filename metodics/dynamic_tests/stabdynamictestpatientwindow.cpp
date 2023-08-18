#include "stabdynamictestpatientwindow.h"
#include "ui_stabdynamictestpatientwindow.h"

#include "dynamicteststimul.h"
#include "metodicdefines.h"

StabDynamicTestPatientWindow::StabDynamicTestPatientWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StabDynamicTestPatientWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::darkGray);
    setAutoFillBackground(true);
    setPalette(pal);

    ui->lblFrontComment->setStyleSheet(MetodicDefines::AutoModeMessageStylePatient);
    setFrontComment("");
}

StabDynamicTestPatientWindow::~StabDynamicTestPatientWindow()
{
    delete ui;
}

void StabDynamicTestPatientWindow::run()
{

}

void StabDynamicTestPatientWindow::stop()
{

}

void StabDynamicTestPatientWindow::setDiap(const int diap)
{
    m_diap = diap;
    ui->wgtStimul->setDiap(diap);
}

void StabDynamicTestPatientWindow::setMarker(const double x, const double y)
{
    m_X = x;
    m_Y = y;
    ui->wgtStimul->setMarker(x, y);
}

void StabDynamicTestPatientWindow::setVisibleMarker(const bool isVisible)
{
    ui->wgtStimul->setVisibleMarker(isVisible);
}

bool StabDynamicTestPatientWindow::isVisibleMarker() const
{
    return ui->wgtStimul->visibleMarker();
}

void StabDynamicTestPatientWindow::initTargets(const int count, const QColor color, const int size)
{
    ui->wgtStimul->initTargets(count, color, size);
}

void StabDynamicTestPatientWindow::addTarget(const double x, const double y, const QColor color, const int size, const QString label)
{
    ui->wgtStimul->addTarget(x, y, color, size, label);
}

void StabDynamicTestPatientWindow::setTargetInfo(const int idx, const QColor color, const int size, const QString label)
{
    ui->wgtStimul->setTargetInfo(idx, color, size, label);
}

void StabDynamicTestPatientWindow::setTarget(const double x, const double y, const int idx )
{
    ui->wgtStimul->setTarget(x, y, idx);
}

void StabDynamicTestPatientWindow::clearTargets()
{
    ui->wgtStimul->clearTargets();
}

QLayout *StabDynamicTestPatientWindow::getWidgetLayout()
{
    return ui->wgtStimul->layout();
}

void StabDynamicTestPatientWindow::setFrontComment(const QString &comment)
{
    ui->lblFrontComment->setText(comment);
    ui->lblFrontComment->setVisible(comment != "");
}

void StabDynamicTestPatientWindow::resizeEvent(QResizeEvent *event)
{
//    ui->lblFrontComment->setGeometry(ui->wgtStimul->geometry());
    QDialog::resizeEvent(event);
}

