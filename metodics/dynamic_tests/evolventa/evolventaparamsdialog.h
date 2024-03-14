#ifndef EVOLVENTAPARAMSDIALOG_H
#define EVOLVENTAPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class EvolventaParamsDialog;
}

class EvolventaParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EvolventaParamsDialog(QWidget *parent = nullptr);
    ~EvolventaParamsDialog();

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
    Ui::EvolventaParamsDialog *ui;
};

#endif // EVOLVENTAPARAMSDIALOG_H
