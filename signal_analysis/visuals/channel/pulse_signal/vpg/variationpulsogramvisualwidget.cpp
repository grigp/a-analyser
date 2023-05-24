#include "variationpulsogramvisualwidget.h"
#include "ui_variationpulsogramvisualwidget.h"

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "pulsefactors.h"

VariationPulsogramVisualWidget::VariationPulsogramVisualWidget(VisualDescriptor* visual,
                                                               const QString& testUid,
                                                               const QString& probeUid,
                                                               const QString& channelId,
                                                               QWidget *parent) :
    ChannelVisual (visual, testUid, probeUid, channelId, parent),
    ui(new Ui::VariationPulsogramVisualWidget)
{
    ui->setupUi(this);

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

void VariationPulsogramVisualWidget::showVPG()
{

}

void VariationPulsogramVisualWidget::showFactors()
{

}
