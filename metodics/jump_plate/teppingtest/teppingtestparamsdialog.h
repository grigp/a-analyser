#ifndef TEPPINGTESTPARAMSDIALOG_H
#define TEPPINGTESTPARAMSDIALOG_H

#include <QDialog>

#include <QJsonObject>

namespace Ui {
class TeppingTestParamsDialog;
}

/*!
 * \brief Класс диалога редактирования параметров теппинг теста TeppingTestParamsDialog class
 */
class TeppingTestParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TeppingTestParamsDialog(QWidget *parent = nullptr);
    ~TeppingTestParamsDialog();

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
    Ui::TeppingTestParamsDialog *ui;

    void initUi();
};

#endif // TEPPINGTESTPARAMSDIALOG_H
