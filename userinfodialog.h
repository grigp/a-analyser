#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include <QDialog>

namespace Ui {
class UserInfoDialog;
}

class UserInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserInfoDialog(QWidget *parent = nullptr);
    ~UserInfoDialog();

    QString fio() const;
    void setFio(const QString& fio);

    QString enterprice() const;
    void setEnterprice(const QString& enterprice);

    QString address() const;
    void setAddress(const QString& address);

    QString sity() const;
    void setSity(const QString& sity);

    QString phones() const;
    void setPhones(const QString& phones);

    QString email() const;
    void setEmail(const QString& email);

    QString website() const;
    void setWebsite(const QString& website);

    QPixmap logotip();
    void setLogotip(QPixmap logotip);

private slots:
    void onLogoBrowseClicked();

private:
    Ui::UserInfoDialog *ui;

};

#endif // USERINFODIALOG_H
