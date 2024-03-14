#ifndef DROPTESTPARAMSDIALOG_H
#define DROPTESTPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class DropTestParamsDialog;
}

/*!
 * \brief Класс диалога редактирования параметров методики дроп теста DropTestParamsDialog class
 */
class DropTestParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DropTestParamsDialog(QWidget *parent = nullptr);
    ~DropTestParamsDialog();

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
    void on_finishKindChanged(int idx);

    void on_ok();
    void on_cancel();
    void on_default();

private:
    Ui::DropTestParamsDialog *ui;

    void initUi();
};

#endif // DROPTESTPARAMSDIALOG_H
