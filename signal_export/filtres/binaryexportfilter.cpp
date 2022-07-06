#include "binaryexportfilter.h"

#include "signalaccess.h"

BinaryExportFilter::BinaryExportFilter(QObject *parent)
    : ExportFilter (parent)
{

}

QString BinaryExportFilter::uid()
{
    return "{F9517665-8F04-47FF-B71B-87D0D4183F49}";
}

QString BinaryExportFilter::name()
{
    return tr("Бинарный");
}

void BinaryExportFilter::doExport(SignalAccess *signal, QString &fileName)
{

}

void BinaryExportFilter::doExport(SignalAccess *signal, QStringList &fileNames)
{
    Q_ASSERT(fileNames.size() == signal->subChansCount());
}
