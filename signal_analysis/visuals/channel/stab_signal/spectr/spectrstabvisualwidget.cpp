#include "spectrstabvisualwidget.h"
#include "ui_spectrstabvisualwidget.h"

#include <QStandardItemModel>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "spectrstabfactors.h"

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
}

void SpectrStabVisualWidget::showTable()
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
