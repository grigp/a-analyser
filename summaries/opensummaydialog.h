#ifndef OPENSUMMAYDIALOG_H
#define OPENSUMMAYDIALOG_H

#include <QDialog>

namespace Ui {
class OpenSummayDialog;
}

/*!
 * \brief Класс диалога открытия сводки The OpenSummayDialog class
 */
class OpenSummayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenSummayDialog(QWidget *parent = nullptr);
    ~OpenSummayDialog();

    /*!
     * \brief Роли в модели списка сводок The OpenSummaryRoles enum
     */
    enum OpenSummaryRoles
    {
          MethodicRole = Qt::UserRole + 1
        , KindRole
    };

private:
    Ui::OpenSummayDialog *ui;

    void load();
};

#endif // OPENSUMMAYDIALOG_H
