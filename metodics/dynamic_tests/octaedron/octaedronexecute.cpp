#include "octaedronexecute.h"
#include "ui_octaedronexecute.h"

#include "aanalyserapplication.h"
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

    auto kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
    setTitle(tr("Тренажер \"Октаедр\"") + " - " + kard.fio);
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
    m_circeRoundRuleMode = BaseDefines::CirceRoundRuleModeValueIndex.value(crm);

    auto dm = params["direction_mode"].toString();
    m_directionMode = BaseDefines::DirectionModeValueIndex.value(dm);

    m_stageTime = params["stage_time"].toInt();
    m_radius = params["radius"].toInt();

    auto sfm = params["stage_fixing_mode"].toString();
    m_stageFinishMode = BaseDefines::StageFinishModeValueIndex.value(sfm);
    m_holdingTime = params["holding_time"].toInt(1);
    m_holdingAmplitude = params["holding_amplitude"].toInt(10);
    m_isShowOnlyCurrentPoint = params["only_current_point"].toBool(false);

    setTargetsCoordinates();

    StabDynamicTestExecute::setParams(params);
}

StabDynamicTestPatientWindow *OctaedronExecute::createPatientWindow()
{
    m_patientWin = new OctaedronPatientWindow(static_cast<int>(m_radius * 1.2), m_targets, this);
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
    m_res->clear();

    if (isRecording())
    {
        getFirstStage();

        m_res->setFreq(freqStab());
        m_res->setDiap(diap());
        m_res->setRadius(m_radius);
        m_res->setStageTime(m_stageTime);
        m_res->setCirceRoundRuleMode(BaseDefines::CirceRoundRuleModeValueName.value(m_circeRoundRuleMode));
        m_res->setDirectionMode(BaseDefines::DirectionModeValueName.value(m_directionMode));
        addStageToResult();
    }

    setCurrentTarget();
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
            if (isStageEnded())
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
    m_targets << BaseDefines::TargetInfo(std::make_tuple(0, m_radius, !m_isShowOnlyCurrentPoint))
              << BaseDefines::TargetInfo(std::make_tuple(-m_radius * cos(M_PI_4), m_radius * sin(M_PI_4), !m_isShowOnlyCurrentPoint))
              << BaseDefines::TargetInfo(std::make_tuple(-m_radius, 0, !m_isShowOnlyCurrentPoint))
              << BaseDefines::TargetInfo(std::make_tuple(-m_radius * 0.8 * cos(M_PI_4), -m_radius * 0.8 * sin(M_PI_4), !m_isShowOnlyCurrentPoint))
              << BaseDefines::TargetInfo(std::make_tuple(0, -m_radius * 0.7, !m_isShowOnlyCurrentPoint))
              << BaseDefines::TargetInfo(std::make_tuple(m_radius * 0.8 * cos(M_PI_4), -m_radius * 0.8 * sin(M_PI_4), !m_isShowOnlyCurrentPoint))
              << BaseDefines::TargetInfo(std::make_tuple(m_radius, 0, !m_isShowOnlyCurrentPoint))
              << BaseDefines::TargetInfo(std::make_tuple(m_radius * cos(M_PI_4), m_radius * sin(M_PI_4), !m_isShowOnlyCurrentPoint));
}

void OctaedronExecute::createMarkerAndTargets()
{
    foreach (auto target, m_targets)
    {
        addTarget(target.x, target.y, Qt::gray, Qt::darkGray, target.visible);
//        if (m_patientWin)
//            m_patientWin->addTarget(target.x, target.y, Qt::gray, 30);
    }
    addTarget(0, 0, Qt::green, Qt::darkGreen);
//    if (m_patientWin)
//        m_patientWin->addTarget(0, 0, Qt::green, 30);
    addMarker();
}

void OctaedronExecute::getFirstStage()
{
    m_stage = 0;
    if (m_circeRoundRuleMode == BaseDefines::crmRadial)
    {
        if (m_directionMode == BaseDefines::dmCounterClockwise)
            m_stage = 0;
        else
        if (m_directionMode == BaseDefines::dmClockwise)
            m_stage = SequenceRadial.size() - 1;
        else
        if (m_directionMode == BaseDefines::dmRandom)
            m_stage = 0;
    }
    else
    if (m_circeRoundRuleMode == BaseDefines::crmCircle)
    {
        if (m_directionMode == BaseDefines::dmCounterClockwise)
            m_stage = 0;
        else
        if (m_directionMode == BaseDefines::dmClockwise)
            m_stage = SequenceCircle.size() - 1;
    }
}

bool OctaedronExecute::getNextStage()
{
    auto nextStage = [&]()
    {
        if (m_directionMode == BaseDefines::dmCounterClockwise)
            ++m_stage;
        else
        if (m_directionMode == BaseDefines::dmClockwise)
            --m_stage;
    };
    ///--------------

    if (m_circeRoundRuleMode == BaseDefines::crmRadial)
    {
        nextStage();
        if (m_directionMode == BaseDefines::dmRandom)
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
    if (m_circeRoundRuleMode == BaseDefines::crmCircle)
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
    if (m_circeRoundRuleMode == BaseDefines::crmRadial)
        return SequenceRadial.at(m_stage);
    else
    if (m_circeRoundRuleMode == BaseDefines::crmCircle)
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
    double tx = 0;
    double ty = 0;
    getCurrentTargetCoords(trgtPos, tx, ty);

    m_res->addStage(trgtPos, recCount(), tx, ty);
}

void OctaedronExecute::getCurrentTargetCoords(int& trgtPos, double &x, double &y)
{
    trgtPos = -1;
    if (m_circeRoundRuleMode == BaseDefines::crmRadial)
        trgtPos = SequenceRadial.at(m_stage);
    else
    if (m_circeRoundRuleMode == BaseDefines::crmCircle)
        trgtPos = SequenceCircle.at(m_stage);
    x = 0;
    y = 0;
    if (trgtPos > -1)
    {
        x = m_targets.at(trgtPos).x;
        y = m_targets.at(trgtPos).y;
    }
}

double OctaedronExecute::distanceToTarget()
{
    int trgtPos = -1;
    double tx = 0;
    double ty = 0;
    getCurrentTargetCoords(trgtPos, tx, ty);

    return sqrt(pow(tx - x(), 2) + pow(ty - y(), 2));
}

bool OctaedronExecute::isStageEnded()
{
    int sc = -1;
    if (m_circeRoundRuleMode == BaseDefines::crmRadial)
        sc = SequenceRadial.at(m_stage);
    else
    if (m_circeRoundRuleMode == BaseDefines::crmCircle)
        sc = SequenceCircle.at(m_stage);

    //! При возврате в исходное состояние - только ждем истечения времени
    if (sc == -1)
        return m_stageCounter >= m_stageTime * freqStab();
    else
    //! Возврат по истечению времени
    if (m_stageFinishMode == BaseDefines::sfmFixedTime)
        return m_stageCounter >= m_stageTime * freqStab();
    else
    //! Возврат по времени удержания
    if (m_stageFinishMode == BaseDefines::sfmFixingOnTarget)
    {
        auto d = distanceToTarget();
        if (d <= m_holdingAmplitude)
        {
            ++m_holdingCouter;
            if (m_holdingCouter >= m_holdingTime * freqStab())
                return true;
        }
        else
            m_holdingCouter = 0;
        //! Если не удается, то возврат по истечению времени
        return m_stageCounter >= m_stageTime * freqStab();

    }
    return false;
}
