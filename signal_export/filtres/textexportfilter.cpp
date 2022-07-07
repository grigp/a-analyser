#include "textexportfilter.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

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

void TextExportFilter::writeToFile(SignalAccess *signal, QString &fileName, const int chan)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&file);
        QStringList sl;

        sl.clear();

        for (int i = 0; i < signal->size(); ++i)
        {
            sl.clear();

            if (chan == -1)
                for (int j = 0; j < signal->subChansCount(); ++j)
                    sl << QString::number(signal->value(j, i)); // << "\t";
            else
                sl << QString::number(signal->value(chan, i)); // << "\t";

            ts << sl.join("\t") + "\n";
        }

        file.close();
    }
}
