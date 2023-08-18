#include "stepdeviationpatientwindow.h"

#include <QLayout>
#include <QSpacerItem>

StepDeviationPatientWindow::StepDeviationPatientWindow(BaseDefines::Directions direction, QWidget *parent)
    : StabDynamicTestPatientWindow (parent)
    , m_direction(direction)
{
    setVisibleMarker(false);
    setWindowTitle("Тест со ступенчатым отклонением");

//    setInfoLabel();
}

StepDeviationPatientWindow::~StepDeviationPatientWindow()
{

}

void StepDeviationPatientWindow::run()
{
    StabDynamicTestPatientWindow::run();
    setInfoLabel();
    m_info->setVisible(true);
    m_infoDir->setVisible(true);
}

void StepDeviationPatientWindow::stop()
{
    StabDynamicTestPatientWindow::stop();
    m_info->setVisible(false);
    m_infoDir->setVisible(false);
}

void StepDeviationPatientWindow::setInfoLabel()
{
    auto spacerUp = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    static_cast<QBoxLayout*>(getWidgetLayout())->addSpacerItem(spacerUp);

    m_infoDir = new QLabel(this);
    getWidgetLayout()->addWidget(m_infoDir);
    m_infoDir->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_infoDir->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed)); //setMaximumSize(0, 200);
    m_infoDir->setVisible(false);
    if (m_direction == BaseDefines::dirUp)
        m_infoDir->setPixmap(QPixmap(":/images/up200.png"));
    else
    if (m_direction == BaseDefines::dirRight)
        m_infoDir->setPixmap(QPixmap(":/images/right200.png"));
    else
    if (m_direction == BaseDefines::dirDown)
        m_infoDir->setPixmap(QPixmap(":/images/down200.png"));
    else
    if (m_direction == BaseDefines::dirLeft)
        m_infoDir->setPixmap(QPixmap(":/images/left200.png"));


    m_info = new QLabel(this);
    m_info->setText(tr("Выполняйте отклонения в выбранном направлении с постоянно увеличивающейся амплитудой.\n Величина прироста должна быть минимальна"));
    m_info->setWordWrap(true);
    m_info->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    getWidgetLayout()->addWidget(m_info);
    m_info->setStyleSheet("font-size: 20pt; color:rgb(200, 200, 200)");
    m_info->setVisible(false);

    auto spacerDn = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    static_cast<QBoxLayout*>(getWidgetLayout())->addSpacerItem(spacerDn);
}
