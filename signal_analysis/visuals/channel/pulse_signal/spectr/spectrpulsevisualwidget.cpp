#include "spectrpulsevisualwidget.h"
#include "ui_spectrpulsevisualwidget.h"

#include <QStandardItemModel>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "pulsespectrfactors.h"
#include "settingsprovider.h"


SpectrPulseVisualWidget::SpectrPulseVisualWidget(VisualDescriptor* visual,
                                                 const QString& testUid, const QString& probeUid, const QString& channelId,
                                                 QWidget *parent) :
    ChannelVisual (visual, testUid, probeUid, channelId, parent),
    ui(new Ui::SpectrPulseVisualWidget)
{
    ui->setupUi(this);

    restoreSplitterPosition();
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::channelChanged,
            this, &SpectrPulseVisualWidget::on_channelChanged);
}

SpectrPulseVisualWidget::~SpectrPulseVisualWidget()
{
    delete ui;
}

bool SpectrPulseVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctRitmogram;
}

void SpectrPulseVisualWidget::calculate()
{
    if (m_factors)
        delete m_factors;
    m_factors = new PulseSpectrFactors(testUid(), probeUid(), channelId());

    showFactors();
}

void SpectrPulseVisualWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void SpectrPulseVisualWidget::on_channelChanged(const QString &probeUid, const QString &channelId)
{
}

void SpectrPulseVisualWidget::showFactors()
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
    ui->tvFactors->header()->resizeSection(1, 70);
}

void SpectrPulseVisualWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("SpectrPulseVisualWidget", "HSplitterPosition", ui->splHor->saveState());
    SettingsProvider::setValueToRegAppCopy("SpectrPulseVisualWidget", "VSplitterPosition", ui->splVert->saveState());
}

void SpectrPulseVisualWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("SpectrPulseVisualWidget", "HSplitterPosition").toByteArray();
    ui->splHor->restoreState(val);
    val = SettingsProvider::valueFromRegAppCopy("SpectrPulseVisualWidget", "VSplitterPosition").toByteArray();
    ui->splVert->restoreState(val);
}
