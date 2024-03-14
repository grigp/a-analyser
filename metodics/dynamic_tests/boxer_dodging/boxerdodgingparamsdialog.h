#ifndef BOXERDODGINGPARAMSDIALOG_H
#define BOXERDODGINGPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class BoxerDodgingParamsDialog;
}

/*!
 * \brief Класс диалога параметров тренажера с уклонением для боксеров BoxerDodgingParamsDialog class
 */
class BoxerDodgingParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BoxerDodgingParamsDialog(QWidget *parent = nullptr);
    ~BoxerDodgingParamsDialog();

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
    Ui::BoxerDodgingParamsDialog *ui;
};

#endif // BOXERDODGINGPARAMSDIALOG_H
