#ifndef STEPOFFSETPARAMSDIALOG_H
#define STEPOFFSETPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class StepOffsetParamsDialog;
}

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

private:
    Ui::StepOffsetParamsDialog *ui;
};

#endif // STEPOFFSETPARAMSDIALOG_H
