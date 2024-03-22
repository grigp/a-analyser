#include "skgvisualwidget.h"
#include "ui_skgvisualwidget.h"

#include <QTimer>
#include <QStandardItemModel>
#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "skgwidget.h"
#include "dataprovider.h"
#include "stabilogram.h"
#include "classicfactors.h"
#include "settingsprovider.h"

SKGVisualWidget::SKGVisualWidget(VisualDescriptor* visual,
                                 const QString& testUid, const QString& probeUid, const QString& channelId,
                                 QWidget *parent) :
    ChannelVisual(visual, testUid, probeUid, channelId, parent),
    ui(new Ui::SKGVisualWidget)
{
    ui->setupUi(this);

    restoreSplitterPosition();
}

SKGVisualWidget::~SKGVisualWidget()
{
    delete ui;
}

bool SKGVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctStabilogram;
}

void SKGVisualWidget::calculate()
{
    showSKG();
    showFactors();
}

void SKGVisualWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void SKGVisualWidget::on_btnPlusClicked()
{
    auto diap = ui->wgtSKG->diap();
    if (diap > 1)
        ui->wgtSKG->setDiap(diap / 2);
}

void SKGVisualWidget::on_btnMinusClicked()
{
    auto diap = ui->wgtSKG->diap();
    if (diap < 512)
        ui->wgtSKG->setDiap(diap * 2);
}

void SKGVisualWidget::on_btnZeroingClicked(bool isZeroing)
{
    ui->wgtSKG->setZeroing(isZeroing);
}

void SKGVisualWidget::showSKG()
{
    QByteArray data;
    if (DataProvider::getChannel(probeUid(), channelId(), data))
    {
        if (!m_stab)
        {
            //! Назначить сигнал
            m_stab = new Stabilogram(data);
            ui->wgtSKG->setSignal(m_stab);
        }
    }

    ui->wgtSKG->setVisibleMarker(false);
    if (m_stab && m_stab->size() > 0)
    {
        auto absMax = m_stab->absMaxValue();
        int v = 1;
        while (v < absMax)
            v *= 2;
        QTimer::singleShot(50, [=]()
        {
            ui->wgtSKG->setDiap(v);
        });
    }
}

void SKGVisualWidget::showFactors()
{
    if (m_stab->size() > 0)
    {
        auto *model = new QStandardItemModel();

        if (!m_factors)
            m_factors = new ClassicFactors(testUid(), probeUid(), channelId());

        for (int i = 0; i < m_factors->size(); ++i)
        {
            auto uid = m_factors->factorUid(i);

            auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uid);
            QString fn = fi.name();
            if (fi.measure() != "")
                fn = fn + ", " + fi.measure();
            auto* itemName = new QStandardItem(fn);
            itemName->setEditable(false);

            auto fvf = m_factors->factorValueFormatted(uid);
            auto* itemValue = new QStandardItem(fvf);
            itemValue->setEditable(false);

            model->appendRow(QList<QStandardItem*>() << itemName << itemValue);
        }

        model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Значение"));
        ui->tvFactors->setModel(model);
        ui->tvFactors->header()->resizeSections(QHeaderView::ResizeToContents);
        ui->tvFactors->header()->resizeSection(0, 500);
    }
}

void SKGVisualWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("Geometry/SKGVisualWidget", "SplitterPosition", ui->splitter->saveState());
}

void SKGVisualWidget::restoreSplitterPosition()
{
    ui->splitter->setSizes(QList<int>() << geometry().height() / 20 * 11 << geometry().height() / 20 * 9);

    auto val = SettingsProvider::valueFromRegAppCopy("Geometry/SKGVisualWidget", "HSplitterPosition").toByteArray();
    ui->splitter->restoreState(val);
}

