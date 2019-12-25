#include "advancedchannels.h"
#include "ui_advancedchannels.h"

#include "driver.h"
#include "channelsutils.h"
#include "settingsprovider.h"
#include "stabilogramwidget.h"

#include <QDebug>

AdvancedChannels::AdvancedChannels(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdvancedChannels)
{
    ui->setupUi(this);

}

AdvancedChannels::~AdvancedChannels()
{
    delete ui;
}

void AdvancedChannels::assignDriver(Driver *driver)
{
    Q_ASSERT(driver);
    m_driver = driver;

    m_mdlDrvChan.clear();
    auto *itemDrv = new QStandardItem(m_driver->driverName());
    itemDrv->setEditable(false);
    itemDrv->setData(QVariant(), WidgetRole);
    m_mdlDrvChan.appendRow(itemDrv);

    auto chanList = m_driver->getChannels();
    foreach (auto channelId, chanList)
    {
        auto chanName = ChannelsUtils::instance().channelName(channelId);
        auto *item = new QStandardItem(chanName);
        item->setEditable(false);

        auto *wgt = createWidget(channelId);
        QVariant var;
        var.setValue(wgt);
        item->setData(var, WidgetRole);
        wgt->setVisible(false);

        itemDrv->appendRow(item);
    }

    ui->tvDrvChannels->setModel(&m_mdlDrvChan);
    ui->tvDrvChannels->expandAll();

    connect(ui->tvDrvChannels->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &AdvancedChannels::on_selectionChanged);
    auto val = SettingsProvider::valueFromRegAppCopy("AdvancedChannelsWidget", "SplitterTreePosition").toByteArray();
    ui->splitter->restoreState(val);
}

void AdvancedChannels::on_selectIndex(QModelIndex index)
{
    QObjectList children = ui->frWidgets->children();
    foreach(QObject * child, children)
        if (child->isWidgetType())
            static_cast<QWidget*>(child)->setVisible(false);

    QVariant var = index.data(WidgetRole);
    SignalWidget* wgt = var.value<SignalWidget*>();
    if (wgt)
        wgt->setVisible(true);
}

void AdvancedChannels::on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    on_selectIndex(ui->tvDrvChannels->selectionModel()->currentIndex());
}

void AdvancedChannels::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    SettingsProvider::setValueToRegAppCopy("AdvancedChannelsWidget", "SplitterTreePosition", ui->splitter->saveState());
}

SignalWidget *AdvancedChannels::createWidget(const QString channelId)
{
    SignalWidget *retval = nullptr;

    auto channelType = ChannelsUtils::instance().channelType(channelId);
    if (channelType == ChannelsDefines::ctStabilogram)
        retval = new StabilogramWidget(channelId, ui->frWidgets);

    return retval;
}
