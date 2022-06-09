#ifndef OPENSUMMARYDIALOG_H
#define OPENSUMMARYDIALOG_H

#include <QDialog>

namespace Ui {
class OpenSummaryDialog;
}

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
     * \brief Роли в модели списка сводок The OpenSummaryRoles enum
     */
    enum OpenSummaryRoles
    {
          MethodicRole = Qt::UserRole + 1
        , KindRole
    };

private:
    Ui::OpenSummaryDialog *ui;

    void load();
};

#endif // OPENSUMMARYDIALOG_H
