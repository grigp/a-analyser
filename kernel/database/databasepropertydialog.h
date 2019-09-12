#ifndef DATABASEPROPERTYDIALOG_H
#define DATABASEPROPERTYDIALOG_H

#include <QDialog>

namespace Ui {
class DataBasePropertyDialog;
}

class DataBasePropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataBasePropertyDialog(QWidget *parent = 0);
    ~DataBasePropertyDialog();

    /*!
     * \brief Устанавливает путь к БД
     */
    void setPath(const QString &path);

    /*!
     * \brief Доступ к полю комментария
     */
    QString comment() const;
    void setComment(const QString &comment);

private:
    Ui::DataBasePropertyDialog *ui;
};

#endif // DATABASEPROPERTYDIALOG_H
