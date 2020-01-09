#include "advancedchannels.h"
#include "ui_advancedchannels.h"

#include "driver.h"
#include "testresultdata.h"
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

void AdvancedChannels::assignDriver(Driver *driver, TestResultData *trd)
{
    Q_ASSERT(driver);
    m_driver = driver;

    m_mdlDrvChan.clear();
    auto *itemDrv = new QStandardItem(m_driver->driverName());
    itemDrv->setEditable(false);
    itemDrv->setData(QVariant(), WidgetRole);
    itemDrv->setData("", ChannelIdRole);
    m_mdlDrvChan.appendRow(itemDrv);

    auto chanList = m_driver->getChannels();
    foreach (auto channelId, chanList)
    {
        auto chanName = ChannelsUtils::instance().channelName(channelId);
        auto *item = new QStandardItem(chanName);
        item->setEditable(false);

        auto *wgt = createWidget(m_driver, channelId);
        QVariant var;
        var.setValue(wgt);
        item->setData(var, WidgetRole);
        item->setData(channelId, ChannelIdRole);
        wgt->setVisible(false);
        wgt->setFrequency(driver->frequency(channelId));
        wgt->assignTestResultDataObject(trd);

        itemDrv->appendRow(item);
    }

    ui->tvDrvChannels->setModel(&m_mdlDrvChan);
    ui->tvDrvChannels->expandAll();

    connect(ui->tvDrvChannels->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &AdvancedChannels::on_selectionChanged);
    auto val = SettingsProvider::valueFromRegAppCopy("AdvancedChannelsWidget", "SplitterTreePosition").toByteArray();
    ui->splitter->restoreState(val);

    selectFirstChannel();
}

void AdvancedChannels::getData(DeviceProtocols::DeviceData *data)
{
    for (int id = 0; id < m_mdlDrvChan.rowCount(); ++id)
    {
        QModelIndex idxDrv = m_mdlDrvChan.index(id, 0);
        for (int i = 0; i < m_mdlDrvChan.rowCount(idxDrv); ++i)
        {
            QModelIndex index = m_mdlDrvChan.index(i, 0, idxDrv);
            if (index.data(ChannelIdRole).toString() == data->channelId())
            {
                QVariant var = index.data(WidgetRole);
                SignalWidget* wgt = var.value<SignalWidget*>();
                if (wgt)
                    wgt->getData(data);
            }
        }
    }
}

void AdvancedChannels::newProbe()
{
    for (int id = 0; id < m_mdlDrvChan.rowCount(); ++id)
    {
        QModelIndex idxDrv = m_mdlDrvChan.index(id, 0);
        for (int i = 0; i < m_mdlDrvChan.rowCount(idxDrv); ++i)
        {
            QModelIndex index = m_mdlDrvChan.index(i, 0, idxDrv);
            QVariant var = index.data(WidgetRole);
            SignalWidget* wgt = var.value<SignalWidget*>();
            if (wgt)
                wgt->newProbe();
        }
    }
}

void AdvancedChannels::abortProbe()
{
    for (int id = 0; id < m_mdlDrvChan.rowCount(); ++id)
    {
        QModelIndex idxDrv = m_mdlDrvChan.index(id, 0);
        for (int i = 0; i < m_mdlDrvChan.rowCount(idxDrv); ++i)
        {
            QModelIndex index = m_mdlDrvChan.index(i, 0, idxDrv);
            QVariant var = index.data(WidgetRole);
            SignalWidget* wgt = var.value<SignalWidget*>();
            if (wgt)
                wgt->abortProbe();
        }
    }
}

void AdvancedChannels::record(DeviceProtocols::DeviceData *data)
{
    for (int id = 0; id < m_mdlDrvChan.rowCount(); ++id)
    {
        QModelIndex idxDrv = m_mdlDrvChan.index(id, 0);
        for (int i = 0; i < m_mdlDrvChan.rowCount(idxDrv); ++i)
        {
            QModelIndex index = m_mdlDrvChan.index(i, 0, idxDrv);
            QVariant var = index.data(WidgetRole);
            SignalWidget* wgt = var.value<SignalWidget*>();
            if (wgt)
                wgt->record(data);
        }
    }
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

SignalWidget *AdvancedChannels::createWidget(Driver* driver, const QString channelId)
{
    SignalWidget *retval = nullptr;

    auto channelType = ChannelsUtils::instance().channelType(channelId);
    if (channelType == ChannelsDefines::ctStabilogram)
    {
        retval = new StabilogramWidget(driver, channelId, ui->frWidgets);
        ui->frWidgets->layout()->addWidget(retval);
    }

    return retval;
}

void AdvancedChannels::selectFirstChannel()
{
    for (int i = 0; i < m_mdlDrvChan.rowCount(); ++i)
    {
        auto drvIdx = m_mdlDrvChan.index(0, 0);
        for (int j = 0; j < m_mdlDrvChan.rowCount(drvIdx); ++j)
        {
            auto index = m_mdlDrvChan.index(0, 0, drvIdx);
            ui->tvDrvChannels->selectionModel()->select(index, QItemSelectionModel::Select);
            on_selectIndex(index);
            return;
        }
    }
}
