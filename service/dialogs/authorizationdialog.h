#ifndef AUTHORIZATIONDIALOG_H
#define AUTHORIZATIONDIALOG_H

#include <QDialog>

namespace Ui {
class AuthorizationDialog;
}

/*!
// * \brief Класс диалога авторизации The AuthorizationDialog class
 */
class AuthorizationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthorizationDialog(QWidget *parent = nullptr);
    ~AuthorizationDialog();

    void setTitle(const QString &title);
    void setInfo(const QString &info);
    void setTextName(const QString &text);
    void setTextPassword(const QString &text);

    void setLogotip(QPixmap &pixmap);

private:
    Ui::AuthorizationDialog *ui;
};

#endif // AUTHORIZATIONDIALOG_H
