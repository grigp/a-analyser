#ifndef SETMAXFORCEDIALOG_H
#define SETMAXFORCEDIALOG_H

#include <QDialog>

#include "baseutils.h"

namespace Ui {
class SetMaxForceDialog;
}

class SetMaxForceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetMaxForceDialog(QWidget *parent = nullptr);
    ~SetMaxForceDialog() override;

    /*!
     * \brief Устанавливает направление, в котором будут проводиться измерения
     * \param direction - код направления
     */
    void setDirection(const BaseUtils::Directions direction);

    /*!
     * \brief получение и отображение данных
     * \param x, y - координаты ЦД
     */
    void getData(const double x, const double y);

    /*!
     * \brief Возвращает значение максимального отклонения
     */
    int value() const;

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void resetValue();

private:
    Ui::SetMaxForceDialog *ui;

    BaseUtils::Directions m_direction;
};

#endif // SETMAXFORCEDIALOG_H
