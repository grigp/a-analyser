#include "ritmogramvisualwidget.h"
#include "ui_ritmogramvisualwidget.h"

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "pulsefactors.h"

RitmogramVisualWidget::RitmogramVisualWidget(VisualDescriptor* visual,
                                             const QString& testUid, const QString& probeUid, const QString& channelId,
                                             QWidget *parent) :
    ChannelVisual (visual, testUid, probeUid, channelId, parent),
    ui(new Ui::RitmogramVisualWidget)
{
    ui->setupUi(this);
}

RitmogramVisualWidget::~RitmogramVisualWidget()
{
    if (m_factors)
        delete m_factors;
    delete ui;
}

bool RitmogramVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctRitmogram;
}

void RitmogramVisualWidget::calculate()
{


    if (!m_factors)
        m_factors = new PulseFactors(testUid(), probeUid(), channelId());

    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(PulseFactorsDefines::PulseUid);
    ui->lblPulse->setText(tr("Средний пульс") + " " +
                          m_factors->factorValueFormatted(PulseFactorsDefines::PulseUid) + " " +
                          fi.measure());

}
