#include "evolventaexecute.h"
#include "ui_evolventaexecute.h"

#include <QDebug>

#include "evolventapatientwindow.h"

EvolventaExecute::EvolventaExecute(QWidget *parent) :
    StabDynamicTestExecute(parent),
    ui(new Ui::EvolventaExecute)
{
    ui->setupUi(this);

    addTarget(0, 0, Qt::green, Qt::darkGreen);
    addMarker();

    setTitle("Тест \"Эвольвента\"");
    isShowValues(false);
    isTraceControl(false);
    setVisibleRecordLength(false);
}

EvolventaExecute::~EvolventaExecute()
{
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
    return new EvolventaPatientWindow(this);
}

void EvolventaExecute::finishTest()
{
    StabDynamicTestExecute::finishTest();
}

void EvolventaExecute::fillSpecific(QFrame *frSpecific)
{
    StabDynamicTestExecute::fillSpecific(frSpecific);
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

    if (isRecording())
    {
    }
    else
    {
    }
}

void EvolventaExecute::getData(DeviceProtocols::DeviceData *data)
{
    StabDynamicTestExecute::getData(data);

    if (selectedChannel() == data->channelId())
    {
        if (isRecording())
        {
            //! Управление углом цели относительно 0
            //if (m_targetAmpl > m_radiusMax / (2 * M_PI * 0.1))  <<< логично - же ???
//            if (m_targetAmpl > m_speedMax / (2 * M_PI * 0.1))
//                m_targetAngle = m_targetAngle + static_cast<double>(m_speedMax) / static_cast<double>(freqStab()) * m_targetAmpl;
//            else
                m_targetAngle = m_targetAngle + (2 * M_PI / static_cast<double>(freqStab())) * 0.1;

            //! Раскручивание
            if (m_stage == EvolventaDefines::stgUpwinding)
            {
                if (m_targetAmpl < m_radiusMax)
                    m_targetAmpl = m_targetAmpl + 0.04 / (static_cast<double>(freqStab()) / 50.0);
                else
                {
                    m_stage = EvolventaDefines::stgHold;
                    m_endStageCount = m_targetAngle + 2 * M_PI * m_circleCount;
                    //TODO! Установка маркера смены этапа на удержание
                }
            }
            else
            //! Удержание
            if (m_stage == EvolventaDefines::stgHold)
            {
                if (m_targetAngle >= m_endStageCount)
                {
                    m_stage = EvolventaDefines::stgConvolution;
                    //TODO! Установка маркера смены этапа на сворачивание
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

            //! Установка цели
            setTargetPolar(m_targetAngle, m_targetAmpl);
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

    setTarget(mx, my);
}
