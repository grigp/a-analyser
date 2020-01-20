#include "droptestexecute.h"
#include "ui_droptestexecute.h"


DropTestExecute::DropTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DropTestExecute)
{
    ui->setupUi(this);
}

DropTestExecute::~DropTestExecute()
{
    delete ui;
}

void DropTestExecute::setParams(const QJsonObject &params)
{
    auto sFK = params["finish_kind"].toString();
    m_testFinishKind = JumpPlateDefines::TestFinishKindIndex.value(sFK);
    m_jumpsMax = params["jumps_count"].toInt();
    m_testTime = params["time"].toInt();
}
