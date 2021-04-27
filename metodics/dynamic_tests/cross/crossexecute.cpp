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
}

void CrossExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}

void CrossExecute::recording()
{

    StabDynamicTestExecute::recording();
}

