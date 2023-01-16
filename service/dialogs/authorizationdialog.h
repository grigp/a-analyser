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

    QString name() const;
    void setName(const QString& name);

    QString password() const;
    void setPassword(const QString& password);

private slots:
    void on_showPassword(bool visible);

private:
    Ui::AuthorizationDialog *ui;
};

#endif // AUTHORIZATIONDIALOG_H
