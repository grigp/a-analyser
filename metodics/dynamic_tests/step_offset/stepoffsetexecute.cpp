#include "stepoffsetexecute.h"
#include "ui_stepoffsetexecute.h"

#include "stepoffsettpatientwindow.h"

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
    m_force = params["force"].toInt();

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
    StabDynamicTestExecute::recording();
}

void StepOffsetExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);
}

void StepOffsetExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}
