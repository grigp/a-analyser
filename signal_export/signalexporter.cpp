#include "signalexporter.h"

#include <QDebug>

#include "textexportfilter.h"
#include "binaryexportfilter.h"
#include "signalexportdialog.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsutils.h"
#include "stabilogram.h"
#include "balistogram.h"
#include "myogram.h"
#include "breathsignal.h"
#include "dynamosignal.h"
#include "ritmogram.h"

SignalExporter::SignalExporter(const QString &probeUid, const QString &channelId)
{
    m_probeUid = probeUid;
    m_channelId = channelId;
    m_mode = mdOnce;
    getFilters();
    doExport();
}

SignalExporter::SignalExporter(const QStringList &testUids)
{
    m_testUids = testUids;
    m_mode = mdBatch;
    getFilters();
    doExport();
}

SignalExporter::~SignalExporter()
{
    foreach (auto filter, m_filters)
        delete filter;
    m_filters.clear();
}

void SignalExporter::getFilters()
{
    m_filters.clear();
    m_filters << new TextExportFilter();
    m_filters << new BinaryExportFilter();
}

void SignalExporter::doExport()
{
    auto dlg = new SignalExportDialog(nullptr);
    foreach (auto filter, m_filters)
        dlg->addFilter(filter->uid(), filter->name());
    if (m_mode == mdOnce)
    {
        m_signal = createSignal(m_probeUid, m_channelId);
        if (m_signal)
        {
            QStringList titles;
            for (int i = 0; i < m_signal->subChansCount(); ++i)
                titles << m_signal->subChanName(i);
            dlg->setFileNameSelectors(titles);
        }
    }
    else
    if (m_mode == mdBatch)
        dlg->setFolderSelector();

    if (dlg->exec() == QDialog::Accepted)
    {
        qDebug() << dlg->fileNameSelectorCount();
        for (int i = 0; i < dlg->fileNameSelectorCount(); ++i)
        {
            qDebug() << dlg->fileName(i);
        }
    }
    if (m_signal)
        delete m_signal;
    delete dlg;
}

SignalAccess *SignalExporter::createSignal(const QString &probeUid, const QString &channelId) const
{
    QByteArray data;
    if (DataProvider::getChannel(probeUid, channelId, data))
    {
        if (ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctStabilogram)
            return new Stabilogram(data);
        else
        if (ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctBalistogram)
            return new Balistogram(data);
        else
        if (ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctMyogram)
            return new Myogram(data);
        else
        if (ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctDynamo)
            return new DynamoSignal(data);
        else
        if (ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctBalistogram)
            return new BreathSignal(data);
        else
        if (ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctRitmogram)
            return new Ritmogram(data);
    }
    return nullptr;
}
