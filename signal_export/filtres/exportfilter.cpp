#include "exportfilter.h"

#include "signalaccess.h"

ExportFilter::ExportFilter(QObject *parent)
    : QObject (parent)
{

}

ExportFilter::~ExportFilter()
{

}

void ExportFilter::doExport(SignalAccess *signal, QString &fileName)
{
    writeToFile(signal, fileName);
}

void ExportFilter::doExport(SignalAccess *signal, QStringList &fileNames)
{
    Q_ASSERT(fileNames.size() == signal->subChansCount());
    for (int i = 0; i < fileNames.size(); ++i)
    {
        auto s = fileNames.at(i);
        writeToFile(signal, s, i);
    }
}
