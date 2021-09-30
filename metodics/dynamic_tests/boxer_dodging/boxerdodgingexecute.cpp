#include "boxerdodgingexecute.h"

#include <QTimer>

BoxerDodgingExecute::BoxerDodgingExecute(QWidget *parent)
    : StabDynamicTestExecute (parent)
{
    setTitle(tr("Тренажер с уклонением для боксеров"));
    isShowValues(false);
    isTraceControl(false);
    setVisibleRecordLength(false);

    QTimer::singleShot(10, [=]
    {
        createMarker();
    });
}

BoxerDodgingExecute::~BoxerDodgingExecute()
{

}

void BoxerDodgingExecute::setParams(const QJsonObject &params)
{
    StabDynamicTestExecute::setParams(params);
}

StabDynamicTestPatientWindow *BoxerDodgingExecute::createPatientWindow()
{
//    m_patientWin = new OctaedronPatientWindow(static_cast<int>(m_radius * 1.2), m_targets, this);
//    return m_patientWin;

    return nullptr;
}

void BoxerDodgingExecute::finishTest()
{
    StabDynamicTestExecute::finishTest();
}

void BoxerDodgingExecute::fillSpecific(QFrame *frSpecific)
{
    StabDynamicTestExecute::fillSpecific(frSpecific);
}

void BoxerDodgingExecute::start()
{
    StabDynamicTestExecute::start();
}

void BoxerDodgingExecute::recording()
{
    StabDynamicTestExecute::recording();
}

void BoxerDodgingExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);
}

void BoxerDodgingExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}

void BoxerDodgingExecute::createMarker()
{
    addMarker();
}
