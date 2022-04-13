#include "bilateralvisualwidget.h"
#include "ui_bilateralvisualwidget.h"

#include <QDebug>

#include "dataprovider.h"
#include "channelsdefines.h"
#include "bilateralresultdata.h"


BilateralVisualWidget::BilateralVisualWidget(VisualDescriptor* visual,
                                             const QString& testUid, const QString& probeUid,
                                             QWidget *parent) :
    ProbeVisual(visual, testUid, probeUid, parent),
    ui(new Ui::BilateralVisualWidget)
{
    ui->setupUi(this);
}

BilateralVisualWidget::~BilateralVisualWidget()
{
    delete ui;
}

bool BilateralVisualWidget::isValid()
{
    bool isChanStab = DataProvider::channelExists(probeUid(), ChannelsDefines::chanStab);
    bool isChanStabLeft = DataProvider::channelExists(probeUid(), ChannelsDefines::chanStabLeft);
    bool isChanStabRight = DataProvider::channelExists(probeUid(), ChannelsDefines::chanStabRight);
    bool isChanBilat = DataProvider::channelExists(probeUid(), ChannelsDefines::chanBilat);
    return isChanStab && isChanStabLeft && isChanStabRight && isChanBilat;
}

void BilateralVisualWidget::calculate()
{
    showPlatforms();
}

void BilateralVisualWidget::showPlatforms()
{
    QByteArray baData;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanBilat, baData))
    {
        m_bData = new BilateralResultData(baData);
        for (int i = 0; i < m_bData->platformsCount(); ++i)
        {
            ui->wgtSKG->addPlatform(m_bData->platform(i));
        }
        ui->wgtSKG->setDiap(computeDiap());
    }
}

int BilateralVisualWidget::computeDiap()
{
    auto plate1 = m_bData->platform(0);
    auto plate2 = m_bData->platform(1);
    int xMin = plate1.x();
    if (plate2.x() < plate1.x())
        xMin = plate2.x();
    int xMax = plate1.x() + plate1.width();
    if (plate2.x() + plate2.width() > plate1.x() + plate1.width())
        xMax = plate2.x() + plate2.width();

    int yMax = plate1.y();
    if (plate2.y() > plate1.y())
        yMax = plate2.y();
    int yMin = plate1.y() - plate1.height();
    if (plate2.y() - plate2.height() < plate1.y() - plate1.height())
        yMin = plate2.y() - plate2.height();

    int diap = qMax(abs(xMin), abs(xMax));
    diap = qMax(diap, abs(yMin));
    diap = qMax(diap, abs(yMax));

    return diap;
}
