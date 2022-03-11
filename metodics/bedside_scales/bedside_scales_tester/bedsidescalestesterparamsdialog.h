#ifndef BEDSIDESCALESTESTERPARAMSDIALOG_H
#define BEDSIDESCALESTESTERPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class BedsideScalesTesterParamsDialog;
}

/*!
 * \brief Класс диалога редактирования параметров методики тестирования прикроватных весов The BedsideScalesTesterParamsDialog class
 */
class BedsideScalesTesterParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BedsideScalesTesterParamsDialog(QWidget *parent = nullptr);
    ~BedsideScalesTesterParamsDialog();

    /*!
     * \brief Получает параметры для редактирования
     * \param params - параметры
     */
    void setParams(const QJsonObject &params);

    /*!
     * \brief Возвращает отредактированные параметры
     */
    QJsonObject getParams();

private:
    Ui::BedsideScalesTesterParamsDialog *ui;
};

#endif // BEDSIDESCALESTESTERPARAMSDIALOG_H
