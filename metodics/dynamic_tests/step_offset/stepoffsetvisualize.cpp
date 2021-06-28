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

        int min = qMin(m_calculator->bufferCompensationCount(), m_calculator->bufferReturnCount());
        ui->wgtProcess->setFreq(m_calculator->freq());
        ui->wgtProcess->beginAddValues();
        for (int i = 0; i < min; ++i)
        {
            ui->wgtProcess->setCompensationValue(m_calculator->bufferCompensationValue(i));
            ui->wgtProcess->setReturnValue(m_calculator->bufferReturnValue(i));
        }
        ui->wgtProcess->endAddValues();

        ui->lblForce->setText(tr("Усилие") + " - " + QString::number(m_calculator->force()) + " " + tr("%"));
        ui->lblDirection->setText(tr("Направление") + " - " + BaseUtils::DirectionValueName.value(m_calculator->direction()));
        ui->lblDeviation->setText(tr("Отклонение") + " - " +
                                  QString::number(m_calculator->diap() * m_calculator->force() / 100) +
                                  " " + tr("мм"));
        ui->lblRepeatCount->setText(tr("Кол-во повторений") + " - " + QString::number(m_calculator->stepsCount()));
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
