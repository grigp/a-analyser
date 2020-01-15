#ifndef JUMPHEIGHTTESTPARAMSDIALOG_H
#define JUMPHEIGHTTESTPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class JumpHeightTestParamsDialog;
}

/*!
 * \brief Класс диалога редактирования параметров методики анализа высоты серии прыжков JumpHeightTestParamsDialog class
 */
class JumpHeightTestParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit JumpHeightTestParamsDialog(QWidget *parent = 0);
    ~JumpHeightTestParamsDialog();

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
    void on_strategyChanged(int idx);

private:
    Ui::JumpHeightTestParamsDialog *ui;

    void initUi();
};

#endif // JUMPHEIGHTTESTPARAMSDIALOG_H
