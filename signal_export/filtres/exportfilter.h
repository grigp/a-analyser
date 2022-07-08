#ifndef EXPORTFILTER_H
#define EXPORTFILTER_H

#include <QObject>

class SignalAccess;

/*!
 * \brief Суперкласс фильтров экспорта The ExportFilter class
 */
class ExportFilter :public QObject
{
public:
    ExportFilter(QObject *parent = nullptr);
    ~ExportFilter() override;

    /*!
     * \brief Возвращает uid фильтра
     */
    virtual QString uid() = 0;

    /*!
     * \brief Возвращается название фильтра
     */
    virtual QString name() = 0;

    /*!
     * \brief Производит экспорт сигналов. Все подканалы в один файл
     * \param signal - указатель на сигнал
     * \param fileName - имя файла
     */
    void doExport(SignalAccess* signal, QString& fileName);

    /*!
     * \brief Производит экспорт сигналов. Каждый подканал в свой файл
     * \param signal - указатель на сигнал
     * \param fileNames - имена файлов подканалов.
     *                    Кол-во должно равняться кол-ву подканалов fileNames.size() == signal->subChansCount()
     */
    void doExport(SignalAccess* signal, QStringList& fileNames);

    /*!
     * \brief Записывает данные сигнала в файл
     * \param signal - указатель на сигнал
     * \param fileName - полное имя файла с путем и суффиксом (расширением)
     * \param chan - номер подканала -1 - все в один файл
     */
    virtual void writeToFile(SignalAccess* signal, QString& fileName, const int chan = -1) = 0;

    /*!
     * \brief Возвращает суффикс (расширение) для файла экспорта сигнала
     */
    virtual QString suffix() const = 0;

    /*!
     * \brief Возвращает фильтр для диалога выбора файла с суффиксом (расширением)
     */
    virtual QString suffixFilter() const = 0;
};

#endif // EXPORTFILTER_H
