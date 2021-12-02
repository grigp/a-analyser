#include "stepdeviationtestexecute.h"

#include "aanalyserapplication.h"
#include "stepdeviationpatientwindow.h"
#include "testresultdata.h"

#include <QTimer>

StepDeviationTestExecute::StepDeviationTestExecute(QWidget *parent)
    : StabDynamicTestExecute (parent)
{
    auto kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
    setTitle(tr("Тренажер с уклонением для боксеров") + " - " + kard.fio);
    isShowValues(false);
    isTraceControl(false);

    QTimer::singleShot(10, [=]
    {
        addMarker();
    });

}

StepDeviationTestExecute::~StepDeviationTestExecute()
{

}

void StepDeviationTestExecute::setParams(const QJsonObject &params)
{
    StabDynamicTestExecute::setParams(params);
    m_maxTime = params["max_time"].toInt();

    auto d = params["direction"].toString();
    m_direction = BaseUtils::DirectionValueIndex.value(d);
}

StabDynamicTestPatientWindow *StepDeviationTestExecute::createPatientWindow()
{
    return new StepDeviationPatientWindow(this);

}

void StepDeviationTestExecute::finishTest()
{
//    trd()->addChannel(m_res);

    StabDynamicTestExecute::finishTest();
}

void StepDeviationTestExecute::fillSpecific(QFrame *frSpecific)
{
    StabDynamicTestExecute::fillSpecific(frSpecific);

}

void StepDeviationTestExecute::start()
{
    StabDynamicTestExecute::start();
}

void StepDeviationTestExecute::recording()
{
    StabDynamicTestExecute::recording();
}

void StepDeviationTestExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);
}

void StepDeviationTestExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}
