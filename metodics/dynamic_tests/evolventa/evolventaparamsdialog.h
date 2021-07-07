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

private:
    Ui::EvolventaParamsDialog *ui;
};

#endif // EVOLVENTAPARAMSDIALOG_H
