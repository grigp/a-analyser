#ifndef EXPORTFILTER_H
#define EXPORTFILTER_H

#include <QObject>

class SignalAccess;

class ExportFilter :public QObject
{
public:
    ExportFilter(QObject *parent = nullptr);
    ~ExportFilter() override;

    virtual QString uid() = 0;
    virtual QString name() = 0;
    virtual void doExport(SignalAccess* signal, QString& fileName) = 0;
};

#endif // EXPORTFILTER_H
