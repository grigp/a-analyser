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
    virtual void doExport(SignalAccess* signal, QString& fileName) = 0;

    /*!
     * \brief Производит экспорт сигналов. Каждый подканал в свой файл
     * \param signal - указатель на сигнал
     * \param fileNames - имена файлов подканалов.
     *                    Кол-во должно равняться кол-ву подканалов fileNames.size() == signal->subChansCount()
     */
    virtual void doExport(SignalAccess* signal, QStringList& fileNames) = 0;
};

#endif // EXPORTFILTER_H
