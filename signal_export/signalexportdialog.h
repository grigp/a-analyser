#ifndef SIGNALEXPORTDIALOG_H
#define SIGNALEXPORTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

#include "signalexportdefines.h"

namespace Ui {
class SignalExportDialog;
}

/*!
 * \brief Класс диалога экспорта сигналов The SignalExportDialog class
 */
class SignalExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignalExportDialog(QWidget *parent = nullptr);
    ~SignalExportDialog() override;

    enum Roles
    {
        FilterUidRole = Qt::UserRole + 0
    };

    /*!
     * \brief Устанавливает виджеты выбора имен файлов в режиме одиночного экспорта
     * \param titles - список заголовков
     */
    void setFileNameSelectors(QStringList &titles);

    /*!
     * \brief Устанавливает виджет выбора имени папки в режиме пакетного экспорта
     */
    void setFolderSelector();

    /*!
     * \brief Возвращает количество виджетов выбора имен файлов
     */
    int fileNameSelectorCount() const;

    /*!
     * \brief Возвращает имена файлов по номеру виджета
     * \param idx - номер виджета
     */
    QString fileName(const int idx) const;

    void addFilter(const QString& uid, const QString& name);

    /*!
     * \brief Возвращает uid выбранного фильтра
     */
    QString filter() const {return m_filterUid;}

    SignalExportDefines::FilesMode filesMode() const;

public slots:
    void accept() override;

private slots:
    void on_selectFilter(QModelIndex index);

    void on_isSeparateChanged(bool);

private:
    Ui::SignalExportDialog *ui;

    void clearFNSelectors();

    QString m_filterUid {""};
};

#endif // SIGNALEXPORTDIALOG_H
