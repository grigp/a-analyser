#ifndef DIAGCROSS_H
#define DIAGCROSS_H

#include <QWidget>

namespace Ui {
class DiagCross;
}

/*!
 * \brief Класс диаграммы теста на устойчивость DiagCross class
 */
class DiagCross : public QWidget
{
    Q_OBJECT

public:
    explicit DiagCross(QWidget *parent = nullptr);
    ~DiagCross() override;

    void setBackgroundColor(const QColor color) {m_backgroundColor = color;}
    QColor backgroundColor() const {return m_backgroundColor;}

    void setFrameColor(const QColor color) {m_frameColor = color;}
    QColor frameColor() const {return m_frameColor;}

    void setFrontalColor(const QColor color) {m_frontalColor = color;}
    QColor frontalColor() const {return m_frontalColor;}

    void setSagittalColor(const QColor color) {m_sagittalColor = color;}
    QColor sagittalColor() const {return m_sagittalColor;}

    void setValueUp(const int value) {m_valueUp = value; repaint();}
    int valueUp() const {return m_valueUp;}

    void setValueDown(const int value) {m_valueDown = value; repaint();}
    int valueDown() const {return m_valueDown;}

    void setValueRight(const int value) {m_valueRight = value; repaint();}
    int valueRight() const {return m_valueRight;}

    void setValueLeft(const int value) {m_valueLeft = value; repaint();}
    int valueLeft() const {return m_valueLeft;}

    void setDiap(const int value) {m_diap = value;}
    int diap() const {return m_diap;}

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::DiagCross *ui;

    QColor m_backgroundColor {Qt::white};
    QColor m_frameColor {Qt::darkGray};
    QColor m_frontalColor {QColor(0, 118, 163)};
    QColor m_sagittalColor {QColor(0, 166, 80)};

    int m_valueUp {0};
    int m_valueDown {0};
    int m_valueRight {0};
    int m_valueLeft {0};
    int m_diap {1};
};

#endif // DIAGCROSS_H
