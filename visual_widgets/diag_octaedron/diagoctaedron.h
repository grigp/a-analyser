#ifndef DIAGOCTAEDRON_H
#define DIAGOCTAEDRON_H

#include <QWidget>

#include "octaedronpainter.h"

namespace Ui {
class DiagOctaedron;
}

/*!
 * \brief Класс диаграммы результатов тренажера "Октаэдр" DiagOctaedron class
 */
class DiagOctaedron : public QWidget, public OctaedronPainter
{
    Q_OBJECT

    Q_PROPERTY(QColor line_color READ lineColor WRITE setLineColor DESIGNABLE true)
    Q_PROPERTY(QColor label_color READ labelColor WRITE setLabelColor DESIGNABLE true)
    Q_PROPERTY(QColor labelText_color READ labelTextColor WRITE setLabelTextColor DESIGNABLE true)
    Q_PROPERTY(QColor labelFrame_color READ labelFrameColor WRITE setLabelFrameColor DESIGNABLE true)

public:
    explicit DiagOctaedron(QWidget *parent = nullptr);
    ~DiagOctaedron() override;

    void doUpdate() override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::DiagOctaedron *ui;

};

#endif // DIAGOCTAEDRON_H
