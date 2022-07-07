#ifndef TEXTEXPORTFILTER_H
#define TEXTEXPORTFILTER_H

#include <QObject>

#include "exportfilter.h"

class TextExportFilter : public ExportFilter
{
public:
    TextExportFilter(QObject *parent = nullptr);

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

#endif // TEXTEXPORTFILTER_H
