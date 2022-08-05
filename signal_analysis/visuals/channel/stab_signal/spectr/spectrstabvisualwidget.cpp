#include "spectrstabvisualwidget.h"
#include "ui_spectrstabvisualwidget.h"

#include <QStandardItemModel>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "spectrstabfactors.h"
#include "settingsprovider.h"

SpectrStabVisualWidget::SpectrStabVisualWidget(VisualDescriptor* visual,
                                               const QString& testUid, const QString& probeUid, const QString& channelUid,
                                               QWidget *parent) :
    ChannelVisual(visual, testUid, probeUid, channelUid, parent),
    ui(new Ui::SpectrStabVisualWidget)
{
    ui->setupUi(this);
}

SpectrStabVisualWidget::~SpectrStabVisualWidget()
{
    if (m_factors)
        delete m_factors;
    delete ui;
}

bool SpectrStabVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctStabilogram;
}

void SpectrStabVisualWidget::calculate()
{
    m_factors = new SpectrStabFactors(testUid(), probeUid(), channelUid());
    showTable();
    showSpectrs();
    restoreSplitterPosition();
}

void SpectrStabVisualWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void SpectrStabVisualWidget::showTable()
{
    auto *model = new QStandardItemModel();

    int cnt = m_factors->size() / 2;
    for (int i = 0; i < cnt; ++i)
    {
        auto uidX = m_factors->factorUid(i);
        auto uidY = m_factors->factorUid(cnt + i);

        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uidX);
        QString fn = fi.name();
        if (fi.measure() != "")
            fn = fn + ", " + fi.measure();
        auto* itemName = new QStandardItem(fn);
        itemName->setEditable(false);

        auto fvfX = m_factors->factorValueFormatted(uidX);
        auto* itemValueX = new QStandardItem(fvfX);
        itemValueX->setEditable(false);

        auto fvfY = m_factors->factorValueFormatted(uidY);
        auto* itemValueY = new QStandardItem(fvfY);
        itemValueY->setEditable(false);

        model->appendRow(QList<QStandardItem*>() << itemName << itemValueX << itemValueY);
    }

    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Фронталь") << tr("Сагитталь"));
    ui->tvFactors->setModel(model);
    ui->tvFactors->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvFactors->header()->resizeSection(0, 500);
}

void SpectrStabVisualWidget::showSpectrs()
{
    ui->wgtSpectrX->setTitle(tr("Фронталь X"));
    for (int i = 0; i < m_factors->points(); ++i)
        ui->wgtSpectrX->addValue(m_factors->value(0, i));
    ui->wgtSpectrX->setFormatData(m_factors->frequency(), m_factors->points(), 6);

    ui->wgtSpectrY->setTitle(tr("Сагитталь Y"));
    for (int i = 0; i < m_factors->points(); ++i)
        ui->wgtSpectrY->addValue(m_factors->value(1, i));
    ui->wgtSpectrY->setFormatData(m_factors->frequency(), m_factors->points(), 6);
}

void SpectrStabVisualWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("SpectrStabVisualWidget", "HSplitterPosition", ui->splHorizontal->saveState());
    SettingsProvider::setValueToRegAppCopy("SpectrStabVisualWidget", "VSplitterPosition", ui->splVertical->saveState());
}

void SpectrStabVisualWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("SpectrStabVisualWidget", "HSplitterPosition").toByteArray();
    ui->splHorizontal->restoreState(val);
    val = SettingsProvider::valueFromRegAppCopy("SpectrStabVisualWidget", "VSplitterPosition").toByteArray();
    ui->splVertical->restoreState(val);
}
