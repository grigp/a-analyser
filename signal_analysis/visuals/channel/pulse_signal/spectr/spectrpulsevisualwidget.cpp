#include "spectrpulsevisualwidget.h"
#include "ui_spectrpulsevisualwidget.h"

#include <QStandardItemModel>
#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "pulsespectrfactorsdefines.h"
#include "pulsespectrfactors.h"
#include "settingsprovider.h"

namespace
{
QStringList DiagFactorUids = QStringList()
        << PulseSpectrFactorsDefines::PwULFUid
        << PulseSpectrFactorsDefines::PwVLFUid
        << PulseSpectrFactorsDefines::PwLFUid
        << PulseSpectrFactorsDefines::PwHFUid;

}


SpectrPulseVisualWidget::SpectrPulseVisualWidget(VisualDescriptor* visual,
                                                 const QString& testUid, const QString& probeUid, const QString& channelId,
                                                 QWidget *parent) :
    ChannelVisual (visual, testUid, probeUid, channelId, parent),
    ui(new Ui::SpectrPulseVisualWidget)
{
    ui->setupUi(this);

//    restoreSplitterPosition();
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

    ui->frNoSpectr->setVisible(m_factors->spectrCount() <= 0);
    ui->splVert->setVisible(m_factors->spectrCount() > 0);
    if (m_factors->spectrCount() > 0)
    {
        showSpectr();
        showDiag();
        showFactors();
    }

    restoreSplitterPosition();
}

void SpectrPulseVisualWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void SpectrPulseVisualWidget::on_channelChanged(const QString &probeUid, const QString &channelId)
{
    Q_UNUSED(probeUid);
    Q_UNUSED(channelId);
    calculate();
}

void SpectrPulseVisualWidget::showSpectr()
{
    ui->wgtSpectr->setTitle(tr(""));
    ui->wgtSpectr->clear();
    for (int i = 0; i < m_factors->spectrCount(); ++i)
        ui->wgtSpectr->addValue(m_factors->spectrValue(i));
    ui->wgtSpectr->setFormatData(m_factors->freqRate(), 0.4);

    foreach (auto area, PulseSpectrFactorsDefines::SpectrAreases)
        ui->wgtSpectr->addFreqArea(area.lo, area.hi, area.shortName, area.colorBackground, area.colorText);
    ui->wgtSpectr->addFreqLabel(m_factors->factorValue(PulseSpectrFactorsDefines::Pw60Uid), "60%");
    ui->wgtSpectr->setNameAxisX(tr("F,Гц"));
    ui->wgtSpectr->setNameAxisY(tr("A,мс"));
}


void SpectrPulseVisualWidget::showDiag()
{
    ui->wgtDiag->clear();
    foreach (auto uid, DiagFactorUids)
    {
        auto pw = m_factors->factorValue(uid);
        auto s_pw = m_factors->factorValueFormatted(uid);
        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uid);

        auto item = new DiagItem(pw, fi.shortName() + ": " + s_pw + "," + fi.measure());
        ui->wgtDiag->appendItem(item);
    }

    ui->wgtDiag->setKind(DynamicDiagramDefines::KindBar);
    ui->wgtDiag->setVolume(DynamicDiagramDefines::Volume3D);
    ui->wgtDiag->setTitle(tr("Распределение мощности по зонам спектра"));
    ui->wgtDiag->setAxisSpaceLeft(30);
    ui->wgtDiag->setSizeDivider(2);
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
    SettingsProvider::setValueToRegAppCopy("Geometry/SpectrPulseVisualWidget", "HSplitterPosition", ui->splHor->saveState());
    SettingsProvider::setValueToRegAppCopy("Geometry/SpectrPulseVisualWidget", "VSplitterPosition", ui->splVert->saveState());
}

void SpectrPulseVisualWidget::restoreSplitterPosition()
{
    ui->splVert->setSizes(QList<int>() << geometry().width() / 2 << geometry().width() / 2);
    ui->splHor->setSizes(QList<int>() << geometry().height() / 2 << geometry().height() / 2);

    auto val = SettingsProvider::valueFromRegAppCopy("Geometry/SpectrPulseVisualWidget", "HSplitterPosition").toByteArray();
    ui->splHor->restoreState(val);
    val = SettingsProvider::valueFromRegAppCopy("Geometry/SpectrPulseVisualWidget", "VSplitterPosition").toByteArray();
    ui->splVert->restoreState(val);
}
