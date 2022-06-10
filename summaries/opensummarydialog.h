#ifndef OPENSUMMARYDIALOG_H
#define OPENSUMMARYDIALOG_H

#include <QDialog>
#include <QModelIndex>


namespace Ui {
class OpenSummaryDialog;
}

class ListSummariesModel;
class ListSummariesProxyModel;

/*!
 * \brief Класс диалога открытия сводки The OpenSummaryDialog class
 */
class OpenSummaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenSummaryDialog(QWidget *parent = nullptr);
    ~OpenSummaryDialog();

    /*!
     * \brief Возвращает выбранное имя файла сводки (с путем)
     */
    QString summaryFileName() const;

private slots:
    void on_selectMethodic(QModelIndex index);
    void on_kindChanged();

private:
    Ui::OpenSummaryDialog *ui;

    void load();

    ListSummariesModel* m_mdlSummaries {nullptr};
    ListSummariesProxyModel* m_pmdlSummaries {nullptr};
};

#endif // OPENSUMMARYDIALOG_H
