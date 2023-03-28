#ifndef DIAGCROSS_H
#define DIAGCROSS_H

#include <QWidget>
//#include <QComboBox>

#include "crosspainter.h"


namespace Ui {
class DiagCross;
}

/*!
 * \brief Класс диаграммы теста на устойчивость DiagCross class
 */
class DiagCross : public QWidget, public CrossPainter
{
    Q_OBJECT

    Q_PROPERTY(QColor frontal_color READ frontalColor WRITE setFrontalColor DESIGNABLE true)
    Q_PROPERTY(QColor sagittal_color READ sagittalColor WRITE setSagittalColor DESIGNABLE true)

public:
    explicit DiagCross(QWidget *parent = nullptr);
    ~DiagCross() override;

protected:
    void paintEvent(QPaintEvent *event) override;

    void doUpdate() override;

private:
    Ui::DiagCross *ui;

};

#endif // DIAGCROSS_H
