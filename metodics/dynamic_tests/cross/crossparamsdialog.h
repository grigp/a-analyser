#ifndef CROSSPARAMSDIALOG_H
#define CROSSPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class CrossParamsDialog;
}

class CrossParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CrossParamsDialog(QWidget *parent = nullptr);
    ~CrossParamsDialog();

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
    void onChangeStateMode(int index);

    void on_ok();
    void on_cancel();
    void on_default();

private:
    Ui::CrossParamsDialog *ui;
};

#endif // CROSSPARAMSDIALOG_H
