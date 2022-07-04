#ifndef TEXTEXPORTFILTER_H
#define TEXTEXPORTFILTER_H

#include <QObject>

#include "exportfilter.h"

class TextExportFilter : public ExportFilter
{
public:
    TextExportFilter(QObject *parent = nullptr);

    QString uid() override;
    QString name() override;
    void doExport(SignalAccess* signal, QString& fileName) override;
};

#endif // TEXTEXPORTFILTER_H
