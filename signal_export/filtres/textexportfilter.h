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
    QString suffix() const override {return "txt";}

    /*!
     * \brief Возвращает фильтр для диалога выбора файла с суффиксом (расширением)
     */
    QString suffixFilter() const override {return tr("Текстовые файлы") + " *.txt (*.txt)";}
    //
};

#endif // TEXTEXPORTFILTER_H
