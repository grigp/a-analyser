#include "triangleexecute.h"

#include <QTimer>

#include "aanalyserapplication.h"
#include "trianglepatientwindow.h"
#include "setmaxforcedialog.h"

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

    m_forcePercent = params["force"].toInt();
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
    if(!isRecording())
    {
        if (!m_mfd)
        {
            m_mfd = new SetMaxForceDialog(SetMaxForceDialog::Triangle);
            m_mfd->setMinValueOffset(m_minDeviation);
            connect(m_mfd, &SetMaxForceDialog::accepted, this, &TriangleExecute::setMaxForceDialogAccepted);
        }
        m_mfd->showFullScreen();
    }
    else
    {
//        m_stage = StepOffsetDefines::stgWaiting;
        StabDynamicTestExecute::recording();
    }

}

void TriangleExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);
    if (selectedChannel() == data->channelId())
    {
        if (m_mfd)
            m_mfd->getData(x(), y());
    }
}

void TriangleExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}

void TriangleExecute::setMaxForceDialogAccepted()
{
    if (m_mfd)
    {
//        m_force = m_mfd->value() * m_forcePercent / 100;
        StabDynamicTestExecute::recording();

        if (isRecording())
        {
//            m_stage = StepOffsetDefines::stgWaiting;
//            m_res->setFreq(freqStab());
//            m_res->setDiap(diap());
//            m_res->setForce(m_forcePercent);
//            m_res->setStageTime(m_stageTime);
//            m_res->setDirection(m_direction);

//            if (m_patientWin)
//                m_patientWin->setDiapSpecific(static_cast<int>(m_forcePercent * 1.2));
        }
        else
        {
//            m_stage = StepOffsetDefines::stgWaiting;
//            m_tx = 0;
//            m_ty = 0;
//            setTarget(m_tx, m_ty);
        }
    }
}
