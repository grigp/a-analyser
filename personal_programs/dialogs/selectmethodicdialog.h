#ifndef SELECTMETHODICDIALOG_H
#define SELECTMETHODICDIALOG_H

#include <QDialog>

namespace Ui {
class SelectMethodicDialog;
}

class SelectMethodicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectMethodicDialog(QWidget *parent = nullptr);
    ~SelectMethodicDialog();

    /*!
     * \brief Возвращает uid выбранной методики
     */
    QString methodic() const;

private slots:
    void on_selectMethod(const QString& uid, const bool isAccept);

private:
    Ui::SelectMethodicDialog *ui;
};

#endif // SELECTMETHODICDIALOG_H
