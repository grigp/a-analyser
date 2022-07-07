#ifndef BINARYEXPORTFILTER_H
#define BINARYEXPORTFILTER_H

#include <QObject>

#include "exportfilter.h"

class BinaryExportFilter : public ExportFilter
{
public:
    BinaryExportFilter(QObject *parent = nullptr);

    /*!
     * \brief Возвращает uid фильтра
     */
    QString uid() override;

    /*!
     * \brief Возвращается название фильтра
     */
    QString name() override;

    void writeToFile(SignalAccess* signal, QString& fileName, const int chan = -1) override;
};

#endif // BINARYEXPORTFILTER_H
