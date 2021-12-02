#include "stepdeviationpatientwindow.h"

#include <QLayout>

StepDeviationPatientWindow::StepDeviationPatientWindow(QWidget *parent)
    : StabDynamicTestPatientWindow (parent)
{
    setVisibleMarker(false);
    setWindowTitle("Тест со ступенчатым отклонением");

    setInfoLabel();
}

StepDeviationPatientWindow::~StepDeviationPatientWindow()
{

}

void StepDeviationPatientWindow::run()
{
    StabDynamicTestPatientWindow::run();
    m_info->setVisible(true);
}

void StepDeviationPatientWindow::stop()
{
    StabDynamicTestPatientWindow::stop();
    m_info->setVisible(false);
}

void StepDeviationPatientWindow::setInfoLabel()
{
    m_info = new QLabel(this);
    m_info->setText(tr("Выполняйте отклонения в выбранном направлении с постоянно увеличивающейся амплитудой.\n Величина прироста должна быть минимальна"));
    m_info->setWordWrap(true);
    m_info->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    getWidgetLayout()->addWidget(m_info);
    m_info->setStyleSheet("font-size: 20pt; color:rgb(200, 200, 200)");
    m_info->setVisible(false);
}
