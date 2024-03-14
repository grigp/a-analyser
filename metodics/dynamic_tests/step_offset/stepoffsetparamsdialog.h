#ifndef STEPOFFSETPARAMSDIALOG_H
#define STEPOFFSETPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class StepOffsetParamsDialog;
}

/*!
 * \brief Класс диалога редактирования параметров теста "Ступени" (со ступенчатым воздействием) The StepOffsetParamsDialog class
 */
class StepOffsetParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StepOffsetParamsDialog(QWidget *parent = nullptr);
    ~StepOffsetParamsDialog();

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
    Ui::StepOffsetParamsDialog *ui;
};

#endif // STEPOFFSETPARAMSDIALOG_H
