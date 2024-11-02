#include "bilateralvisualwidget.h"
#include "ui_bilateralvisualwidget.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "channelsdefines.h"
#include "bilateralresultdata.h"
#include "channelsutils.h"
#include "stabilogram.h"
#include "balistogram.h"
#include "skgwidget.h"


BilateralVisualWidget::BilateralVisualWidget(VisualDescriptor* visual,
                                             const QString& testUid, const QString& probeUid,
                                             QWidget *parent) :
    ProbeVisual(visual, testUid, probeUid, parent),
    ui(new Ui::BilateralVisualWidget)
{
    ui->setupUi(this);

    ui->lblMassa->setStyleSheet("font-size: 14pt;");
    ui->btnZeroingNo->setVisible(false);
    ui->btnZeroingDif->setVisible(false);
    ui->btnZeroingCommon->setVisible(false);
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
    showSKG();
    showDiagram();
}

void BilateralVisualWidget::on_btnPlusClick()
{
    auto diap = ui->wgtSKG->diap();
    if (diap > 1)
        ui->wgtSKG->setDiap(diap / 2);
}

void BilateralVisualWidget::on_btnMinusClick()
{
    auto diap = ui->wgtSKG->diap();
    if (diap < m_diap)
        ui->wgtSKG->setDiap(diap * 2);
}

void BilateralVisualWidget::on_cbSKGCommonChecked(bool checked)
{
    ui->wgtSKG->setVisibleSKG(checked);
}

void BilateralVisualWidget::on_cbSKGLeftChecked(bool checked)
{
    ui->wgtSKG->setVisibleSKG(checked, 1);
}

void BilateralVisualWidget::on_cbSKGRightChecked(bool checked)
{
    ui->wgtSKG->setVisibleSKG(checked, 2);
}

void BilateralVisualWidget::on_btnZeroingNoClicked()
{
    ui->wgtSKG->setZeroing(false);
}

void BilateralVisualWidget::on_btnZeroingDifClicked()
{

}

void BilateralVisualWidget::on_btnZeroingCommonClicked()
{
    ui->wgtSKG->setZeroing(true);
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
        m_diap = computeDiap();
        ui->wgtSKG->setDiap(m_diap);
    }
}

void BilateralVisualWidget::showSKG()
{
    QByteArray data;
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStab, data))
    {
        if (!m_stab)
        {
            //! Назначить сигнал
            m_stab = new Stabilogram(data);
            ui->wgtSKG->setSignal(m_stab);
            ui->wgtSKG->setZeroing(true);
            ui->wgtSKG->setVisibleMarker(false);
            ui->wgtSKG->setOffset(0, 0, 0);
        }
    }

    data.clear();
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStabLeft, data))
    {
        if (!m_stabLeft)
        {
            //! Назначить сигнал
            m_stabLeft = new Stabilogram(data);
            ui->wgtSKG->setSignal(m_stabLeft, 1);
            ui->wgtSKG->setOffset(-m_bData->platform(0).center().x(),
                                  -m_bData->platform(0).y() + m_bData->platform(0).height() / 2, 1);
        }
    }

    data.clear();
    if (DataProvider::getChannel(probeUid(), ChannelsDefines::chanStabRight, data))
    {
        if (!m_stabRight)
        {
            //! Назначить сигнал
            m_stabRight = new Stabilogram(data);
            ui->wgtSKG->setSignal(m_stabRight, 2);
            ui->wgtSKG->setOffset(-m_bData->platform(1).center().x(),
                                  -m_bData->platform(1).y() + m_bData->platform(1).height() / 2, 2);
        }
    }
}

void BilateralVisualWidget::showDiagram()
{
    double left = 0;
    double right = 0;

    QByteArray data;
    if (DataProvider::getChannel(probeUid(), ChannelsUtils::instance().zChanForStabChan(ChannelsDefines::chanStab), data))
    {
        auto z = new Balistogram(data);
        ui->lblMassa->setText(tr("Масса") + " : " + QString::number(z->maxValue()) + " " + tr("кг"));
    }

    data.clear();
    if (DataProvider::getChannel(probeUid(), ChannelsUtils::instance().zChanForStabChan(ChannelsDefines::chanStabLeft), data))
    {
        auto z = new Balistogram(data);
        left = z->maxValue();
        ui->lblLeft->setText(QString::number(left));
    }

    data.clear();
    if (DataProvider::getChannel(probeUid(), ChannelsUtils::instance().zChanForStabChan(ChannelsDefines::chanStabRight), data))
    {
        auto z = new Balistogram(data);
        right = z->maxValue();
        ui->lblRight->setText(QString::number(right));
    }

    ui->wgtDiag->setKind(DynamicDiagramDefines::KindBar);
    ui->wgtDiag->setVolume(DynamicDiagramDefines::Volume3D);
    ui->wgtDiag->setAxisSpaceLeft(15);
    ui->wgtDiag->setAxisSpaceBottom(5);
    ui->wgtDiag->appendItem(new DiagItem(left, ""));
    ui->wgtDiag->appendItem(new DiagItem(right, ""));
    ui->wgtDiag->setDiap(0, qMax(left, right));
}
