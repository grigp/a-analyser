#ifndef WINDROSEFACTORSDIAG_H
#define WINDROSEFACTORSDIAG_H

#include <QWidget>

namespace Ui {
class WindRoseFactorsDiag;
}

/*!
 * \brief Класс виджета диаграммы представления показателей в виде розы ветров The WindRoseFactorsDiag class
 */
class WindRoseFactorsDiag : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor line_color MEMBER m_lineColor DESIGNABLE true)
    Q_PROPERTY(QColor axis_color MEMBER m_axisColor DESIGNABLE true)

public:
    explicit WindRoseFactorsDiag(QWidget *parent = nullptr);
    ~WindRoseFactorsDiag() override;

    QColor lineColor() const {return m_lineColor;}
    void setLineColor(const QColor color) {m_lineColor = color;}

    QColor axisColor() const {return m_axisColor;}
    void setAxisColor(const QColor color) {m_axisColor = color;}

    double min() const {return m_min;}
    void setMin(const double min) {m_min = min;}

    double max() const {return m_max;}
    void setMax(const double max) {m_max = max;}

    /*!
     * \brief Добавляет значение показателя
     * \param value - значение показателя
     */
    void addFactor(const double value);

    /*!
     * \brief Очищает список показателей
     */
    void clear();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::WindRoseFactorsDiag *ui;

    QColor m_lineColor {Qt::blue};
    QColor m_axisColor {Qt::black};

    QVector<double> m_factors;
    double m_min {0};
    double m_max {100};
};

#endif // WINDROSEFACTORSDIAG_H
