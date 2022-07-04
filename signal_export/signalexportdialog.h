#ifndef SIGNALEXPORTDIALOG_H
#define SIGNALEXPORTDIALOG_H

#include <QDialog>

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
    ~SignalExportDialog();

    enum Roles
    {
        FilterUidRole = Qt::UserRole + 0
    };

    void setFileNameCount(const int count);
    int fileNameCount() const;
    QString fileName(const int idx) const;

    void addFilter(const QString& uid, const QString& name);

private:
    Ui::SignalExportDialog *ui;
};

#endif // SIGNALEXPORTDIALOG_H
