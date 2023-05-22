#include "ritmogramvisualwidget.h"
#include "ui_ritmogramvisualwidget.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "pulsefactors.h"
#include "dataprovider.h"
#include "ritmogram.h"
#include "graphpainter.h"

RitmogramVisualWidget::RitmogramVisualWidget(VisualDescriptor* visual,
                                             const QString& testUid, const QString& probeUid, const QString& channelId,
                                             QWidget *parent) :
    ChannelVisual (visual, testUid, probeUid, channelId, parent),
    ui(new Ui::RitmogramVisualWidget)
{
    ui->setupUi(this);
    ui->wgtRitmogram->setIsShowCursorValue(true);
    connect(ui->wgtRitmogram, &AreaGraph::press, this, &RitmogramVisualWidget::on_press);
    connect(ui->wgtRitmogram, &AreaGraph::move, this, &RitmogramVisualWidget::on_move);
    connect(ui->wgtRitmogram, &AreaGraph::mouseRelease, this, &RitmogramVisualWidget::on_release);
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

    showGraph();
    showResume();
}

void RitmogramVisualWidget::on_press(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(x);
    if (buttons == Qt::LeftButton)
    {
        ui->wgtRitmogram->clearSelectAreaValue();
        m_selBeg = y;
    }
}

void RitmogramVisualWidget::on_release(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    if (buttons == Qt::LeftButton)
    {
        m_selBeg = -1;
    }
    double begin;
    double end;
    ui->wgtRitmogram->selectedAreaValue(begin, end);
    qDebug() << Q_FUNC_INFO << begin << end;
}

void RitmogramVisualWidget::on_move(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(x);
    if (buttons == Qt::LeftButton)
    {
        if (m_selBeg > -1)
            ui->wgtRitmogram->selectAreaValue(m_selBeg, y);
    }
}

void RitmogramVisualWidget::showGraph()
{
    QByteArray data;
    if (DataProvider::getChannel(probeUid(), channelId(), data))
    {
        if (!m_signal)
        {
            //! Назначить сигнал
            m_signal = new Ritmogram(data);

            if (m_signal->size() > 0)
            {
                ui->wgtRitmogram->appendSignal(m_signal, tr("уд/мин"));
                ui->wgtRitmogram->setHorizontalLabelsKind(GraphPainter::hlkNumbers);

                //! Диапазон сигнала
                auto absMax = m_signal->absMaxValue();
                ui->wgtRitmogram->setDiapazone(0, absMax * 1.2);
            }
        }
    }
}

void RitmogramVisualWidget::showResume()
{
    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(PulseFactorsDefines::PulseUid);
    ui->lblPulse->setText(tr("Средний пульс") + " " +
                          m_factors->factorValueFormatted(PulseFactorsDefines::PulseUid) + " " +
                          fi.measure());

    auto oer = m_factors->overallEffectReg();
    auto style = getStyleByValue(static_cast<int>(oer));
    ui->lblOverallEffectReg->setText(tr("Суммарный эффект регуляции") + " - " + PulseFactorsDefines::OverallEffectRegName.value(oer));
    ui->lblOverallEffectReg->setStyleSheet(style);

    auto af = m_factors->automaticFunction();
    style = getStyleByValue(static_cast<int>(af));
    ui->lblAutomaticFunction->setText(tr("Функция автоматизма") + " - " + PulseFactorsDefines::AutomaticFunctionName.value(af));
    ui->lblAutomaticFunction->setStyleSheet(style);

    auto vh = m_factors->vegetativeHomeostasis();
    style = getStyleByValue(static_cast<int>(vh));
    ui->lblVegetativeHomeostasis->setText(tr("Вегетативный гомеостаз") + " - " + PulseFactorsDefines::VegetativeHomeostasisName.value(vh));
    ui->lblVegetativeHomeostasis->setStyleSheet(style);

    auto sr = m_factors->stabilityRegulation();
    style = getStyleByValue(static_cast<int>(sr));
    ui->lblStabilityRegulation->setText(tr("Устойчивость регуляции") + " - " + PulseFactorsDefines::StabilityRegulationName.value(sr));
    ui->lblStabilityRegulation->setStyleSheet(style);
}

QString RitmogramVisualWidget::getStyleByValue(const int value) const
{
    if (abs(value) == 0)
        return QString("font-size: 12pt; color: rgb(0, 150, 0);");
    else
    if (abs(value) == 1)
        return QString("font-size: 12pt; color: rgb(150, 150, 0);");
    else
    if (abs(value) == 2)
        return QString("font-size: 12pt; color: rgb(150, 0, 0);");
    else
        return QString("font-size: 12pt; color: rgb(50, 50, 50);");
}
