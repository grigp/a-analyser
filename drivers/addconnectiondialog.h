#ifndef ADDCONNECTIONDIALOG_H
#define ADDCONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class AddConnectionDialog;
}

class AddConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddConnectionDialog(QWidget *parent = 0);
    ~AddConnectionDialog();

    /*!
     * \brief Возвращает uid драйвера
     */
    QString driverUid() const;

    /*!
     * \brief Возвращает название драйвера
     */
    QString driverName() const;

    /*!
     * \brief Возвращает порт
     */
    QString port() const;

    /*!
     * \brief Возвращает комментарий к подключению
     */
    QString comment() const;

private:
    Ui::AddConnectionDialog *ui;

    void fill();
};

#endif // ADDCONNECTIONDIALOG_H
