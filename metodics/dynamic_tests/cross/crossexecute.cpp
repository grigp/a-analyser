#include "crossexecute.h"
#include "ui_crossexecute.h"

#include "crosspatientwindow.h"
#include "crossresultdata.h"
#include "testresultdata.h"

#include <QDebug>

CrossExecute::CrossExecute(QWidget *parent) :
    StabDynamicTestExecute(parent),
    ui(new Ui::CrossExecute)
  , m_res (new CrossResultData(ChannelsDefines::chanCrossResult))
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

void CrossExecute::finishTest()
{
    trd()->addChannel(m_res);

    StabDynamicTestExecute::finishTest();
}

void CrossExecute::start()
{
    StabDynamicTestExecute::start();
}

void CrossExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);

    if (selectedChannel() == data->channelId())
    {
        if (isRecording())
        {

            //! На этапе продвижения цели
            if (m_stage == CrossDefines::stgGo)
            {
                if (newCoordinatesTarget(m_tx, m_ty))
                {
                    setTarget(m_tx, m_ty);
                }
                else
                {
                    m_tx = 0;
                    m_ty = 0;
                    setTarget(m_tx, m_ty);
                    m_stage = CrossDefines::stgWait;
                    m_waitCounter = 0;
                    m_res->addEvent(0, m_curDirection, recCount());
                }
            }
            else
            //! На этапе ожидания
            if (m_stage == CrossDefines::stgWait)
            {
                //! Ожидание закончено
                if (waitingSuccessful())
                {
                    if (newDirection())  //! Новое направление
                    {
                        m_res->addEvent(1, m_curDirection, recCount());
                        m_stage = CrossDefines::stgGo;  //! Успешно - двигаем цель
                    }
                    else
                        finishTest();                   //! Нет - надо выходить, тест закончен
                }
            }
        }
    }
}

void CrossExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}

bool CrossExecute::newDirection()
{
    if (m_curDirection != CrossDefines::dirNone)
        m_stagesProcess[m_curDirection]++;

    if (isTestFinished())
    {
        m_curDirection = CrossDefines::dirNone;
        return false;
    }

    if (m_directionMode == CrossDefines::dmClockwise)  //! По часовой
    {
        if (m_curDirection < CrossDefines::dirLeft)
        {
            int d = static_cast<int>(m_curDirection);
            d++;
            m_curDirection = static_cast<CrossDefines::Directions>(d);
        }
        else
            m_curDirection = CrossDefines::dirUp;
    }
    else
    if (m_directionMode == CrossDefines::dmCounterClockwise)  //! Против часовой
    {
        if (m_curDirection > CrossDefines::dirUp)
        {
            int d = static_cast<int>(m_curDirection);
            d--;
            m_curDirection = static_cast<CrossDefines::Directions>(d);
        }
        else
            m_curDirection = CrossDefines::dirLeft;
    }
    else
    if (m_directionMode == CrossDefines::dmRandom)
    {
        int d = -1;
        do
        {
            d = qrand() % 4;
        }
        while (m_stagesProcess[d] >= m_repeatCount);
        m_curDirection = static_cast<CrossDefines::Directions>(d);
    }
    return true;
}

bool CrossExecute::isTestFinished()
{
    for (int i = 0; i < 4; ++i)
        if (m_stagesProcess[i] < m_repeatCount)
            return false;
    return true;
}

bool CrossExecute::newCoordinatesTarget(double &tx, double &ty)
{
    double stepCount = m_stageTime * freqStab();
    switch (m_curDirection)
    {
    case CrossDefines::dirUp:
    {
        ty = ty + (diap() / (stepCount));
        return ty < diap();
    };
    case CrossDefines::dirRight:
    {
        tx = tx + (diap() / (stepCount));
        return tx < diap();
    };
    case CrossDefines::dirDown:
    {
        ty = ty - (diap() / (stepCount));
        return ty > -diap();
    };
    case CrossDefines::dirLeft:
    {
        tx = tx - (diap() / (stepCount));
        return tx > -diap();
    };
    default:
        return false;
    };
}

bool CrossExecute::waitingSuccessful()
{
    //! Ожидаем возврата в центр m_centerSize мм
    if (m_changeStateMode == CrossDefines::csmReturn)
    {
        if (fabs(x()) < m_centerSize && fabs(y()) < m_centerSize )
            return true;
    }
    else
    //! Ожидаем заданное время m_delayTime сек
    if (m_changeStateMode == CrossDefines::csmFixedTime)
    {
        ++m_waitCounter;
        if (m_waitCounter >= m_delayTime * freqStab())
            return true;
    }
    return false;
}

void CrossExecute::recording()
{
    StabDynamicTestExecute::recording();

    if (isRecording())
    {
        for (int i = 0; i < 4; ++i)
            m_stagesProcess[i] = 0;
        m_stage = CrossDefines::stgGo;
        newDirection();
        m_res->setFreq(freqStab());
        m_res->setStageTime(m_stageTime);
        m_res->addEvent(1, m_curDirection, recCount());
    }
    else
    {
        m_stage = CrossDefines::stgNo;
        m_tx = 0;
        m_ty = 0;
        setTarget(m_tx, m_ty);
    }
}

