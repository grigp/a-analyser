#ifndef TRIANGLEPARAMSDIALOG_H
#define TRIANGLEPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class TriangleParamsDialog;
}

/*!
 * \brief Класс диалога редактирования параметров теста "Треугольник" TriangleParamsDialog class
 */
class TriangleParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TriangleParamsDialog(QWidget *parent = nullptr);
    ~TriangleParamsDialog();

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
    Ui::TriangleParamsDialog *ui;
};

#endif // TRIANGLEPARAMSDIALOG_H
