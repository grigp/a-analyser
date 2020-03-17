#include "droptestvisualize.h"
#include "ui_droptestvisualize.h"

#include "droptestcalculator.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "dynamicdiagram.h"
#include "settingsprovider.h"

#include <QDebug>

namespace  {

}

DropTestVisualize::DropTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DropTestVisualize)
{
    ui->setupUi(this);
    ui->twPages->setCurrentIndex(0);
}

DropTestVisualize::~DropTestVisualize()
{
    delete ui;
}

void DropTestVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new DropTestCalculator(testUid, this);
        m_calculator->calculate();
    }

    if (m_calculator->primaryFactorsCount() >= 5)
    {
        auto fctJumpsCount = m_calculator->primaryFactor(0);
        auto fctTimeTest = m_calculator->primaryFactor(1);
        auto fctHeightAvg = m_calculator->primaryFactor(2);
        auto fctTimeContactAvg = m_calculator->primaryFactor(3);
        auto fctTimeNoContactAvg = m_calculator->primaryFactor(4);
        ui->lblJumpsCount->setText(QString(tr("Количество прыжков") + " %1").arg(fctJumpsCount->value()));
        ui->lblTimeTest->setText(QString(tr("Общее время прыжков") + " %1 " + tr("сек")).arg(fctTimeTest->value()));
        ui->lblHeightAvg->setText(QString(tr("Средняя высота прыжков") + " %1 " + tr("м")).arg(fctHeightAvg->value()));
        ui->lblTimeContactAvg->setText(QString(tr("Среднее время контактной фазы") + " %1 " + tr("сек")).arg(fctTimeContactAvg->value()));
        ui->lblTimeNoContactAvg->setText(QString(tr("Среднее время бесконтактной фазы") + " %1 " + tr("сек")).arg(fctTimeNoContactAvg->value()));

        auto* model = new QStandardItemModel(ui->tvJumpsDrop);

        for (int i = 0; i < m_calculator->jumpsCount(); ++i)
        {
            auto jump = m_calculator->jump(i);

            auto *itemN = new QStandardItem(QString::number(i + 1));
            itemN->setEditable(false);
            auto *itemC = new QStandardItem(QString::number(jump.timeContact));
            itemC->setData(jump.timeContact, ValueRole);
            itemC->setEditable(false);
            auto *itemNC = new QStandardItem(QString::number(jump.timeNoContact));
            itemNC->setData(jump.timeNoContact, ValueRole);
            itemNC->setEditable(false);
            int massa = getPatientMassa(testUid);
            auto *itemM = new QStandardItem(QString::number(massa));
            itemM->setData(massa, ValueRole);
            itemM->setEditable(false);
            auto *itemFH = new QStandardItem(QString::number(jump.fallHeight));
            itemFH->setData(jump.fallHeight, ValueRole);
            itemFH->setEditable(false);
            auto *itemH = new QStandardItem(QString::number(jump.height));
            itemH->setData(jump.height, ValueRole);
            itemH->setEditable(false);
            auto *itemP = new QStandardItem(QString::number(jump.power));
            itemP->setData(jump.power, ValueRole);
            itemP->setEditable(false);
            auto *itemS = new QStandardItem(QString::number(jump.stiffness));
            itemS->setData(jump.stiffness, ValueRole);
            itemS->setEditable(false);
            auto *itemIS = new QStandardItem(QString::number(jump.initialSpeed));
            itemIS->setData(jump.initialSpeed, ValueRole);
            itemIS->setEditable(false);
            auto *itemRSI = new QStandardItem(QString::number(jump.rsi));
            itemRSI->setData(jump.rsi, ValueRole);
            itemRSI->setEditable(false);

            model->appendRow(QList<QStandardItem*>() << itemN << itemC << itemNC << itemM << itemFH << itemH
                             << itemP << itemS << itemIS << itemRSI);
        }

        ui->tvJumpsDrop->setModel(model);
        model->setHorizontalHeaderLabels(QStringList() << tr("N")
                                         << tr("Контактная\nфаза, сек")
                                         << tr("Бесконтактная\nфаза, сек")
                                         << tr("Масса\nпациента, кг")
                                         << tr("Высота\nспрыгивания, м")
                                         << tr("Высота\nпрыжка, м")
                                         << tr("Мощность")
                                         << tr("Жесткость")
                                         << tr("Начальная\nскорость, м/сек")
                                         << tr("Индекс\nреактивной силы"));
        ui->tvJumpsDrop->header()->resizeSections(QHeaderView::ResizeToContents);
        ui->tvJumpsDrop->header()->resizeSection(0, 50);

        setDiagram();
    }
}

void DropTestVisualize::on_selectGraph()
{
    ui->wgtDiag1->setKind(DynamicDiagram::KindGraph);
    ui->wgtDiag2->setKind(DynamicDiagram::KindGraph);
    ui->wgtDiag3->setKind(DynamicDiagram::KindGraph);

    SettingsProvider::setValueToRegAppCopy("JumpTest", "DropTestDiagKind", static_cast<int>(DynamicDiagram::KindGraph));
}

void DropTestVisualize::on_selectBar()
{
    ui->wgtDiag1->setKind(DynamicDiagram::KindBar);
    ui->wgtDiag2->setKind(DynamicDiagram::KindBar);
    ui->wgtDiag3->setKind(DynamicDiagram::KindBar);

    SettingsProvider::setValueToRegAppCopy("JumpTest", "DropTestDiagKind", static_cast<int>(DynamicDiagram::KindBar));
}

void DropTestVisualize::on_select3D(bool checked)
{
    if (checked)
    {
        ui->wgtDiag1->setVolume(DynamicDiagram::Volume3D);
        ui->wgtDiag2->setVolume(DynamicDiagram::Volume3D);
        ui->wgtDiag3->setVolume(DynamicDiagram::Volume3D);
        SettingsProvider::setValueToRegAppCopy("JumpTest", "DropTestDiagVolume", static_cast<int>(DynamicDiagram::Volume3D));
    }
    else
    {
        ui->wgtDiag1->setVolume(DynamicDiagram::Volume2D);
        ui->wgtDiag2->setVolume(DynamicDiagram::Volume2D);
        ui->wgtDiag3->setVolume(DynamicDiagram::Volume2D);
        SettingsProvider::setValueToRegAppCopy("JumpTest", "DropTestDiagVolume", static_cast<int>(DynamicDiagram::Volume2D));
    }
}

void DropTestVisualize::on_selectFactorDiag1(int idx)
{
    Q_UNUSED(idx);
    showDiagram(ui->cbFactors1->currentData().toInt(), ui->wgtDiag1);
    SettingsProvider::setValueToRegAppCopy("JumpTest", "DropTestFactor1", ui->cbFactors1->currentData().toInt());
}

void DropTestVisualize::on_selectFactorDiag2(int idx)
{
    Q_UNUSED(idx);
    showDiagram(ui->cbFactors2->currentData().toInt(), ui->wgtDiag2);
    SettingsProvider::setValueToRegAppCopy("JumpTest", "DropTestFactor2", ui->cbFactors2->currentData().toInt());
}

void DropTestVisualize::on_selectFactorDiag3(int idx)
{
    Q_UNUSED(idx);
    showDiagram(ui->cbFactors3->currentData().toInt(), ui->wgtDiag3);
    SettingsProvider::setValueToRegAppCopy("JumpTest", "DropTestFactor3", ui->cbFactors3->currentData().toInt());
}

void DropTestVisualize::on_selectDiag1Item(const int idx)
{
    if (idx >= 0 && idx < m_calculator->jumpsCount())
    {
        showSelectedValues(idx);
        ui->wgtDiag2->doSelectItem(idx);
        ui->wgtDiag3->doSelectItem(idx);
    }
}

void DropTestVisualize::on_selectDiag2Item(const int idx)
{
    if (idx >= 0 && idx < m_calculator->jumpsCount())
    {
        showSelectedValues(idx);
        ui->wgtDiag1->doSelectItem(idx);
        ui->wgtDiag3->doSelectItem(idx);
    }
}

void DropTestVisualize::on_selectDiag3Item(const int idx)
{
    if (idx >= 0 && idx < m_calculator->jumpsCount())
    {
        showSelectedValues(idx);
        ui->wgtDiag1->doSelectItem(idx);
        ui->wgtDiag2->doSelectItem(idx);
    }
}

void DropTestVisualize::showSelectedValues(const int idx)
{
    {
        auto column = ui->cbFactors1->currentData().toInt();
        auto value = ui->tvJumpsDrop->model()->index(idx, column).data(ValueRole).toDouble();
        ui->lblFactorValue1->setText(QString("(%1)").arg(value));
    }
    {
        auto column = ui->cbFactors2->currentData().toInt();
        auto value = ui->tvJumpsDrop->model()->index(idx, column).data(ValueRole).toDouble();
        ui->lblFactorValue2->setText(QString("(%1)").arg(value));
    }
    {
        auto column = ui->cbFactors3->currentData().toInt();
        auto value = ui->tvJumpsDrop->model()->index(idx, column).data(ValueRole).toDouble();
        ui->lblFactorValue3->setText(QString("(%1)").arg(value));
    }
}

int DropTestVisualize::getPatientMassa(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        DataDefines::PatientKard pk;
        if (DataProvider::getPatient(ti.patientUid, pk))
            return pk.massa;
    }
    return 0;
}

void DropTestVisualize::setDiagram()
{
    auto factor1 = SettingsProvider::valueFromRegAppCopy("JumpTest", "DropTestFactor1", 5).toInt();
    auto factor2 = SettingsProvider::valueFromRegAppCopy("JumpTest", "DropTestFactor2", 1).toInt();
    auto factor3 = SettingsProvider::valueFromRegAppCopy("JumpTest", "DropTestFactor3", 9).toInt();

    restoreGraphParams();
    ui->wgtDiag1->setAxisSpaceBottom(15);
    ui->wgtDiag1->setTitleHeight(1);
    ui->wgtDiag2->setAxisSpaceBottom(15);
    ui->wgtDiag2->setTitleHeight(1);
    ui->wgtDiag3->setAxisSpaceBottom(15);
    ui->wgtDiag3->setTitleHeight(1);

    foreach (auto key, FactorsByColumn.keys())
    {
        ui->cbFactors1->addItem(FactorsByColumn.value(key), key);
        ui->cbFactors2->addItem(FactorsByColumn.value(key), key);
        ui->cbFactors3->addItem(FactorsByColumn.value(key), key);
    }
    ui->cbFactors1->setCurrentText(FactorsByColumn.value(factor1));
    showDiagram(ColHeight, ui->wgtDiag1);
    ui->cbFactors2->setCurrentText(FactorsByColumn.value(factor2));
    showDiagram(ColContact, ui->wgtDiag2);
    ui->cbFactors3->setCurrentText(FactorsByColumn.value(factor3));
    showDiagram(ColRSI, ui->wgtDiag3);

    connect(ui->wgtDiag1, &DynamicDiagram::selectItem, this, &DropTestVisualize::on_selectDiag1Item);
    connect(ui->wgtDiag2, &DynamicDiagram::selectItem, this, &DropTestVisualize::on_selectDiag2Item);
    connect(ui->wgtDiag3, &DynamicDiagram::selectItem, this, &DropTestVisualize::on_selectDiag3Item);
}

void DropTestVisualize::showDiagram(const int column, DynamicDiagram *diag)
{
    diag->clear();
    auto *model = ui->tvJumpsDrop->model();
    for (int i = 0; i < model->rowCount(); ++i)
    {
        auto v = model->index(i, column).data(ValueRole).toDouble();
        auto item = new DiagItem(v, QString::number(i + 1));
        diag->appendItem(item);
    }
}

void DropTestVisualize::restoreGraphParams()
{
    auto kindCode = SettingsProvider::valueFromRegAppCopy("JumpTest", "DropTestDiagKind", 1).toInt();
    DynamicDiagram::Kind kind = static_cast<DynamicDiagram::Kind>(kindCode);
    ui->wgtDiag1->setKind(kind);
    ui->wgtDiag2->setKind(kind);
    ui->wgtDiag3->setKind(kind);
    ui->btnGraph->setChecked(kind == DynamicDiagram::KindGraph);
    ui->btnBar->setChecked(kind == DynamicDiagram::KindBar);

    auto volumeCode = SettingsProvider::valueFromRegAppCopy("JumpTest", "DropTestDiagVolume", 1).toInt();
    DynamicDiagram::Volume volume = static_cast<DynamicDiagram::Volume>(volumeCode);
    ui->wgtDiag1->setVolume(volume);
    ui->wgtDiag2->setVolume(volume);
    ui->wgtDiag3->setVolume(volume);
    ui->btn3D->setChecked(volume == DynamicDiagram::Volume3D);
}
