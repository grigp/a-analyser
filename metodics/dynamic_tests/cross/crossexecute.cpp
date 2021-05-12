#include "crossexecute.h"
#include "ui_crossexecute.h"

#include "crosspatientwindow.h"

#include <QDebug>

CrossExecute::CrossExecute(QWidget *parent) :
    StabDynamicTestExecute(parent),
    ui(new Ui::CrossExecute)
{
    ui->setupUi(this);

    addTarget(0, 0, Qt::green, Qt::darkGreen);
    addMarker();

    setTitle("Тест на устойчивость");
    isShowValues(false);
    isTraceControl(false);
}

CrossExecute::~CrossExecute()
{
    delete ui;
}

void CrossExecute::setParams(const QJsonObject &params)
{
    m_stageTime = params["stage_time"].toInt();
    m_repeatCount = params["repeat_count"].toInt();
    m_centerSize = params["center_size"].toInt();
    m_delayTime = params["delay_time"].toInt();

    auto csm = params["change_stage_mode"].toString();
    m_changeStateMode = CrossDefines::ChangeStateModeValueIndex.value(csm);
    auto dm = params["direction_mode"].toString();
    m_directionMode = CrossDefines::DirectionModeValueIndex.value(dm);

    StabDynamicTestExecute::setParams(params);
}

StabDynamicTestPatientWindow *CrossExecute::createPatientWindow()
{
    return new CrossPatientWindow(this);
}

void CrossExecute::start()
{
    StabDynamicTestExecute::start();
}

void CrossExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);

//    setTarget(0, n);

}

void CrossExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}

bool CrossExecute::newDirection()
{
    if (m_curDirection != CrossDefines::dirNone)
        m_stagesProcess[m_curDirection]++;

    if (m_directionMode == CrossDefines::dmClockwise)
    {
        if (m_curDirection < CrossDefines::dirLeft)
        {
            int d = static_cast<int>(m_curDirection);
            d++;
            m_curDirection = static_cast<CrossDefines::Directions>(d);
        }
        else
            m_curDirection = CrossDefines::dirUp;
    }
    else
    if (m_directionMode == CrossDefines::dmCounterClockwise)
    {
        if (m_curDirection > CrossDefines::dirUp)
        {
            int d = static_cast<int>(m_curDirection);
            d--;
            m_curDirection = static_cast<CrossDefines::Directions>(d);
        }
        else
            m_curDirection = CrossDefines::dirLeft;
    }
    else
    if (m_directionMode == CrossDefines::dmRandom)
    {
        int d = qrand() % 4;

    }
}

void CrossExecute::recording()
{

    StabDynamicTestExecute::recording();
}

