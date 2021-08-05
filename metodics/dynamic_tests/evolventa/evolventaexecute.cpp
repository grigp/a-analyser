#include "evolventaexecute.h"
#include "ui_evolventaexecute.h"

#include <QLabel>
#include <QLayout>
#include <QDebug>

#include "evolventapatientwindow.h"
#include "evolventaresultdata.h"
#include "testresultdata.h"
#include "decartcoordinatessignal.h"

EvolventaExecute::EvolventaExecute(QWidget *parent) :
    StabDynamicTestExecute(parent),
    ui(new Ui::EvolventaExecute)
  , m_res(new EvolventaResultData(ChannelsDefines::chanEvolventaResult))

{
    ui->setupUi(this);

    addTarget(0, 0, Qt::green, Qt::darkGreen);
    addMarker();

    setTitle(tr("Тест \"Эвольвента\""));
    isShowValues(false);
    isTraceControl(false);
    setVisibleRecordLength(false);

    m_targetSign = new DecartCoordinatesSignal(ChannelsDefines::chanTargetTraectory, freqStab());
}

EvolventaExecute::~EvolventaExecute()
{
//    delete m_targetSign;
    delete ui;
}

void EvolventaExecute::setParams(const QJsonObject &params)
{
    m_speedMax = params["max_speed"].toInt();
    m_radiusMax = params["max_radius"].toInt();
    m_circleCount = params["circles"].toInt();

    auto dm = params["direction_mode"].toString();
    m_directionMode = BaseUtils::DirectionModeValueIndex.value(dm);

    StabDynamicTestExecute::setParams(params);
}

StabDynamicTestPatientWindow *EvolventaExecute::createPatientWindow()
{
    return new EvolventaPatientWindow(static_cast<int>(m_radiusMax * 1.2), this);
}

void EvolventaExecute::finishTest()
{
    trd()->addChannel(m_res);
    trd()->addChannel(m_targetSign);
    StabDynamicTestExecute::finishTest();
}

void EvolventaExecute::fillSpecific(QFrame *frSpecific)
{
    StabDynamicTestExecute::fillSpecific(frSpecific);

    auto lblTitle = new QLabel(frSpecific);
    m_lblStage = new QLabel(frSpecific);
    m_lblStage->setText(tr("Этап") + " : " + tr("ожидание"));
    frSpecific->layout()->addWidget(m_lblStage);
}

void EvolventaExecute::start()
{
    StabDynamicTestExecute::start();
}

void EvolventaExecute::recording()
{
    StabDynamicTestExecute::recording();

    m_stage = EvolventaDefines::stgUpwinding;
    m_tx = 0;
    m_ty = 0;
    setTarget(m_tx, m_ty);

    m_targetAmpl = 0;
    m_targetAngle = 0;

    m_targetSign->clear();
    m_res->clear();

    if (isRecording())
    {
        m_res->setFreq(freqStab());
        m_res->setDiap(diap());
        m_res->setTimeUpwinding(recCount());
        m_lblStage->setText(tr("Этап") + " : " + EvolventaDefines::StageValueName.value(m_stage));
    }
    else
    {
        m_lblStage->setText(tr("Этап") + " : " + tr("ожидание"));
    }
}

void EvolventaExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);

    if (selectedChannel() == data->channelId())
    {
        if (isRecording())
        {
            //! Установка цели
            setTargetPolar(m_targetAngle, m_targetAmpl);

            //! Управление углом цели относительно 0
            double w = (2 * M_PI / static_cast<double>(freqStab())) * 0.1;
            if (w * static_cast<double>(freqStab()) * m_targetAmpl > m_speedMax)
                m_targetAngle = m_targetAngle + (static_cast<double>(m_speedMax) / (static_cast<double>(freqStab()) * m_targetAmpl));
            else
                m_targetAngle = m_targetAngle + w;

            //! Раскручивание
            if (m_stage == EvolventaDefines::stgUpwinding)
            {
                if (m_targetAmpl < m_radiusMax)
                    m_targetAmpl = m_targetAmpl + 0.04 / (static_cast<double>(freqStab()) / 50.0);
                else
                {
                    m_stage = EvolventaDefines::stgHold;
                    m_endStageCount = m_targetAngle + 2 * M_PI * m_circleCount;
                    m_res->setTimeHold(recCount());
                    m_lblStage->setText(tr("Этап") + " : " + EvolventaDefines::StageValueName.value(m_stage));
                }
            }
            else
            //! Удержание
            if (m_stage == EvolventaDefines::stgHold)
            {
                if (m_targetAngle >= m_endStageCount)
                {
                    m_stage = EvolventaDefines::stgConvolution;
                    m_res->setTimeConvolution(recCount());
                    m_lblStage->setText(tr("Этап") + " : " + EvolventaDefines::StageValueName.value(m_stage));
                }
            }
            else
            //! Сворачивание
            if (m_stage == EvolventaDefines::stgConvolution)
            {
                if (m_targetAmpl > 0)
                    m_targetAmpl = m_targetAmpl - 0.04 / (static_cast<double>(freqStab()) / 50.0);
                else
                {
                    finishTest();
                }
            }
        }
    }
}

void EvolventaExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    StabDynamicTestExecute::on_communicationError(drvName, port, errorCode);
}

void EvolventaExecute::setTargetPolar(const double angle, const double ampl)
{
    double mx = 0;
    double my = 0;
    //! По часовой стрелке
    if (m_directionMode == BaseUtils::dmClockwise)
    {
        mx = ampl * sin(angle);
        my = ampl * cos(angle);
//MX := Round ( (SKG.Width - iScaleField) div 2 + TargetMark.Width div 2 + aAmpl /
//              SKG.AreaSize * ((SKG.Width - iScaleField) div 2 - TargetMark.Width div 2) * SIN(aAlfa) );
//MY := Round ( (SKG.Height - iScaleField) div 2 - TargetMark.Height - aAmpl /
//              SKG.AreaSize * ((SKG.Height - iScaleField) div 2 - TargetMark.Height) * COS(aAlfa) );
    }
    else
    //! Против часовой стрелки
    if (m_directionMode == BaseUtils::dmCounterClockwise)
    {
        mx = ampl * cos(angle);
        my = ampl * sin(angle);
    }

    //! Сохраняем траекторию в БД
    SignalsDefines::StabRec rec(std::make_tuple(mx, my));
    m_targetSign->addValue(rec);

    setTarget(mx, my);
}
