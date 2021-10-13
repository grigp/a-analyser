#include "crossexecute.h"
#include "ui_crossexecute.h"

#include "aanalyserapplication.h"
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

    auto kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
    setTitle(tr("Тест на устойчивость") + " - " + kard.fio);
    isShowValues(false);
    isTraceControl(false);
    setVisibleRecordLength(false);

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
    m_directionMode = BaseUtils::DirectionModeValueIndex.value(dm);

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

void CrossExecute::fillSpecific(QFrame *frSpecific)
{
    StabDynamicTestExecute::fillSpecific(frSpecific);

    auto lblTitle = new QLabel(frSpecific);
    lblTitle->setText(tr("Осталось проходов"));
    frSpecific->layout()->addWidget(lblTitle);
    m_lblUp = new QLabel(frSpecific);
    m_lblUp->setText(tr("Вперед") + " - " + stagesRemained(BaseUtils::dirUp));
    frSpecific->layout()->addWidget(m_lblUp);
    m_lblDn = new QLabel(frSpecific);
    m_lblDn->setText(tr("Назад") +" - " + stagesRemained(BaseUtils::dirDown));
    frSpecific->layout()->addWidget(m_lblDn);
    m_lblRt = new QLabel(frSpecific);
    m_lblRt->setText(tr("Вправо") + " - " + stagesRemained(BaseUtils::dirRight));
    frSpecific->layout()->addWidget(m_lblRt);
    m_lblLf = new QLabel(frSpecific);
    m_lblLf->setText(tr("Влево") + " - " + stagesRemained(BaseUtils::dirLeft));
    frSpecific->layout()->addWidget(m_lblLf);
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
    if (m_curDirection != BaseUtils::dirNone)
        m_stagesProcess[m_curDirection]++;
    if (m_curDirection == BaseUtils::dirUp)
        m_lblUp->setText(tr("Вперед") + " - " + stagesRemained(m_curDirection));
    else
    if (m_curDirection == BaseUtils::dirDown)
        m_lblDn->setText(tr("Назад") + " - " + stagesRemained(m_curDirection));
    else
    if (m_curDirection == BaseUtils::dirRight)
        m_lblRt->setText(tr("Вправо") + " - " + stagesRemained(m_curDirection));
    else
    if (m_curDirection == BaseUtils::dirLeft)
        m_lblLf->setText(tr("Влево") + " - " + stagesRemained(m_curDirection));

    if (isTestFinished())
    {
        m_curDirection = BaseUtils::dirNone;
        return false;
    }

    if (m_directionMode == BaseUtils::dmClockwise)  //! По часовой
    {
        if (m_curDirection < BaseUtils::dirLeft)
        {
            int d = static_cast<int>(m_curDirection);
            d++;
            m_curDirection = static_cast<BaseUtils::Directions>(d);
        }
        else
            m_curDirection = BaseUtils::dirUp;
    }
    else
    if (m_directionMode == BaseUtils::dmCounterClockwise)  //! Против часовой
    {
        if (m_curDirection > BaseUtils::dirUp)
        {
            int d = static_cast<int>(m_curDirection);
            d--;
            m_curDirection = static_cast<BaseUtils::Directions>(d);
        }
        else
            m_curDirection = BaseUtils::dirLeft;
    }
    else
    if (m_directionMode == BaseUtils::dmRandom)
    {
        int d = -1;
        do
        {
            d = qrand() % 4;
        }
        while (m_stagesProcess[d] >= m_repeatCount);
        m_curDirection = static_cast<BaseUtils::Directions>(d);
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
    case BaseUtils::dirUp:
    {
        ty = ty + (diap() / (stepCount));
        return ty < diap();
    };
    case BaseUtils::dirRight:
    {
        tx = tx + (diap() / (stepCount));
        return tx < diap();
    };
    case BaseUtils::dirDown:
    {
        ty = ty - (diap() / (stepCount));
        return ty > -diap();
    };
    case BaseUtils::dirLeft:
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

QString CrossExecute::stagesRemained(const BaseUtils::Directions dir) const
{
    int n = m_repeatCount - m_stagesProcess[dir];
    return QString::number(n);
}

void CrossExecute::recording()
{
    StabDynamicTestExecute::recording();

    m_res->clear();

    for (int i = 0; i < 4; ++i)
        m_stagesProcess[i] = 0;
    m_lblUp->setText(tr("Вперед") + " - " + stagesRemained(BaseUtils::dirUp));
    m_lblDn->setText(tr("Назад") + " - " + stagesRemained(BaseUtils::dirDown));
    m_lblRt->setText(tr("Вправо") + " - " + stagesRemained(BaseUtils::dirRight));
    m_lblLf->setText(tr("Влево") + " - " + stagesRemained(BaseUtils::dirLeft));

    if (isRecording())
    {
        m_stage = CrossDefines::stgGo;
        newDirection();
        m_res->setFreq(freqStab());
        m_res->setDiap(diap());
        m_res->setStageTime(m_stageTime);
        m_res->addEvent(1, m_curDirection, recCount());
    }
    else
    {
        m_stage = CrossDefines::stgNo;
        m_curDirection = BaseUtils::dirNone;
        m_tx = 0;
        m_ty = 0;
        setTarget(m_tx, m_ty);
    }
}

