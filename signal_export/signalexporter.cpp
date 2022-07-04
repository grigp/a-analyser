#include "signalexporter.h"

#include <QDebug>

#include "textexportfilter.h"
#include "binaryexportfilter.h"
#include "signalexportdialog.h"

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
    dlg->setFileNameCount(2);
    if (dlg->exec() == QDialog::Accepted)
    {
        qDebug() << dlg->fileNameCount();
        for (int i = 0; i < dlg->fileNameCount(); ++i)
        {
            qDebug() << dlg->fileName(i);
        }
    }
    delete dlg;
}
