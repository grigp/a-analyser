#ifndef DIAGTRIANGLE_H
#define DIAGTRIANGLE_H

#include <QWidget>

namespace Ui {
class DiagTriangle;
}

/*!
 * \brief Класс виджета треугольника по диапазону отклонений DiagTriangle class
 */
class DiagTriangle : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor diag_color READ diagColor WRITE setDiagColor DESIGNABLE true)

public:
    explicit DiagTriangle(QWidget *parent = nullptr);
    ~DiagTriangle() override;

    void setFrameColor(const QColor color) {m_frameColor = color;}
    QColor frameColor() const {return m_frameColor;}

    void setDiagColor(const QColor color) {m_diagColor = color;}

    void setValueUp(const int value) {m_valueUp = value; repaint();}
    int valueUp() const {return m_valueUp;}

    void setValueRightDown(const int value) {m_valueRightDown = value; repaint();}
    int valueRightDown() const {return m_valueRightDown;}

    void setValueLeftDown(const int value) {m_valueLeftDown = value; repaint();}
    int valueLeftDown() const {return m_valueLeftDown;}

    void setDiap(const int value) {m_diap = value;}
    int diap() const {return m_diap;}

public slots:
    QColor diagColor() const {return m_diagColor;}

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::DiagTriangle *ui;

    QColor m_frameColor {Qt::black};
    QColor m_diagColor {Qt::darkCyan};
    int m_valueUp {0};
    int m_valueRightDown {0};
    int m_valueLeftDown {0};
    int m_diap {128};
};

#endif // DIAGTRIANGLE_H
