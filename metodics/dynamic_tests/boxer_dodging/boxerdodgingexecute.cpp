#include "boxerdodgingexecute.h"

#include "channelsdefines.h"
#include "boxerdodgingpatientwindow.h"
#include "aanalyserapplication.h"
#include "boxerdodgingresultdata.h"
#include "testresultdata.h"

#include <QDesktopWidget>
#include <QTimer>
#include <QDebug>

BoxerDodgingExecute::BoxerDodgingExecute(QWidget *parent)
    : StabDynamicTestExecute (parent)
    , m_res(new BoxerDodgingResultData(ChannelsDefines::chanBoxerDodgingResult))
{
    auto kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
    setTitle(tr("Тренажер с уклонением для боксеров") + " - " + kard.fio);
    isShowValues(false);
    isTraceControl(false);

    QTimer::singleShot(10, [=]
    {
        createMarker();
    });
}

BoxerDodgingExecute::~BoxerDodgingExecute()
{

}

void BoxerDodgingExecute::setParams(const QJsonObject &params)
{
    StabDynamicTestExecute::setParams(params);

    m_deviationThreshold = params["deviation_threshold"].toInt();
    m_time = params["time"].toInt();
    m_stimulTimeMin = params["min_len"].toInt();
    m_stimulTimeMax = params["max_len"].toInt();
    setRecordLength(m_time * freqStab());
}

StabDynamicTestPatientWindow *BoxerDodgingExecute::createPatientWindow()
{
//    m_patientWin = new OctaedronPatientWindow(static_cast<int>(m_radius * 1.2), m_targets, this);
//    return m_patientWin;

    return nullptr;
}

void BoxerDodgingExecute::finishTest()
{
    trd()->addChannel(m_res);
    StabDynamicTestExecute::finishTest();
}

void BoxerDodgingExecute::fillSpecific(QFrame *frSpecific)
{
    StabDynamicTestExecute::fillSpecific(frSpecific);
}

void BoxerDodgingExecute::start()
{
    StabDynamicTestExecute::start();

    if (QApplication::desktop()->screenCount() > 1)
        createAndShowPatientWindow();
}

void BoxerDodgingExecute::recording()
{
    StabDynamicTestExecute::recording();

    m_stageCounter = 0;
    nextStage(true);

    m_res->clear();

    if (isRecording())
    {
        if (QApplication::desktop()->screenCount() == 1)
            createAndShowPatientWindow();
        m_res->setFreq(freqStab());
        m_res->setDiap(diap());
        m_res->setDeviationThreshold(m_deviationThreshold);
    }
    else
    {
        if (QApplication::desktop()->screenCount() == 1)
            hidePatientWindow();
    }
}

void BoxerDodgingExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);

    if (selectedChannel() == data->channelId())
    {
        if (isRecording())
        {
            ++m_stageCounter;
            if (m_stageCounter >= m_nextStageCount)
            {
                nextStage(false);
                m_stageCounter = 0;
            }

            if (recCount() >= m_time * freqStab())
                finishTest();
        }
    }
}

void BoxerDodgingExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}

void BoxerDodgingExecute::createMarker()
{
    addMarker();
}

void BoxerDodgingExecute::createAndShowPatientWindow()
{
    m_patientWin = new BoxerDodgingPatientWindow(this);

    if (m_patientWin)
    {
        auto rect = static_cast<AAnalyserApplication*>(QApplication::instance())->getPatientWindowGeometry();
        m_patientWin->resize(rect.size());
        m_patientWin->move(rect.x(), rect.y());
        m_patientWin->show();
    }
}

void BoxerDodgingExecute::hidePatientWindow()
{
    if (m_patientWin)
    {
        BoxerDodgingPatientWindow *pw = m_patientWin;
        m_patientWin = nullptr;
        delete pw;
    }
}

void BoxerDodgingExecute::nextStage(const bool isStart)
{
    //! Длительность следующего этапа в пакетах
    m_nextStageCount = m_stimulTimeMin * freqStab() + qrand() % (freqStab() * (m_stimulTimeMax - m_stimulTimeMin));

    //! Выбор следующего этапа
    m_patientWin->setVisibleStage(m_stage, false);
    if (isStart)  //! Для старта - всегда базовый
        m_stage = BoxerDodgingDefines::bdsBase;
    else          //! Для послеующих по очереди
    {
        if (m_stage == BoxerDodgingDefines::bdsBase)  //! Случайный
            m_stage = static_cast<BoxerDodgingDefines::Stages>(qrand() % 4 + 1);
        else                                          //! Базовый
            m_stage = BoxerDodgingDefines::bdsBase;
    }
    m_patientWin->setVisibleStage(m_stage, true);

    m_res->addStage(m_stage, recCount());
}
