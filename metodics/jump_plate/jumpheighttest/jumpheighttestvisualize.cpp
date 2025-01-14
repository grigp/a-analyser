#include "jumpheighttestvisualize.h"
#include "ui_jumpheighttestvisualize.h"

#include "jumpheighttestcalculator.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "dynamicdiagram.h"
#include "settingsprovider.h"
#include "baseutils.h"

#include <QStandardItemModel>
#include <QFileDialog>
#include <QDebug>

JumpHeightTestVisualize::JumpHeightTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JumpHeightTestVisualize)
{
    ui->setupUi(this);
    ui->twPages->setCurrentIndex(0);
    ui->frTableTool->setVisible(false);
}

JumpHeightTestVisualize::~JumpHeightTestVisualize()
{
    delete ui;
}

void JumpHeightTestVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new JumpHeightTestCalculator(testUid, this);
        m_calculator->calculate();
    }

    if (m_calculator->primaryFactorsCount() >= 5)
    {
        getStrategyParams(testUid);

        auto fctJumpsCount = m_calculator->primaryFactor(0);
        auto fctTimeTest = m_calculator->primaryFactor(1);
        auto fctHeightAvg = m_calculator->primaryFactor(2);
        auto fctHeightMax = m_calculator->primaryFactor(3);
        auto fctTimeAvg = m_calculator->primaryFactor(4);
        ui->lblJumpsCount->setText(QString(tr("Количество прыжков") + " %1").arg(fctJumpsCount->value()));
        ui->lblTimeTest->setText(QString(tr("Общее время прыжков") + " %1 " + tr("сек")).arg(fctTimeTest->value()));
        ui->lblHeightAvg->setText(QString(tr("Средняя высота прыжков") + " %1 " + tr("м")).arg(fctHeightAvg->value()));
        ui->lblHeightMax->setText(QString(tr("Максимальная высота прыжков") + " %1 " + tr("м")).arg(fctHeightMax->value()));
        ui->lblTimeAvg->setText(QString(tr("Среднее время на платформе") + " %1 " + tr("сек")).arg(fctTimeAvg->value()));

        setTable();
        setDiagrams();
    }
}

void JumpHeightTestVisualize::on_selectGraph()
{
    ui->wgtDiagHeight->setKind(DynamicDiagramDefines::KindGraph);
    ui->wgtDiagTime->setKind(DynamicDiagramDefines::KindGraph);

    SettingsProvider::setValueToRegAppCopy("JumpTest", "HeightTestDiagKind", static_cast<int>(DynamicDiagramDefines::KindGraph));
}

void JumpHeightTestVisualize::on_selectBar()
{
    ui->wgtDiagHeight->setKind(DynamicDiagramDefines::KindBar);
    ui->wgtDiagTime->setKind(DynamicDiagramDefines::KindBar);

    SettingsProvider::setValueToRegAppCopy("JumpTest", "HeightTestDiagKind", static_cast<int>(DynamicDiagramDefines::KindBar));
}

void JumpHeightTestVisualize::on_select3D(bool checked)
{
    if (checked)
    {
        ui->wgtDiagHeight->setVolume(DynamicDiagramDefines::Volume3D);
        ui->wgtDiagTime->setVolume(DynamicDiagramDefines::Volume3D);
        SettingsProvider::setValueToRegAppCopy("JumpTest", "HeightTestDiagVolume", static_cast<int>(DynamicDiagramDefines::Volume3D));
    }
    else
    {
        ui->wgtDiagHeight->setVolume(DynamicDiagramDefines::Volume2D);
        ui->wgtDiagTime->setVolume(DynamicDiagramDefines::Volume2D);
        SettingsProvider::setValueToRegAppCopy("JumpTest", "HeightTestDiagVolume", static_cast<int>(DynamicDiagramDefines::Volume2D));
    }
}

void JumpHeightTestVisualize::on_selectDiagItemHeight(const int idx)
{
    if (idx >= 0 && idx < m_calculator->jumpsCount())
    {
        showCurrentValues(idx);
        ui->wgtDiagTime->doSelectItem(idx);
    }
}

void JumpHeightTestVisualize::on_selectDiagItemTime(const int idx)
{
    if (idx >= 0 && idx < m_calculator->jumpsCount())
    {
        showCurrentValues(idx);
        ui->wgtDiagHeight->doSelectItem(idx);
    }
}

void JumpHeightTestVisualize::on_selectPage(const int pageIdx)
{
    ui->frDiagTool->setVisible(pageIdx == 0);
    ui->frTableTool->setVisible(pageIdx == 1);
}

void JumpHeightTestVisualize::on_exportToMSExcel()
{
    QString path = DataDefines::aanalyserDocumentsPath();
    auto fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Файл для экспорта таблицы"),
                                                 path,
                                                 tr("Файлы с разделенными значениями *.csv (*.csv)"));
    if (fileName != "")
        BaseUtils::modelToMSExcel(ui->tvJumps->model(), fileName);
}

void JumpHeightTestVisualize::showCurrentValues(const int idx)
{
    ui->wgtDiagHeight->setTitle(tr("Высота прыжка, м") + QString(" (%1)").arg(m_calculator->jump(idx).height));
    ui->wgtDiagTime->setTitle(tr("Время контактной фазы, сек") + QString(" (%1)").arg(m_calculator->jump(idx).timeJump));
}

void JumpHeightTestVisualize::getStrategyParams(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        auto sStrg = ti.params["strategy"].toString();
        m_strategy = JumpHeightTestDefines::StrategyIndex.value(sStrg);
        if (m_strategy == JumpHeightTestDefines::jhsMinContactTime)
        {
            m_contactTimeBound = ti.params["contact_time_bound"].toInt() / 1000.0;
            ui->lblContactTimeBound->setText(QString(tr("Порог времени на платформе") + " %1 " + tr("сек")).arg(m_contactTimeBound));
        }
        ui->lblContactTimeBound->setVisible(m_strategy == JumpHeightTestDefines::jhsMinContactTime);
    }
}

void JumpHeightTestVisualize::setDiagrams()
{
    restoreGraphParams();
    ui->wgtDiagHeight->setTitle(tr("Высота прыжка, м"));
    ui->wgtDiagTime->setTitle(tr("Время контактной фазы, сек"));
    ui->wgtDiagHeight->setAxisSpaceBottom(15);
    ui->wgtDiagHeight->setTitleHeight(20);
    ui->wgtDiagTime->setAxisSpaceBottom(15);
    ui->wgtDiagTime->setTitleHeight(20);

    for (int i = 0; i < m_calculator->jumpsCount(); ++i)
    {
        auto jump = m_calculator->jump(i);

        auto itemH = new DiagItem(jump.height, QString::number(i + 1));
        ui->wgtDiagHeight->appendItem(itemH);

        auto itemT = new DiagItem(jump.timeJump, QString::number(i + 1));
        ui->wgtDiagTime->appendItem(itemT);
    }
    connect(ui->wgtDiagHeight, &DynamicDiagram::selectItem, this, &JumpHeightTestVisualize::on_selectDiagItemHeight);
    connect(ui->wgtDiagTime, &DynamicDiagram::selectItem, this, &JumpHeightTestVisualize::on_selectDiagItemTime);
}

void JumpHeightTestVisualize::setTable()
{
    auto* model = new QStandardItemModel(ui->tvJumps);

    for (int i = 0; i < m_calculator->jumpsCount(); ++i)
    {
        auto jump = m_calculator->jump(i);

        auto *itemN = new QStandardItem(QString::number(i + 1));
        itemN->setEditable(false);
        auto *itemH = new QStandardItem(QString::number(jump.height));
        itemH->setEditable(false);
        auto *itemT = new QStandardItem(QString::number(jump.timeJump));
        itemT->setEditable(false);

        model->appendRow(QList<QStandardItem*>() << itemN << itemH << itemT);
    }

    ui->tvJumps->setModel(model);
    model->setHorizontalHeaderLabels(QStringList() << tr("N") << tr("Высота прыжка, м") << tr("Время контактной фазы, сек"));
    ui->tvJumps->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvJumps->header()->resizeSection(0, 100);
}

void JumpHeightTestVisualize::restoreGraphParams()
{
    auto kindCode = SettingsProvider::valueFromRegAppCopy("JumpTest", "HeightTestDiagKind", 1).toInt();
    DynamicDiagramDefines::Kind kind = static_cast<DynamicDiagramDefines::Kind>(kindCode);
    ui->wgtDiagHeight->setKind(kind);
    ui->wgtDiagTime->setKind(kind);
    ui->btnGraph->setChecked(kind == DynamicDiagramDefines::KindGraph);
    ui->btnBar->setChecked(kind == DynamicDiagramDefines::KindBar);

    auto volumeCode = SettingsProvider::valueFromRegAppCopy("JumpTest", "HeightTestDiagVolume", 1).toInt();
    DynamicDiagramDefines::Volume volume = static_cast<DynamicDiagramDefines::Volume>(volumeCode);
    ui->wgtDiagHeight->setVolume(volume);
    ui->wgtDiagTime->setVolume(volume);
    ui->btn3D->setChecked(volume == DynamicDiagramDefines::Volume3D);
}
