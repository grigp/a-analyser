#include "stepdeviationtestexecute.h"

#include "aanalyserapplication.h"
#include "stepdeviationpatientwindow.h"
#include "testresultdata.h"

#include <QTimer>
#include <QDebug>

StepDeviationTestExecute::StepDeviationTestExecute(QWidget *parent)
    : StabDynamicTestExecute (parent)
{
    auto kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
    setTitle(tr("Тест со ступенчатым отклонением") + " - " + kard.fio);
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
    setRecordLength(m_maxTime * freqStab());

    auto d = params["direction"].toString();
    m_direction = BaseDefines::DirectionValueIndex.value(d);
}

StabDynamicTestPatientWindow *StepDeviationTestExecute::createPatientWindow()
{
    return new StepDeviationPatientWindow(m_direction, this);
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

    if (recCount() >= m_maxTime * freqStab())
        finishTest();
}

void StepDeviationTestExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}
