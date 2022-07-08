#ifndef BINARYEXPORTFILTER_H
#define BINARYEXPORTFILTER_H

#include <QObject>

#include "exportfilter.h"

/*!
 * \brief Класс фильтра экспорта сигналов в бинарном виде The BinaryExportFilter class
 * Формат данных следующий:
 * [N0|N1|N2|N3] - Количество отсчетов (старший - младший)
 * [C] - Количество подканалов (отведений)
 * [v00_0|v00_1|v00_2|v00_3|v00_4|v00_5|v00_6|v00_7]...[v0C_0|v0C_1|v0C_2|v0C_3|v0C_4|v0C_5|v0C_6|v0C_7] - первый отсчет C каналов
 * [v10_0|v10_1|v10_2|v10_3|v10_4|v10_5|v10_6|v10_7]...[v1C_0|v1C_1|v1C_2|v1C_3|v1C_4|v1C_5|v1C_6|v1C_7]
 * ...
 * [vN0_0|vN0_1|vN0_2|vN0_3|vN0_4|vN0_5|vN0_6|vN0_7]...[vNC_0|vNC_1|vNC_2|vNC_3|vNC_4|vNC_5|vNC_6|vNC_7] - последний N отсчет C каналов
 */
class BinaryExportFilter : public ExportFilter  //IEEE 754
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
     * \brief Записывает данные сигнала в файл
     * \param signal - указатель на сигнал
     * \param fileName - полное имя файла с путем и суффиксом (расширением)
     * \param chan - номер подканала -1 - все в один файл
     */
    void writeToFile(SignalAccess* signal, QString& fileName, const int chan = -1) override;

    /*!
     * \brief Возвращает суффикс (расширение) для файла экспорта сигнала
     */
    QString suffix() const override {return "bin";}

    /*!
     * \brief Возвращает фильтр для диалога выбора файла с суффиксом (расширением)
     */
    QString suffixFilter() const override {return tr("Бинарные файлы") + " *.bin (*.bin)";}
};

#endif // BINARYEXPORTFILTER_H
