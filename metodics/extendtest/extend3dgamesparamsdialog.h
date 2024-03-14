#ifndef EXTEND3DGAMESPARAMSDIALOG_H
#define EXTEND3DGAMESPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class Extend3DGamesParamsDialog;
}

class Extend3DGamesParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Extend3DGamesParamsDialog(QWidget *parent = nullptr);
    ~Extend3DGamesParamsDialog();

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
    Ui::Extend3DGamesParamsDialog *ui;
};

#endif // EXTEND3DGAMESPARAMSDIALOG_H
