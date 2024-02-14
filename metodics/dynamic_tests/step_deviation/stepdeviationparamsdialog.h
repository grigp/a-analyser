#ifndef STEPDEVIATIONPARAMSDIALOG_H
#define STEPDEVIATIONPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class StepDeviationParamsDialog;
}

/*!
 * \brief Класс диалога редактирования параметров теста прироста (со ступенчатым отклонением) StepDeviationParamsDialog class
 */
class StepDeviationParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StepDeviationParamsDialog(QWidget *parent = nullptr);
    ~StepDeviationParamsDialog() override;

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
    Ui::StepDeviationParamsDialog *ui;
};

#endif // STEPDEVIATIONPARAMSDIALOG_H
