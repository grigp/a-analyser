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

    /*!
     * \brief Производит экспорт сигналов. Все подканалы в один файл
     * \param signal - указатель на сигнал
     * \param fileName - имя файла
     */
    void doExport(SignalAccess* signal, QString& fileName) override;

    /*!
     * \brief Производит экспорт сигналов. Каждый подканал в свой файл
     * \param signal - указатель на сигнал
     * \param fileNames - имена файлов подканалов.
     *                    Кол-во должно равняться кол-ву подканалов fileNames.size() == signal->subChansCount()
     */
    void doExport(SignalAccess* signal, QStringList& fileNames) override;
};

#endif // BINARYEXPORTFILTER_H
