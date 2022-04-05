#ifndef BILATERALPARAMSDIALOG_H
#define BILATERALPARAMSDIALOG_H

#include <QDialog>

namespace Ui {
class BilateralParamsDialog;
}

class BilateralParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BilateralParamsDialog(QWidget *parent = nullptr);
    ~BilateralParamsDialog() override;

public slots:
    int exec() override;

    QPoint getPlate1Pos() const;
    void setPlate1Pos(const QPoint &pos);

    QSize getPlate1Size() const;
    void setPlate1Size(const QSize &size);

    QPoint getPlate2Pos() const;
    void setPlate2Pos(const QPoint &pos);

    QSize getPlate2Size() const;
    void setPlate2Size(const QSize &size);

private slots:
    /*!
     * \brief Перемещение платформы
     * \param plate - номер платформы 0 - первая, 1 - вторая
     * \param x, y - новые координаты в мм
     */
    void on_movePlate(const int plate, const int x, const int y);

private:
    Ui::BilateralParamsDialog *ui;
};

#endif // BILATERALPARAMSDIALOG_H
