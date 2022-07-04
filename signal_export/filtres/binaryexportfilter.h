#ifndef BINARYEXPORTFILTER_H
#define BINARYEXPORTFILTER_H

#include <QObject>

#include "exportfilter.h"

class BinaryExportFilter : public ExportFilter
{
public:
    BinaryExportFilter(QObject *parent = nullptr);

    QString uid() override;
    QString name() override;
    void doExport(SignalAccess* signal, QString& fileName) override;
};

#endif // BINARYEXPORTFILTER_H
