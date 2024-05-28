#ifndef MWCOMPETITIVESTICKPARAMSDIALOG_H
#define MWCOMPETITIVESTICKPARAMSDIALOG_H

#include <QDialog>

namespace Ui {
class MWCompetitiveStickParamsDialog;
}

/*!
 * \brief Класс диалога редактирования параметров теста с соревновательной палкой The MWCompetitiveStickParamsDialog class
 */
class MWCompetitiveStickParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MWCompetitiveStickParamsDialog(QWidget *parent = nullptr);
    ~MWCompetitiveStickParamsDialog();

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
    void on_default();

private:
    Ui::MWCompetitiveStickParamsDialog *ui;
};

#endif // MWCOMPETITIVESTICKPARAMSDIALOG_H
