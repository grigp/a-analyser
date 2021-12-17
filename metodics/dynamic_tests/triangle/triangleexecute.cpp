#include "triangleexecute.h"

#include <QTimer>
#include <QDebug>

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
    m_trainingTime = params["training_time"].toInt();
    m_analysisTime = params["analysis_time"].toInt();

    auto dm = params["direction_mode"].toString();
    m_directionMode = BaseUtils::DirectionModeValueIndex.value(dm);

    m_forcePercent = params["force"].toInt();
    m_stageTime = params["stage_time"].toInt();
    m_minDeviation = params["min_deviation"].toInt();
    m_showMarkerAnal = params["show_marker_anal"].toBool();
}

StabDynamicTestPatientWindow *TriangleExecute::createPatientWindow()
{
    m_patientWin = new TrianglePatientWindow(this);
    return m_patientWin;
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
        m_mfd->resetValue();
        m_mfd->showFullScreen();
    }
    else
    {
        m_stage = TriangleDefines::stgWaiting;
        clearTargets();
        m_targets.clear();
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

        ++m_stageCounter;
        if (m_stageCounter == m_stageTime * freqStab())
        {
            nextCorner();
            showCurrentCorner();

            m_stageCounter = 0;
        }

        if ((recCount() >= m_trainingTime * freqStab()) && (m_stage == TriangleDefines::stgTraining))
        {
            if (m_stageCounter == BaseUtils::tcUp)
            {
                m_stage = TriangleDefines::stgAnalysis;
                m_startAnalysis = recCount();
                clearTargets();
            }
        }
        else
        if ((recCount() >= (m_trainingTime + m_analysisTime) * freqStab()) && (m_stage == TriangleDefines::stgAnalysis))
        {
            if (m_stageCounter == BaseUtils::tcUp)
            {
                m_targets.clear();
                finishTest();
            }
        }
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
            clearTargets();
            m_targets.clear();

            int vu = m_mfd->value(BaseUtils::tcUp) * m_forcePercent / 100;
            int vld = m_mfd->value(BaseUtils::tcLeftDown) * m_forcePercent / 100;
            int vrd = m_mfd->value(BaseUtils::tcRightDown) * m_forcePercent / 100;
            m_targets << Target(0, vu, BaseUtils::tcUp)
                      << Target(-vld * cos(M_PI/6), -vld * sin(M_PI/6), BaseUtils::tcLeftDown)
                      << Target(vrd * cos(M_PI/6), -vrd * sin(M_PI/6), BaseUtils::tcRightDown);

            foreach (auto corner, m_targets)
            {
                addTarget(corner.x, corner.y, Qt::gray, Qt::darkGray);
                if (m_patientWin)
                    m_patientWin->addTarget(corner.x, corner.y, Qt::gray, 30);
            }

            if (m_directionMode == BaseUtils::dmClockwise)
                m_curCorner = BaseUtils::tcLeftDown;
            else
            if (m_directionMode == BaseUtils::dmCounterClockwise)
                m_curCorner = BaseUtils::tcRightDown;
            if (m_curCorner < m_targets.size())
            {
                addTarget(m_targets.at(m_curCorner).x, m_targets.at(m_curCorner).y, Qt::green, Qt::darkGreen);
                if (m_patientWin)
                    m_patientWin->addTarget(m_targets.at(m_curCorner).x, m_targets.at(m_curCorner).y, Qt::green, 30);
            }

            m_stageCounter = 0;
            m_stage = TriangleDefines::stgTraining;

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
//            m_tx = 0;
//            m_ty = 0;
//            setTarget(m_tx, m_ty);
        }
    }
}

void TriangleExecute::showCurrentCorner()
{
    if (m_curCorner < m_targets.size())
    {
        setTarget(m_targets.at(m_curCorner).x, m_targets.at(m_curCorner).y, 3);
        if (m_patientWin)
            m_patientWin->setTarget(m_targets.at(m_curCorner).x, m_targets.at(m_curCorner).y, 3);
    }
}

void TriangleExecute::nextCorner()
{
    auto cur = static_cast<int>(m_curCorner);
    if (m_directionMode == BaseUtils::dmClockwise)
    {
        if (cur > 0)
            --cur;
        else
            cur = BaseUtils::tcRightDown;
    }
    else
    if (m_directionMode == BaseUtils::dmCounterClockwise)
    {
        if (cur < 2)
            ++cur;
        else
            cur = BaseUtils::tcUp;
    }
    m_curCorner = static_cast<BaseUtils::TriangleCorner>(cur);
}
