#include "stepoffsetvisualize.h"
#include "ui_stepoffsetvisualize.h"

#include <QDebug>
#include <QByteArray>

#include "stepoffsetcalculator.h"
#include "settingsprovider.h"

StepOffsetVisualize::StepOffsetVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StepOffsetVisualize)
{
    auto val = SettingsProvider::valueFromRegAppCopy("StepOffsetTestWidget", "CurrentPage").toInt();

    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(val);
    restoreSplitterPosition();
}

StepOffsetVisualize::~StepOffsetVisualize()
{
    delete ui;
}

void StepOffsetVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new StepOffsetCalculator(testUid, this);
        m_calculator->calculate();
    }
}

void StepOffsetVisualize::curPageChanged(int pageIdx)
{
    SettingsProvider::setValueToRegAppCopy("StepOffsetTestWidget", "CurrentPage", pageIdx);
}

void StepOffsetVisualize::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void StepOffsetVisualize::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("StepOffsetTestWidget", "ProcessSplitterPosition", ui->splProcess->saveState());
}

void StepOffsetVisualize::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("StepOffsetTestWidget", "ProcessSplitterPosition").toByteArray();
    ui->splProcess->restoreState(val);
}
