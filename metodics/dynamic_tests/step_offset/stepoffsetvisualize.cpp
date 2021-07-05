#include "stepoffsetvisualize.h"
#include "ui_stepoffsetvisualize.h"

#include <QByteArray>
#include <QStandardItemModel>
#include <QDebug>

#include "aanalyserapplication.h"
#include "stepoffsetcalculator.h"
#include "stepoffsetfactors.h"
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

        showTransients();
        showTable();
        showParams();
        showConslution();
        showConslutionStrategy();
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

void StepOffsetVisualize::showTransients()
{
    int min = qMin(m_calculator->bufferCompensationCount(), m_calculator->bufferReturnCount());
    ui->wgtProcess->setFreq(m_calculator->freq());

    //! Время броска от стимула = время броска + время размаха.
    //! Если время размаха == 0, то время броска + латентный период.
    double spurtTime = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SpurtTimeUid) +
                       m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SwingTimeUid);
    if (m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SwingTimeUid) - 0 < 1e-10)
        spurtTime = spurtTime + m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::LatentUid);
    ui->wgtProcess->setParams(StepOffsetDefines::stgCompensaton,
                              m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::LatentUid),
                              m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SwingTimeUid),
                              spurtTime,
                              m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::ReactionTimeUid),
                              m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::StatismUid),
                              m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::RetentionDeviationUid));

    spurtTime = m_calculator->factorValue(StepOffsetFactorsDefines::Return::SpurtTimeUid) +
                           m_calculator->factorValue(StepOffsetFactorsDefines::Return::SwingTimeUid);
        if (m_calculator->factorValue(StepOffsetFactorsDefines::Return::SwingTimeUid) - 0 < 1e-10)
            spurtTime = spurtTime + m_calculator->factorValue(StepOffsetFactorsDefines::Return::LatentUid);
    ui->wgtProcess->setParams(StepOffsetDefines::stgReturn,
                              m_calculator->factorValue(StepOffsetFactorsDefines::Return::LatentUid),
                              m_calculator->factorValue(StepOffsetFactorsDefines::Return::SwingTimeUid),
                              spurtTime,
                              m_calculator->factorValue(StepOffsetFactorsDefines::Return::ReactionTimeUid),
                              m_calculator->factorValue(StepOffsetFactorsDefines::Return::StatismUid),
                              m_calculator->factorValue(StepOffsetFactorsDefines::Return::RetentionDeviationUid));

    ui->wgtProcess->beginAddValues();
    for (int i = 0; i < min; ++i)
    {
        ui->wgtProcess->setCompensationValue(m_calculator->bufferCompensationValue(i));
        ui->wgtProcess->setReturnValue(m_calculator->bufferReturnValue(i));
    }
    ui->wgtProcess->endAddValues();
}

void StepOffsetVisualize::showTable()
{
    auto *model = new QStandardItemModel();
    for (int i = 0; i < m_calculator->factorsCount() / 2; ++i)
    {
        auto factorCompUid = m_calculator->factorUid(i);
        auto factorCompVal = m_calculator->factorValue(i);
        auto fiComp = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factorCompUid);
        QString fnComp = fiComp.name();
        if (fiComp.measure() != "")
            fnComp = fnComp + ", " + fiComp.measure();

        auto factorRetUid = m_calculator->factorUid(m_calculator->factorsCount() / 2 + i);
        auto factorRetVal = m_calculator->factorValue(m_calculator->factorsCount() / 2 + i);
        auto fiRet = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factorRetUid);
        QString fnRet = fiRet.name();
        if (fiRet.measure() != "")
            fnRet = fnRet + ", " + fiRet.measure();

        auto *itemName = new QStandardItem(fnComp);
        itemName->setEditable(false);
        auto *itemValueComp = new QStandardItem(QString::number(factorCompVal));
        itemValueComp->setEditable(false);
        auto *itemValueRet = new QStandardItem(QString::number(factorRetVal));
        itemValueRet->setEditable(false);
        model->appendRow(QList<QStandardItem*>() << itemName << itemValueComp << itemValueRet);
    }

    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель")
                                                   << tr("Компенсация воздействия")
                                                   << tr("Возврат в исходное состояние"));
    ui->tvFactors->setModel(model);
    ui->tvFactors->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvFactors->header()->resizeSection(0, 450);
}

void StepOffsetVisualize::showParams()
{
    ui->lblForce->setText(tr("Усилие") + " - " + QString::number(m_calculator->force()) + " " + tr("%"));
    ui->lblDirection->setText(tr("Направление") + " - " + BaseUtils::DirectionValueName.value(m_calculator->direction()));
    ui->lblDeviation->setText(tr("Отклонение") + " - " +
                              QString::number(m_calculator->diap() * m_calculator->force() / 100) +
                              " " + tr("мм"));
    ui->lblRepeatCount->setText(tr("Кол-во повторений") + " - " + QString::number(m_calculator->stepsCount()));
}

void StepOffsetVisualize::showConslution()
{
    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::ReactionTimeUid);
    double v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::ReactionTimeUid);
    ui->lblReactionTime->setText(tr("Время реакции") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("сек"));

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::StatismUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::StatismUid);
    ui->lblStatism->setText(tr("Статизм") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм"));

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::LatentUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::LatentUid);
    ui->lblLatent->setText(tr("Латентный период") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("сек"));

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::SpurtSpeedMMUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SpurtSpeedMMUid);
    ui->lblSpurtSpeed->setText(tr("Скорость броска") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм/сек"));

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::SpurtAmplUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SpurtAmplUid);
    ui->lblSpurtAmpl->setText(tr("Амплитуда броска") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм"));

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::StabilityDeviationUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::StabilityDeviationUid);
    ui->lblStabilityDeviation->setText(tr("Разброс на этапе стабилизации") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм"));

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::RetentionDeviationUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::RetentionDeviationUid);
    ui->lblRetentionDeviation->setText(tr("Разброс на этапе удержания") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм"));

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::CorrectKognErrorUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::CorrectKognErrorUid);
    ui->lblPrecisionKogn->setText(tr("Точность выполнения когнитивным сигналом") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм"));

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::CorrectMotorErrorUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::CorrectMotorErrorUid);
    ui->lblPrecisionMotor->setText(tr("Точность моторной коррекции") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм"));

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::ProcessKindUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::ProcessKindUid);
    ui->lblProcessKind->setText(tr("Тип переходного процесса") + " : " + QString::number(v, 'f', fi.format()));
}

void StepOffsetVisualize::showConslutionStrategy()
{
    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::CorrectDominanceUid);
    double v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::CorrectDominanceUid);
    ui->lblCorrectionPredominace->setText(tr("Преобладание коррекций") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("%"));

    QString korr = "";
    QString colorResume = "color: rgb(100, 100, 100)";
    if ((fabs(v) > 10) && (fabs(v) <= 30))
    {
        korr = tr("незначительно");
        colorResume = "color: rgb(0, 150, 0)";
    }
    else
    if ((fabs(v) > 30) && (fabs(v) <= 50))
    {
        korr = tr("умеренно");
        colorResume = "color: rgb(150, 150, 0)";
    }
    else
    {
        korr = tr("выражено");
        colorResume = "color: rgb(150, 0, 0)";
    }

    QString resume = "";
    if (v > 10)
        resume = tr("Преобладание моторных коррекций") + ' ' + korr;
    else
    if (v < -10)
      resume = tr("Преобладание когнитивных коррекций") + ' ' + korr;
    else
      resume = tr("Нет преобладания типа коррекции");
    ui->lblCorrectionResume->setText(resume);
    ui->lblCorrectionResume->setStyleSheet(colorResume);

    ui->wgtCorrectionDiag->setValue(v);
    ui->wgtCorrectionDiag->setDescriptionLeft(tr("Быстрые коррекции"));
    ui->wgtCorrectionDiag->setDescriptionRight(tr("Медленные коррекции"));
}
