#include "variationpulsogramvisualwidget.h"
#include "ui_variationpulsogramvisualwidget.h"

#include <QStandardItemModel>
#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "pulsefactors.h"
#include "settingsprovider.h"

VariationPulsogramVisualWidget::VariationPulsogramVisualWidget(VisualDescriptor* visual,
                                                               const QString& testUid,
                                                               const QString& probeUid,
                                                               const QString& channelId,
                                                               QWidget *parent) :
    ChannelVisual (visual, testUid, probeUid, channelId, parent),
    ui(new Ui::VariationPulsogramVisualWidget)
{
    ui->setupUi(this);

    restoreSplitterPosition();
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::channelChanged,
            this, &VariationPulsogramVisualWidget::on_channelChanged);
}

VariationPulsogramVisualWidget::~VariationPulsogramVisualWidget()
{
    if (m_factors)
        delete m_factors;
    delete ui;
}

bool VariationPulsogramVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctRitmogram;
}

void VariationPulsogramVisualWidget::calculate()
{
    if (!m_factors)
        m_factors = new PulseFactors(testUid(), probeUid(), channelId());
    showVPG();
    showFactors();
}

void VariationPulsogramVisualWidget::on_channelChanged(const QString &probeUid, const QString &channelId)
{
    if (m_factors)
        delete m_factors;
    m_factors = new PulseFactors(testUid(), probeUid, channelId);
    showVPG();
    showFactors();
}

void VariationPulsogramVisualWidget::on_splitterMoved(int, int)
{
    saveSplitterPosition();
}

void VariationPulsogramVisualWidget::showVPG()
{
    ui->wgtVPG->setKind(DynamicDiagramDefines::KindBar);
    ui->wgtVPG->setVolume(DynamicDiagramDefines::Volume3D);
    ui->wgtVPG->setTitle(tr(""));
    ui->wgtVPG->setBottomText(tr("Период, сек"));

    ui->wgtVPG->setAxisSpaceLeft(30);

    for (int i = 0; i < m_factors->statCountsCount(); ++i)
    {
        auto item = new DiagItem(m_factors->statCounts(i), QString::number(m_factors->statBounds(i), 'f', 2));
        ui->wgtVPG->appendItem(item);
    }

}

void VariationPulsogramVisualWidget::showFactors()
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

        auto* itemShortName = new QStandardItem(fi.shortName());
        itemShortName->setEditable(false);

        auto fvf = m_factors->factorValueFormatted(uid);
        auto* itemValue = new QStandardItem(fvf);
        itemValue->setEditable(false);

        model->appendRow(QList<QStandardItem*>() << itemName << itemShortName << itemValue);
    }

    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << "" << tr("Значение"));
    ui->tvFactors->setModel(model);
    ui->tvFactors->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvFactors->header()->resizeSection(0, 500);
    ui->tvFactors->header()->resizeSection(1, 150);
}

void VariationPulsogramVisualWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("VPGVisualWidget", "HSplitterPosition", ui->splitter->saveState());
}

void VariationPulsogramVisualWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("VPGVisualWidget", "HSplitterPosition").toByteArray();
    ui->splitter->restoreState(val);
}
