#ifndef JUMPTESTPARAMSDIALOG_H
#define JUMPTESTPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class JumpTestParamsDialog;
}

class JumpTestParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit JumpTestParamsDialog(QWidget *parent = nullptr);
    ~JumpTestParamsDialog();

    /*!
     * \brief Получает параметры для редактирования
     * \param params - параметры
     */
    void setParams(const QJsonObject &params);

    /*!
     * \brief Возвращает отредактированные параметры
     */
    QJsonObject getParams();

private slots:
    void on_ok();
    void on_cancel();
    void on_default();

private:
    Ui::JumpTestParamsDialog *ui;

    void initUi();
};

#endif // JUMPTESTPARAMSDIALOG_H
