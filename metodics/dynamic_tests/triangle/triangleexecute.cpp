#include "triangleexecute.h"

#include <QTimer>

#include "aanalyserapplication.h"
#include "trianglepatientwindow.h"

TriangleExecute::TriangleExecute(QWidget *parent)
    : StabDynamicTestExecute (parent)
{
    auto kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
    setTitle(tr("Тест \"Треугольник\"") + " - " + kard.fio);
    isShowValues(false);
    isTraceControl(false);

    QTimer::singleShot(10, [=]
    {
        addMarker();
    });

}

TriangleExecute::~TriangleExecute()
{

}

void TriangleExecute::setParams(const QJsonObject &params)
{
    auto dm = params["direction_mode"].toString();
    m_directionMode = BaseUtils::DirectionModeValueIndex.value(dm);

    m_force = params["force"].toInt();
    m_stageTime = params["stage_time"].toInt();
    m_minDeviation = params["min_deviation"].toInt();
    m_showMarkerAnal = params["show_marker_anal"].toBool();
}

StabDynamicTestPatientWindow *TriangleExecute::createPatientWindow()
{
    return new TrianglePatientWindow(this);
}

void TriangleExecute::finishTest()
{
    StabDynamicTestExecute::finishTest();
}

void TriangleExecute::fillSpecific(QFrame *frSpecific)
{
    StabDynamicTestExecute::fillSpecific(frSpecific);
}

void TriangleExecute::start()
{
    StabDynamicTestExecute::start();
}

void TriangleExecute::recording()
{
    StabDynamicTestExecute::recording();
}

void TriangleExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);
}

void TriangleExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}
