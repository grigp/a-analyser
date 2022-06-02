#include "addtesttosummarydialog.h"
#include "ui_addtesttosummarydialog.h"

AddTestToSummaryDialog::AddTestToSummaryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTestToSummaryDialog)
{
    ui->setupUi(this);
}

AddTestToSummaryDialog::~AddTestToSummaryDialog()
{
    delete ui;
}

SummaryDefines::AddTestMode AddTestToSummaryDialog::mode() const
{
    if (ui->rbAddToExsistsSummary->isChecked())
        return SummaryDefines::atmExists;
    else
    if (ui->rbAddToNewSummary->isChecked())
        return SummaryDefines::atmNew;
    else
    if (ui->rbAddToActiveSummary->isChecked())
        return SummaryDefines::atmActive;
    return SummaryDefines::atmNone;
}

void AddTestToSummaryDialog::setMode(const SummaryDefines::AddTestMode mode)
{
    if (mode == SummaryDefines::atmExists)
        ui->rbAddToExsistsSummary->setChecked(true);
    else
    if (mode == SummaryDefines::atmNew)
        ui->rbAddToNewSummary->setChecked(true);
    else
    if (mode == SummaryDefines::atmActive)
        ui->rbAddToActiveSummary->setChecked(true);
}

QString AddTestToSummaryDialog::summary() const
{
    return "";
}

void AddTestToSummaryDialog::setSummary(const QString &fn)
{

}

SummaryDefines::Kind AddTestToSummaryDialog::kind() const
{
    if (ui->btnPrimaryFactors->isChecked())
        return SummaryDefines::skPrimary;
    else
    if (ui->btnAllFactors->isChecked())
        return SummaryDefines::skAll;
    return SummaryDefines::skNone;
}

void AddTestToSummaryDialog::setKind(const SummaryDefines::Kind kind)
{
    if (kind == SummaryDefines::skPrimary)
        ui->btnPrimaryFactors->setChecked(true);
    else
    if (kind == SummaryDefines::skAll)
        ui->btnAllFactors->setChecked(true);
}

QString AddTestToSummaryDialog::summaryName() const
{
    return ui->edSummaryName->text();
}

void AddTestToSummaryDialog::setSummaryName(const QString &name)
{
    ui->edSummaryName->setText(name);
}

void AddTestToSummaryDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    ui->edSummaryName->setText("");
}
