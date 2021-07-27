#include "octaedronexecute.h"
#include "ui_octaedronexecute.h"

#include "octaedronpatientwindow.h"

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
}

void OctaedronExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);
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
