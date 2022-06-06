#ifndef STRINGINPUTDIALOG_H
#define STRINGINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class StringInputDialog;
}

class StringInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StringInputDialog(QWidget *parent = nullptr);
    ~StringInputDialog();

    void setTitle(const QString& title);

    void setName(const QString& title);

    QString text() const;
    void setText(const QString& text);

private:
    Ui::StringInputDialog *ui;
};

#endif // STRINGINPUTDIALOG_H
