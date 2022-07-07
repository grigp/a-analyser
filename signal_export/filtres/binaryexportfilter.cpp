#include "binaryexportfilter.h"

#include <QFile>
#include <QDataStream>

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

void BinaryExportFilter::writeToFile(SignalAccess *signal, QString &fileName, const int chan)
{
    QByteArray data;
    data.clear();

    int ss = signal->size();
    quint8 scc = 1;
    if (chan == -1)
        scc = static_cast<quint8>(signal->subChansCount());

    QDataStream ds(&data, QIODevice::WriteOnly);
    ds << ss << scc;

    for (int i = 0; i < signal->size(); ++i)
    {
        if (chan == -1)
            for (int j = 0; j < signal->subChansCount(); ++j)
                ds << signal->value(j, i);
        else
            ds << signal->value(chan, i);
    }

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(data);
        file.close();
    }
}

