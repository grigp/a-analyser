#include "octaedronexecute.h"
#include "ui_octaedronexecute.h"

#include "channelsdefines.h"
#include "testresultdata.h"
#include "octaedronpatientwindow.h"
#include "octaedronresultdata.h"

#include <QTimer>
#include <QDebug>

namespace
{
static const QList<int> SequenceRadial = {-1, 0, -1, 1, -1, 2, -1, 3, -1, 4, -1, 5, -1, 6, -1, 7, -1};
static const QList<int> SequenceCircle = {0, 1, 2, 3, 4, 5, 6, 7, 0};
}

OctaedronExecute::OctaedronExecute(QWidget *parent) :
    StabDynamicTestExecute(parent),
    ui(new Ui::OctaedronExecute)
  , m_res(new OctaedronResultData(ChannelsDefines::chanOctaedronResult))
{
    ui->setupUi(this);

    setTitle(tr("Тренажер \"Остаедр\""));
    isShowValues(false);
    isTraceControl(false);
    setVisibleRecordLength(false);

    QTimer::singleShot(10, [=]
    {
        createMarkerAndTargets();
    });
}

OctaedronExecute::~OctaedronExecute()
{
    delete ui;
}

void OctaedronExecute::setParams(const QJsonObject &params)
{
    auto crm = params["circe_round_rule_mode"].toString();
    m_circeRoundRuleMode = BaseUtils::CirceRoundRuleModeValueIndex.value(crm);

    auto dm = params["direction_mode"].toString();
    m_directionMode = BaseUtils::DirectionModeValueIndex.value(dm);

    m_stageTime = params["stage_time"].toInt();
    m_radius = params["radius"].toInt();

    setTargetsCoordinates();

    StabDynamicTestExecute::setParams(params);
}

StabDynamicTestPatientWindow *OctaedronExecute::createPatientWindow()
{
    m_patientWin = new OctaedronPatientWindow(static_cast<int>(m_radius * 1.2), this);
    return m_patientWin;
}

void OctaedronExecute::finishTest()
{
    trd()->addChannel(m_res);
    StabDynamicTestExecute::finishTest();
}

void OctaedronExecute::fillSpecific(QFrame *frSpecific)
{
    StabDynamicTestExecute::fillSpecific(frSpecific);
}

void OctaedronExecute::start()
{
    StabDynamicTestExecute::start();
}

void OctaedronExecute::recording()
{
    StabDynamicTestExecute::recording();

    m_stage = 0;
    m_stageCounter = 0;
    m_passed.clear();

    if (isRecording())
    {
        getFirstStage();
        setCurrentTarget();

        m_res->setFreq(freqStab());
        m_res->setDiap(diap());
        m_res->setRadius(m_radius);
        m_res->setStageTime(m_stageTime);
        m_res->setCirceRoundRuleMode(BaseUtils::CirceRoundRuleModeValueName.value(m_circeRoundRuleMode));
        addStageToResult();
    }
}

void OctaedronExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);

    if (selectedChannel() == data->channelId())
    {
        if (isRecording())
        {
            ++m_stageCounter;
            //! Достигли конца этапа
            if (m_stageCounter >= m_stageTime * freqStab())
            {
                if (getNextStage())
                {
                    m_stageCounter = 0;
                    setCurrentTarget();
                    addStageToResult();
                }
                else
                    finishTest();
            }
        }
    }
}

void OctaedronExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}

void OctaedronExecute::setTargetsCoordinates()
{
    m_targets.clear();
    m_targets << SignalsDefines::StabRec(std::make_tuple(0, m_radius))
              << SignalsDefines::StabRec(std::make_tuple(-m_radius * cos(M_PI_4), m_radius * sin(M_PI_4)))
              << SignalsDefines::StabRec(std::make_tuple(-m_radius, 0))
              << SignalsDefines::StabRec(std::make_tuple(-m_radius * 0.8 * cos(M_PI_4), -m_radius * 0.8 * sin(M_PI_4)))
              << SignalsDefines::StabRec(std::make_tuple(0, -m_radius * 0.7))
              << SignalsDefines::StabRec(std::make_tuple(m_radius * 0.8 * cos(M_PI_4), -m_radius * 0.8 * sin(M_PI_4)))
              << SignalsDefines::StabRec(std::make_tuple(m_radius, 0))
              << SignalsDefines::StabRec(std::make_tuple(m_radius * cos(M_PI_4), m_radius * sin(M_PI_4)));
}

void OctaedronExecute::createMarkerAndTargets()
{
    foreach (auto target, m_targets)
    {
        addTarget(target.x, target.y, Qt::gray, Qt::darkGray);
        if (m_patientWin)
            m_patientWin->addTarget(target.x, target.y, Qt::gray, 30);
    }
    addTarget(0, 0, Qt::green, Qt::darkGreen);
    if (m_patientWin)
        m_patientWin->addTarget(0, 0, Qt::green, 30);
    addMarker();
}

void OctaedronExecute::getFirstStage()
{
    m_stage = 0;
    if (m_circeRoundRuleMode == BaseUtils::crmRadial)
    {
        if (m_directionMode == BaseUtils::dmCounterClockwise)
            m_stage = 0;
        else
        if (m_directionMode == BaseUtils::dmClockwise)
            m_stage = SequenceRadial.size() - 1;
        else
        if (m_directionMode == BaseUtils::dmRandom)
            m_stage = 0;
    }
    else
    if (m_circeRoundRuleMode == BaseUtils::crmCircle)
    {
        if (m_directionMode == BaseUtils::dmCounterClockwise)
            m_stage = 0;
        else
        if (m_directionMode == BaseUtils::dmClockwise)
            m_stage = SequenceCircle.size() - 1;
    }
}

bool OctaedronExecute::getNextStage()
{
    auto nextStage = [&]()
    {
        if (m_directionMode == BaseUtils::dmCounterClockwise)
            ++m_stage;
        else
        if (m_directionMode == BaseUtils::dmClockwise)
            --m_stage;
    };
    ///--------------

    if (m_circeRoundRuleMode == BaseUtils::crmRadial)
    {
        nextStage();
        if (m_directionMode == BaseUtils::dmRandom)
        {
            if (m_stage == 0)
                m_stage = getRandomStage();
            else
                m_stage = 0;
            if (m_stage == -2)
                return false;
        }
        else
        {
            if (m_stage < 0 || m_stage >= SequenceRadial.size())
                return  false;
        }
    }
    else
    if (m_circeRoundRuleMode == BaseUtils::crmCircle)
    {
        nextStage();
        if (m_stage < 0 || m_stage >= SequenceCircle.size())
            return  false;
    }

    return true;
}

int OctaedronExecute::getRandomStage()
{
    //! Поиск в списке пройденных этапов хотя бы одного непройденного
    int idx = -1;
    for (int i = 0; i < 8; ++i)
        if (!m_passed.contains(i))
        {
            idx = i;
            break;
        }
    //! Все уже пройдены
    if (idx == -1)
        return -2;

    //! Генерация нового
    do
    {
        idx = qrand() % SequenceRadial.size();
    }
    while(SequenceRadial.at(idx) == -1 || m_passed.contains(SequenceRadial.at(idx)));
    m_passed.insert(SequenceRadial.at(idx));

    return idx;
}

int OctaedronExecute::getCurrentTargetPosition()
{
    if (m_circeRoundRuleMode == BaseUtils::crmRadial)
        return SequenceRadial.at(m_stage);
    else
    if (m_circeRoundRuleMode == BaseUtils::crmCircle)
        return SequenceCircle.at(m_stage);
    return -1;
}

void OctaedronExecute::setCurrentTarget()
{
   auto tp = getCurrentTargetPosition();
   if (tp > -1)
       setTarget(m_targets.at(tp).x, m_targets.at(tp).y, 8);
   else
       setTarget(0, 0, 8);
}

void OctaedronExecute::addStageToResult()
{
    int trgtPos = -1;
    if (m_circeRoundRuleMode == BaseUtils::crmRadial)
        trgtPos = SequenceRadial.at(m_stage);
    else
    if (m_circeRoundRuleMode == BaseUtils::crmCircle)
        trgtPos = SequenceCircle.at(m_stage);

    if (trgtPos > -1)
        m_res->addStage(trgtPos, recCount(), m_targets.at(trgtPos).x, m_targets.at(trgtPos).y);
    else
        m_res->addStage(trgtPos, recCount(), 0, 0);
}
