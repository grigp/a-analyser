#include "textexportfilter.h"

#include "signalaccess.h"

TextExportFilter::TextExportFilter(QObject *parent)
    : ExportFilter (parent)
{

}

QString TextExportFilter::uid()
{
    return "{F3E1311F-B23C-4E8E-88E6-2BCACD38523F}";
}

QString TextExportFilter::name()
{
    return tr("Текст");
}

void TextExportFilter::doExport(SignalAccess *signal, QString &fileName)
{

}

void TextExportFilter::doExport(SignalAccess *signal, QStringList &fileNames)
{
    Q_ASSERT(fileNames.size() == signal->subChansCount());

}
