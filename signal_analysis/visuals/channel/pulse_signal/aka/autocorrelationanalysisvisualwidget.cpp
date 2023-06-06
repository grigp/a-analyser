#include "autocorrelationanalysisvisualwidget.h"
#include "ui_autocorrelationanalysisvisualwidget.h"

#include <QStandardItemModel>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "pulsefactors.h"
#include "anysignal.h"
#include "graphpainter.h"

namespace
{
/*!
 * \brief Показатели автокорреляционного анализа FactorsAKA
 */
static const QStringList FactorsAKA = QStringList() << PulseFactorsDefines::AKACClUid
                                                    << PulseFactorsDefines::AKACC0Uid;

}

AutoCorrelationAnalysisVisualWidget::AutoCorrelationAnalysisVisualWidget(VisualDescriptor* visual,
                                                                         const QString& testUid, const QString& probeUid, const QString& channelId,
                                                                         QWidget *parent) :
    ChannelVisual (visual, testUid, probeUid, channelId, parent),
    ui(new Ui::AutoCorrelationAnalysisVisualWidget)
{
    ui->setupUi(this);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::channelChanged,
            this, &AutoCorrelationAnalysisVisualWidget::on_channelChanged);
}

AutoCorrelationAnalysisVisualWidget::~AutoCorrelationAnalysisVisualWidget()
{
    if (m_factors)
        delete m_factors;
    delete ui;
}

bool AutoCorrelationAnalysisVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctRitmogram;
}

void AutoCorrelationAnalysisVisualWidget::calculate()
{
    if (!m_factors)
        m_factors = new PulseFactors(testUid(), probeUid(), channelId());
    showDiag();
    showFactors();
}

void AutoCorrelationAnalysisVisualWidget::on_channelChanged(const QString &probeUid, const QString &channelId)
{
    if (m_factors)
        delete m_factors;
    m_factors = new PulseFactors(testUid(), probeUid, channelId);
    showDiag();
    showFactors();
}

void AutoCorrelationAnalysisVisualWidget::showDiag()
{
    ui->wgtDiag->clear();

    if (!m_diagData)
        m_diagData = new AnySignal(1, 1);
    else
        m_diagData->clear();

    for (int i = 0; i < PulseFactorsDefines::iAKPointsCount; ++i)
    {
        QVector<double> rec;
        rec << m_factors->correlationCoef(i);
        m_diagData->appendValue(rec);

    }
    ui->wgtDiag->setHorizontalLabelsKind(GraphPainter::hlkNumbers);
    ui->wgtDiag->appendSignal(m_diagData, tr("Сдвиг"));
    ui->wgtDiag->area(0)->setDiapazone(-1, 1);
}

void AutoCorrelationAnalysisVisualWidget::showFactors()
{
    auto *model = new QStandardItemModel();

    foreach (auto fctUid, FactorsAKA)
    {
        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(fctUid);
        QString fn = fi.name();
        if (fi.measure() != "")
            fn = fn + ", " + fi.measure();
        auto* itemName = new QStandardItem(fn);
        itemName->setEditable(false);

        auto* itemShortName = new QStandardItem(fi.shortName());
        itemShortName->setEditable(false);

        auto fvf = m_factors->factorValueFormatted(fctUid);
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

