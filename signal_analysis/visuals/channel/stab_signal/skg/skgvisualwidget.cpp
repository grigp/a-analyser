#include "skgvisualwidget.h"
#include "ui_skgvisualwidget.h"

#include <QTimer>
#include <QStandardItemModel>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "areaskg.h"
#include "dataprovider.h"
#include "stabilogram.h"
#include "classicfactors.h"

SKGVisualWidget::SKGVisualWidget(VisualDescriptor* visual,
                                 const QString& testUid, const QString& probeUid, const QString& channelUid,
                                 QWidget *parent) :
    ChannelVisual(visual, testUid, probeUid, channelUid, parent),
    ui(new Ui::SKGVisualWidget)
{
    ui->setupUi(this);
}

SKGVisualWidget::~SKGVisualWidget()
{
    delete ui;
}

bool SKGVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctStabilogram;
}

void SKGVisualWidget::calculate()
{
    QTimer::singleShot(0, [=]{
        setSizeSKG();
    });

    showSKG();
    showFactors();
}

void SKGVisualWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    setSizeSKG();
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
    if (DataProvider::getChannel(probeUid(), channelUid(), data))
    {
        if (!m_stab)
        {
            //! Назначить сигнал
            m_stab = new Stabilogram(data);
            ui->wgtSKG->setSignal(m_stab);
        }
    }

    ui->wgtSKG->setVisibleMarker(false);
    if (m_stab)
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
    auto *model = new QStandardItemModel();

    if (!m_factors)
        m_factors = new ClassicFactors(testUid(), probeUid(), channelUid());

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

void SKGVisualWidget::setSizeSKG()
{
    auto rect = ui->wgtSKG->geometry();
    rect.setHeight(rect.width());
    rect.setY(ui->frSKG->geometry().height() / 2 - rect.height() / 2);
    ui->wgtSKG->setGeometry(rect);
}
