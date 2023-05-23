#include "ritmogramvisualwidget.h"
#include "ui_ritmogramvisualwidget.h"

#include <QMessageBox>
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

    QCursor cursorGraph = QCursor(QPixmap(":/images/ValueCursor.png"));
    ui->wgtRitmogram->setCursor(cursorGraph);

    connect(ui->wgtRitmogram, &AreaGraph::press, this, &RitmogramVisualWidget::on_press);
    connect(ui->wgtRitmogram, &AreaGraph::move, this, &RitmogramVisualWidget::on_move);
    connect(ui->wgtRitmogram, &AreaGraph::mouseRelease, this, &RitmogramVisualWidget::on_release);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::channelChanged,
            this, &RitmogramVisualWidget::on_channelChanged);
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
        m_abLower = -1;
        m_abUpper = -1;
    }
}

void RitmogramVisualWidget::on_release(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    if (buttons == Qt::LeftButton)
        m_selBeg = -1;

    double begin;
    double end;
    ui->wgtRitmogram->selectedAreaValue(begin, end);
    if (begin < end)
    {
        m_abLower = begin;
        m_abUpper = end;
    }
    else
    if (end < begin)
    {
        m_abLower = end;
        m_abUpper = begin;
    }
}

void RitmogramVisualWidget::on_move(const int x, const int y, const Qt::MouseButtons buttons)
{
    Q_UNUSED(x);
    if (buttons == Qt::LeftButton)
    {
        if (m_selBeg > -1)
            ui->wgtRitmogram->selectAreaValue(m_selBeg, y);
    }

    int area = -1;
    auto v = ui->wgtRitmogram->currentValue(area);
    ui->edValue->setText(QString::number(v, 'f', 1));
}

void RitmogramVisualWidget::on_deleteArtifacts()
{
    if ((m_abLower > -1) && (m_abUpper > -1) && (m_abLower < m_abUpper))
    {
        auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Удалить артефакты, выпадающие за пределы выделенной области?"));
        if (mr == QMessageBox::Yes)
                deleteArtifacts(m_abLower, m_abUpper);
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не заданы границы зоны отсечения артефактов"));
}

void RitmogramVisualWidget::on_rewriteSignal()
{
    auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Перезаписать ритмограмму?"));
    if (mr == QMessageBox::Yes)
    {
        if (m_signal)
        {
            QByteArray data;
            m_signal->toByteArray(data);
            DataProvider::setChannel(probeUid(), channelId(), data);
        }
    }
}

void RitmogramVisualWidget::on_channelChanged(const QString &probeUid, const QString &channelId)
{
    if (m_factors)
        delete m_factors;
    m_factors = new PulseFactors(testUid(), probeUid, channelId);

    showGraph();
    showResume();
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

void RitmogramVisualWidget::deleteArtifacts(const double lower, const double upper)
{
    if (m_signal)
    {
//        qDebug() << "{ " << lower << " - " << upper << " }";
        for (int i = m_signal->size() - 1; i >= 0; --i)
        {
            double val = m_signal->value(0, i);
            if (val < lower || val > upper)
            {
                m_signal->removeValue(i);
//                qDebug() << "-x-" << i << val;
            }
        }
        ui->wgtRitmogram->update();
    }
}
