#include "jumpheighttestexecute.h"
#include "ui_jumpheighttestexecute.h"

JumpHeightTestExecute::JumpHeightTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JumpHeightTestExecute)
{
    ui->setupUi(this);
}

JumpHeightTestExecute::~JumpHeightTestExecute()
{
    delete ui;
}

void JumpHeightTestExecute::setParams(const QJsonObject &params)
{
    auto sFK = params["finish_kind"].toString();
    m_testFinishKind = JumpHeightTestDefines::TestFinishKindIndex.value(sFK);

    m_jumpsCount = params["jumps_count"].toInt();
    m_testTime = params["time"].toInt();

    auto sStr = params["strategy"].toString();
    m_strategy = JumpHeightTestDefines::StrategyIndex.value(sStr);

    m_contactTimeBound = params["contact_time_bound"].toInt();
}
