#include "runningmodedialog.h"
#include "ui_runningmodedialog.h"

RunningModeDialog::RunningModeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RunningModeDialog)
{
    ui->setupUi(this);
}

RunningModeDialog::~RunningModeDialog()
{
    delete ui;
}

void RunningModeDialog::setRunningMode(const BaseDefines::RunningMode rm)
{
    m_runningMode = rm;
    if (rm == BaseDefines::rmOperator)
        ui->btnOperator->setChecked(true);
    else
    if (rm == BaseDefines::rmAutomatic)
        ui->btnAutomatic->setChecked(true);
}

void RunningModeDialog::on_setOperatorMode()
{
    m_runningMode = BaseDefines::rmOperator;
}

void RunningModeDialog::on_setAutoMode()
{
    m_runningMode = BaseDefines::rmAutomatic;
}
