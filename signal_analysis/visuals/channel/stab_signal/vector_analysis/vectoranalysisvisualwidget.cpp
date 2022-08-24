#include "vectoranalysisvisualwidget.h"
#include "ui_vectoranalysisvisualwidget.h"

#include <QStandardItemModel>
#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "vectorfactors.h"
#include "settingsprovider.h"

VectorAnalysisVisualWidget::VectorAnalysisVisualWidget(VisualDescriptor* visual,
                                                       const QString& testUid, const QString& probeUid, const QString& channelId,
                                                       QWidget *parent) :
    ChannelVisual(visual, testUid, probeUid, channelId, parent),
    ui(new Ui::VectorAnalysisVisualWidget)
{
    ui->setupUi(this);
}

VectorAnalysisVisualWidget::~VectorAnalysisVisualWidget()
{
    delete ui;
}

bool VectorAnalysisVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctStabilogram;
}

void VectorAnalysisVisualWidget::calculate()
{
    if (!m_factors)
        m_factors = new VectorFactors(testUid(), probeUid(), channelId());

    showFactors();
    showAccumulationFunction();
    showVectorCloud();

    restoreSplitterPosition();
}

void VectorAnalysisVisualWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void VectorAnalysisVisualWidget::on_plusButtonClicked()
{
    ui->wgtVectorCloud->setDiapazone(ui->wgtVectorCloud->diap() / 2);
}

void VectorAnalysisVisualWidget::on_minusButtonClicked()
{
    ui->wgtVectorCloud->setDiapazone(ui->wgtVectorCloud->diap() * 2);
}

void VectorAnalysisVisualWidget::showFactors()
{
    auto *model = new QStandardItemModel();

    for (int i = 0; i < m_factors->size(); ++i)
    {
        auto uid = m_factors->factorUid(i);

        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uid);
        QString fn = fi.name();
        if (fi.measure() != "")
            fn = fn + ", " + fi.measure();
        auto* itemName = new QStandardItem(fn);
        itemName->setEditable(false);

        auto fvf = m_factors->factorValueFormatted(uid);
        auto* itemValue = new QStandardItem(fvf);
        itemValue->setEditable(false);

        model->appendRow(QList<QStandardItem*>() << itemName << itemValue);
    }

    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Значение"));
    ui->tvFactors->setModel(model);
    ui->tvFactors->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvFactors->header()->resizeSection(0, 500);
}

void VectorAnalysisVisualWidget::showAccumulationFunction()
{
    ui->wgtAccumulationFunc->setTitle(tr("Функция накопления"));
    ui->wgtAccumulationFunc->setXAxizName(tr("Зоны"));
    ui->wgtAccumulationFunc->setYAxizName(tr("Вероятность, %"));
    ui->wgtAccumulationFunc->setDiap(0, 100);

    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(VectorFactorsDefines::KFRUid);
    ui->wgtAccumulationFunc->setValue(m_factors->factorValueFormatted(VectorFactorsDefines::KFRUid) + " " + fi.measure());

    for (int i = 0; i < VectorFactorsDefines::DiapsCount; ++i)
        ui->wgtAccumulationFunc->add(m_factors->accumulationFuncValue(i) * 100);

}

void VectorAnalysisVisualWidget::showVectorCloud()
{
    ui->wgtVectorCloud->setTitle(tr("Облако векторов"));
    ui->wgtVectorCloud->setDiapazone(m_factors->diapRangeLimit(VectorFactorsDefines::DiapsCount - 1));
    for (int i = 0; i < VectorFactorsDefines::DiapsCount; ++i)
        ui->wgtVectorCloud->addDiapLimit(m_factors->diapRangeLimit(i));
    for (int i = 0; i < m_factors->vectorCount(); ++i)
        ui->wgtVectorCloud->add(m_factors->vector(i));

}

void VectorAnalysisVisualWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("VectorAnalysisVisualWidget", "HSplitterPosition", ui->splHorizontal->saveState());
    SettingsProvider::setValueToRegAppCopy("VectorAnalysisVisualWidget", "VSplitterPosition", ui->splVertical->saveState());
}

void VectorAnalysisVisualWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("VectorAnalysisVisualWidget", "HSplitterPosition").toByteArray();
    ui->splHorizontal->restoreState(val);
    val = SettingsProvider::valueFromRegAppCopy("VectorAnalysisVisualWidget", "VSplitterPosition").toByteArray();
    ui->splVertical->restoreState(val);
}
