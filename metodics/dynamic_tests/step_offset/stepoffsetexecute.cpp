#include "stepoffsetexecute.h"
#include "ui_stepoffsetexecute.h"

#include <QLayout>
#include <QDebug>

#include "aanalyserapplication.h"
#include "stepoffsettpatientwindow.h"
#include "setmaxforcedialog.h"
#include "stepoffsetresultdata.h"
#include "testresultdata.h"

StepOffsetExecute::StepOffsetExecute(QWidget *parent) :
    StabDynamicTestExecute(parent),
    ui(new Ui::StepOffsetExecute)
  , m_res(new StepOffsetResultData(ChannelsDefines::chanStepOffsetResult))
{
    ui->setupUi(this);

    addTarget(0, 0, Qt::green, Qt::darkGreen);
    addMarker();

    auto kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
    setTitle(tr("Тест \"Ступени\"") + " - " + kard.fio);
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
    m_direction = BaseDefines::DirectionValueIndex.value(d);
}

StabDynamicTestPatientWindow *StepOffsetExecute::createPatientWindow()
{
    m_patientWin = new StepOffsettPatientWindow(this);
    return m_patientWin;
}

void StepOffsetExecute::finishTest()
{
    trd()->addChannel(m_res);
    StabDynamicTestExecute::finishTest();
}

void StepOffsetExecute::fillSpecific(QFrame *frSpecific)
{
    StabDynamicTestExecute::fillSpecific(frSpecific);

    m_lblRemain = new QLabel(frSpecific);
    m_lblRemain->setText(tr("Осталось проходов") + " - " + QString::number(m_repeatCount - m_repeatCounter));
    frSpecific->layout()->addWidget(m_lblRemain);
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
            m_mfd = new SetMaxForceDialog(SetMaxForceDialog::CrossOneLine);
            m_mfd->setDirection(m_direction);
            m_mfd->setMinValueOffset(StepOffsetDefines::MinValueOffset);
            connect(m_mfd, &SetMaxForceDialog::accepted, this, &StepOffsetExecute::setMaxForceDialogAccepted);
        }
        m_mfd->showFullScreen();
    }
    else
    {
        m_stage = StepOffsetDefines::stgWaiting;
        StabDynamicTestExecute::recording();
    }

    m_repeatCounter = 0;
    m_lblRemain->setText(tr("Осталось проходов") + " - " + QString::number(m_repeatCount - m_repeatCounter));
    m_stageCounter = 0;
    m_recordCounter = 0;
    m_tx = 0;
    m_ty = 0;
    setTarget(m_tx, m_ty);
    m_res->clear();
}

void StepOffsetExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);
    if (selectedChannel() == data->channelId())
    {
        if (m_mfd)
            m_mfd->getData(x(), y());

        if (isRecording())
        {
            //! Счетчик этапа
            ++m_stageCounter;
            ++m_recordCounter;

            //! Достигли конца этапа
            if (m_stageCounter >= m_stageTime * freqStab())
            {
                //! После этапа возврата добавляем и инкрементируем счетчик повторений
                if (m_stage == StepOffsetDefines::stgReturn)
                    ++m_repeatCounter;

                if (m_repeatCounter < m_repeatCount)
                {
                    //! Переход к этапу компенсации
                    if (m_stage == StepOffsetDefines::stgWaiting ||
                        m_stage == StepOffsetDefines::stgReturn)
                    {
                        m_stage = StepOffsetDefines::stgCompensaton;
                        setTargetCoordinates();
                        m_lastCompensation = m_recordCounter;
                        m_lblRemain->setText(tr("Осталось проходов") + " - " + QString::number(m_repeatCount - m_repeatCounter));
                    }
                    else
                    //! Переход к этапу возврата
                    {
                        m_stage = StepOffsetDefines::stgReturn;
                        m_tx = 0;
                        m_ty = 0;
                        m_res->addStep(m_lastCompensation, m_recordCounter);
                    }
                    setTarget(m_tx, m_ty);
                }
                else
                //! Прошло заданное число повторений
                {
                    finishTest();
                }

                m_stageCounter = 0;
            }
        }
    }
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

        if (isRecording())
        {
            m_stage = StepOffsetDefines::stgWaiting;
            m_res->setFreq(freqStab());
            m_res->setDiap(diap());
            m_res->setForce(m_force);
            m_res->setStageTime(m_stageTime);
            m_res->setDirection(m_direction);

            if (m_patientWin)
                m_patientWin->setDiapSpecific(static_cast<int>(m_force * 1.2));
        }
        else
        {
            m_stage = StepOffsetDefines::stgWaiting;
            m_tx = 0;
            m_ty = 0;
            setTarget(m_tx, m_ty);
        }
    }
}

void StepOffsetExecute::setTargetCoordinates()
{
    if (m_direction == BaseDefines::dirUp)
    {
        m_tx = 0;
        m_ty = m_force;
    }
    else
    if (m_direction == BaseDefines::dirRight)
    {
        m_tx = m_force;
        m_ty = 0;
    }
    else
    if (m_direction == BaseDefines::dirDown)
    {
        m_tx = 0;
        m_ty = -m_force;
    }
    else
    if (m_direction == BaseDefines::dirLeft)
    {
        m_tx = -m_force;
        m_ty = 0;
    }
}
