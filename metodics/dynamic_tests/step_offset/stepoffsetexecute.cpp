#include "stepoffsetexecute.h"
#include "ui_stepoffsetexecute.h"

#include <QDebug>

#include "stepoffsettpatientwindow.h"
#include "setmaxforcedialog.h"

StepOffsetExecute::StepOffsetExecute(QWidget *parent) :
    StabDynamicTestExecute(parent),
    ui(new Ui::StepOffsetExecute)
{
    ui->setupUi(this);

    addTarget(0, 0, Qt::green, Qt::darkGreen);
    addMarker();

    setTitle("Тест \"Ступени\"");
    isShowValues(false);
    isTraceControl(false);
    setVisibleRecordLength(false);
}

StepOffsetExecute::~StepOffsetExecute()
{
    delete ui;
}

void StepOffsetExecute::setParams(const QJsonObject &params)
{
    StabDynamicTestExecute::setParams(params);

    m_stageTime = params["stage_time"].toInt();
    m_repeatCount = params["repeat_count"].toInt();
    m_forcePercent = params["force"].toInt();

    auto d = params["direction"].toString();
    m_direction = BaseUtils::DirectionValueIndex.value(d);
}

StabDynamicTestPatientWindow *StepOffsetExecute::createPatientWindow()
{
    return new StepOffsettPatientWindow(this);
}

void StepOffsetExecute::finishTest()
{
    StabDynamicTestExecute::finishTest();
}

void StepOffsetExecute::fillSpecific(QFrame *frSpecific)
{
    StabDynamicTestExecute::fillSpecific(frSpecific);
}

void StepOffsetExecute::start()
{
    StabDynamicTestExecute::start();
}

void StepOffsetExecute::recording()
{
    if(!isRecording())
    {
        if (!m_mfd)
        {
            m_mfd = new SetMaxForceDialog();
            m_mfd->setDirection(m_direction);
            connect(m_mfd, &SetMaxForceDialog::accepted, this, &StepOffsetExecute::setMaxForceDialogAccepted);
        }
        m_mfd->showFullScreen();
    }
    else
        StabDynamicTestExecute::recording();
}

void StepOffsetExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);
    if (m_mfd)
        m_mfd->getData(x(), y());
}

void StepOffsetExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}

void StepOffsetExecute::setMaxForceDialogAccepted()
{
    if (m_mfd)
    {
        m_force = m_mfd->value() * m_forcePercent / 100;
        StabDynamicTestExecute::recording();
    }
}
