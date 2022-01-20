#include "teppingtestvisualize.h"
#include "ui_teppingtestvisualize.h"

#include "datadefines.h"
#include "teppingtestcalculator.h"
#include "settingsprovider.h"
#include "dynamicdiagram.h"
#include "baseutils.h"

#include <QFileDialog>
#include <QDebug>

TeppingTestVisualize::TeppingTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeppingTestVisualize)
{
    ui->setupUi(this);
    ui->twPages->setCurrentIndex(0);
    ui->frTableTool->setVisible(false);
}

TeppingTestVisualize::~TeppingTestVisualize()
{
    delete ui;
}

void TeppingTestVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new TeppingTestCalculator(testUid, this);
        m_calculator->calculate();
    }
    if (m_calculator->primaryFactorsCount() >= 7)
    {
        auto fctJumpsCount = m_calculator->primaryFactor(0);
        auto fctTimeTest = m_calculator->primaryFactor(1);
        auto fctTNoContactLeftAvg = m_calculator->primaryFactor(2);
        auto fctTNoContactRightAvg = m_calculator->primaryFactor(3);
        auto fctTContactLeftAvg = m_calculator->primaryFactor(4);
        auto fctTContactRightAvg = m_calculator->primaryFactor(5);
        auto fctTemp = m_calculator->primaryFactor(6);
        ui->lblStepsCount->setText(QString(tr("Количество шагов") + " %1").arg(fctJumpsCount->value()));
        ui->lblTimeTest->setText(QString(tr("Общее время шагов") + " %1 " + tr("сек")).arg(fctTimeTest->value()));
        ui->lblTimeAvgLeftLeg->setText(QString(tr("Быстрота одиночного движения левой ноги") + " %1 " + tr("сек")).arg(fctTNoContactLeftAvg->value()));
        ui->lblTimeAvgRightLeg->setText(QString(tr("Быстрота одиночного движения правой ноги") + " %1 " + tr("сек")).arg(fctTNoContactRightAvg->value()));
        ui->lblContactAvgLeftLeg->setText(QString(tr("Время контакта с опорой левой ноги") + " %1 " + tr("сек")).arg(fctTContactLeftAvg->value()));
        ui->lblContactAvgRightLeg->setText(QString(tr("Время контакта с опорой правой ноги") + " %1 " + tr("сек")).arg(fctTContactRightAvg->value()));
        ui->lblTemp->setText(QString(tr("Темп") + " %1 " + tr("шагов/мин")).arg(fctTemp->value()));

        setTable();
        setDiags();
    }
}

void TeppingTestVisualize::on_selectGraph()
{
    ui->wgtDiagLeft->setKind(DynamicDiagram::KindGraph);
    ui->wgtDiagRight->setKind(DynamicDiagram::KindGraph);

    SettingsProvider::setValueToRegAppCopy("JumpTest", "TeppingTestDiagKind", static_cast<int>(DynamicDiagram::KindGraph));
}

void TeppingTestVisualize::on_selectBar()
{
    ui->wgtDiagLeft->setKind(DynamicDiagram::KindBar);
    ui->wgtDiagRight->setKind(DynamicDiagram::KindBar);

    SettingsProvider::setValueToRegAppCopy("JumpTest", "TeppingTestDiagKind", static_cast<int>(DynamicDiagram::KindBar));
}

void TeppingTestVisualize::on_select3D(bool checked)
{
    if (checked)
    {
        ui->wgtDiagLeft->setVolume(DynamicDiagram::Volume3D);
        ui->wgtDiagRight->setVolume(DynamicDiagram::Volume3D);
        SettingsProvider::setValueToRegAppCopy("JumpTest", "TeppingTestDiagVolume", static_cast<int>(DynamicDiagram::Volume3D));
    }
    else
    {
        ui->wgtDiagLeft->setVolume(DynamicDiagram::Volume2D);
        ui->wgtDiagRight->setVolume(DynamicDiagram::Volume2D);
        SettingsProvider::setValueToRegAppCopy("JumpTest", "TeppingTestDiagVolume", static_cast<int>(DynamicDiagram::Volume2D));
    }
}

void TeppingTestVisualize::on_selectPage(const int pageIdx)
{
    ui->frDiagTool->setVisible(pageIdx == 0);
    ui->frTableTool->setVisible(pageIdx == 1);
}

void TeppingTestVisualize::on_exportToMSExcel()
{
    QString path = DataDefines::aanalyserDocumentsPath();
    auto fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Файл для экспорта таблицы"),
                                                 path,
                                                 tr("Файлы с разделенными значениями *.csv (*.csv)"));
    if (fileName != "")
        BaseUtils::modelToMSExcel(ui->tvSteps->model(), fileName);
}

void TeppingTestVisualize::on_selectDiagItemLeft(const int idx)
{
    if (idx >= 0 && idx < m_calculator->stepsCount(BaseDefines::Right)
             && idx < m_calculator->stepsCount(BaseDefines::Left))
    {
        showCurrentValues(idx);
        ui->wgtDiagRight->doSelectItem(idx);
    }
}

void TeppingTestVisualize::on_selectDiagItemRight(const int idx)
{
    if (idx >= 0 && idx < m_calculator->stepsCount(BaseDefines::Right)
             && idx < m_calculator->stepsCount(BaseDefines::Left))
    {
        showCurrentValues(idx);
        ui->wgtDiagLeft->doSelectItem(idx);
    }
}

void TeppingTestVisualize::setTable()
{
    auto* model = new QStandardItemModel(ui->tvSteps);

    int steps = m_calculator->stepsCount(BaseDefines::Left) > m_calculator->stepsCount(BaseDefines::Right) ?
                m_calculator->stepsCount(BaseDefines::Left) : m_calculator->stepsCount(BaseDefines::Right);
    for (int i = 0; i < steps; ++i)
    {
        SignalsDefines::TeppingStepRec stepLeft(0, 0);
        SignalsDefines::TeppingStepRec stepRight(0, 0);
        if (i < m_calculator->stepsCount(BaseDefines::Left))
            stepLeft = m_calculator->step(BaseDefines::Left, i);
        if (i < m_calculator->stepsCount(BaseDefines::Right))
            stepRight = m_calculator->step(BaseDefines::Right, i);

        QString sTNoContactLeft = "-";
        if (stepLeft.timeNoContact > 0)
            sTNoContactLeft = QString::number(stepLeft.timeNoContact);
        QString sTContactLeft = "-";
        if (stepLeft.timeContact > 0)
            sTContactLeft = QString::number(stepLeft.timeContact);
        QString sTNoContactRight = "-";
        if (stepRight.timeNoContact > 0)
            sTNoContactRight = QString::number(stepRight.timeNoContact);
        QString sTContactRight = "-";
        if (stepRight.timeContact > 0)
            sTContactRight = QString::number(stepRight.timeContact);

        auto *itemN = new QStandardItem(QString::number(i + 1));
        itemN->setEditable(false);
        auto *itemTNCL = new QStandardItem(sTNoContactLeft);
        itemTNCL->setEditable(false);
        auto *itemTCL = new QStandardItem(sTContactLeft);
        itemTCL->setEditable(false);
        auto *itemTNCR = new QStandardItem(sTNoContactRight);
        itemTNCR->setEditable(false);
        auto *itemTCR = new QStandardItem(sTContactRight);
        itemTCR->setEditable(false);

        model->appendRow(QList<QStandardItem*>() << itemN << itemTNCL << itemTCL << itemTNCR << itemTCR);
    }

    ui->tvSteps->setModel(model);
    model->setHorizontalHeaderLabels(QStringList()
                                     << tr("N")
                                     << tr("Левая нога\nБыстрота одиночного движения")
                                     << tr("Левая нога\nВремя контакта")
                                     << tr("Правая нога\nБыстрота одиночного движения")
                                     << tr("Правая нога\nВремя контакта"));
    ui->tvSteps->header()->resizeSection(0, 80);
    ui->tvSteps->header()->resizeSection(1, 200);
    ui->tvSteps->header()->resizeSection(2, 150);
    ui->tvSteps->header()->resizeSection(3, 200);
    ui->tvSteps->header()->resizeSection(4, 150);
}

void TeppingTestVisualize::setDiags()
{
    restoreGraphParams();
    ui->wgtDiagLeft->setTitle(tr("Быстрота одиночного движения левой ноги, сек"));
    ui->wgtDiagRight->setTitle(tr("Быстрота одиночного движения правой ноги, сек"));
    ui->wgtDiagLeft->setAxisSpaceBottom(15);
    ui->wgtDiagLeft->setTitleHeight(20);
    ui->wgtDiagRight->setAxisSpaceBottom(15);
    ui->wgtDiagRight->setTitleHeight(20);

    int steps = m_calculator->stepsCount(BaseDefines::Left) > m_calculator->stepsCount(BaseDefines::Right) ?
                m_calculator->stepsCount(BaseDefines::Left) : m_calculator->stepsCount(BaseDefines::Right);
    for (int i = 0; i < steps; ++i)
    {
        SignalsDefines::TeppingStepRec stepLeft(0, 0);
        SignalsDefines::TeppingStepRec stepRight(0, 0);
        if (i < m_calculator->stepsCount(BaseDefines::Left))
            stepLeft = m_calculator->step(BaseDefines::Left, i);
        if (i < m_calculator->stepsCount(BaseDefines::Right))
            stepRight = m_calculator->step(BaseDefines::Right, i);

        auto itemL = new DiagItem(stepLeft.timeNoContact, QString::number(i + 1));
        ui->wgtDiagLeft->appendItem(itemL);

        auto itemR = new DiagItem(stepRight.timeNoContact, QString::number(i + 1));
        ui->wgtDiagRight->appendItem(itemR);
    }

    connect(ui->wgtDiagLeft, &DynamicDiagram::selectItem, this, &TeppingTestVisualize::on_selectDiagItemLeft);
    connect(ui->wgtDiagRight, &DynamicDiagram::selectItem, this, &TeppingTestVisualize::on_selectDiagItemRight);
}

void TeppingTestVisualize::restoreGraphParams()
{
    auto kindCode = SettingsProvider::valueFromRegAppCopy("JumpTest", "TeppingTestDiagKind", 1).toInt();
    DynamicDiagram::Kind kind = static_cast<DynamicDiagram::Kind>(kindCode);
    ui->wgtDiagLeft->setKind(kind);
    ui->wgtDiagRight->setKind(kind);
    ui->btnGraph->setChecked(kind == DynamicDiagram::KindGraph);
    ui->btnBar->setChecked(kind == DynamicDiagram::KindBar);

    auto volumeCode = SettingsProvider::valueFromRegAppCopy("JumpTest", "TeppingTestDiagVolume", 1).toInt();
    DynamicDiagram::Volume volume = static_cast<DynamicDiagram::Volume>(volumeCode);
    ui->wgtDiagLeft->setVolume(volume);
    ui->wgtDiagRight->setVolume(volume);
    ui->btn3D->setChecked(volume == DynamicDiagram::Volume3D);
}

void TeppingTestVisualize::showCurrentValues(const int idx)
{
    ui->wgtDiagLeft->setTitle(tr("Быстрота одиночного движения левой ноги, сек") +
                              QString(" (%1)").arg(m_calculator->step(BaseDefines::Left, idx).timeNoContact));
    ui->wgtDiagRight->setTitle(tr("Быстрота одиночного движения правой ноги, сек") +
                              QString(" (%1)").arg(m_calculator->step(BaseDefines::Right, idx).timeNoContact));
}
