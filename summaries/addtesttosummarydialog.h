#ifndef ADDTESTTOSUMMARYDIALOG_H
#define ADDTESTTOSUMMARYDIALOG_H

#include <QDialog>

#include "summarydefines.h"

namespace Ui {
class AddTestToSummaryDialog;
}

/*!
 * \brief Класс диалога добавления показателей теста в сводку The AddTestToSummaryDialog class
 */
class AddTestToSummaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTestToSummaryDialog(QWidget *parent = nullptr);
    ~AddTestToSummaryDialog();

    SummaryDefines::AddTestMode mode() const;
    void setMode(const SummaryDefines::AddTestMode mode);

    QString summary() const;
    void setSummary(const QString &fn);

    SummaryDefines::Kind kind() const;
    void setKind(const SummaryDefines::Kind kind);

    QString summaryName() const;
    void setSummaryName(const QString& name);

private:
    Ui::AddTestToSummaryDialog *ui;
};

#endif // ADDTESTTOSUMMARYDIALOG_H
