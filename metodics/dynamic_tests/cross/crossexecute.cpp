#include "crossexecute.h"
#include "ui_crossexecute.h"

//#include "testresultdata.h"
//#include "aanalyserapplication.h"
//#include "driver.h"
//#include "channelsutils.h"
//#include "settingsprovider.h"
//#include "executewidget.h"

//#include <QTimer>
//#include <QMessageBox>
#include <QDebug>

CrossExecute::CrossExecute(QWidget *parent) :
    StabDynamicTestExecute(parent),
    ui(new Ui::CrossExecute)
{
    ui->setupUi(this);

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

    auto csm = params["change_stage_mode"].toString();
    m_changeStateMode = CrossDefines::ChangeStateModeValueIndex.value(csm);
    auto dm = params["direction_mode"].toString();
    m_directionMode = CrossDefines::DirectionModeValueIndex.value(dm);

    StabDynamicTestExecute::setParams(params);
}

void CrossExecute::start()
{
    StabDynamicTestExecute::start();
}

//void CrossExecute::closeEvent(QCloseEvent *event)
//{

//}

//void CrossExecute::start()
//{
//}

//void CrossExecute::scaleChange(int scaleId)
//{

//}

//void CrossExecute::getData(DeviceProtocols::DeviceData *data)
//{
//}

//void CrossExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
//{

//}

//void CrossExecute::zeroing()
//{

//}

//void CrossExecute::calibrate()
//{

//}

//void CrossExecute::recording()
//{

//}

//void CrossExecute::on_advChannelsClicked(bool checked)
//{
//}

//void CrossExecute::splitterMoved(int pos, int index)
//{

//}

