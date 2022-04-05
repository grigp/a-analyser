#ifndef PLATESPOSITIONING_H
#define PLATESPOSITIONING_H

#include <QWidget>

namespace Ui {
class PlatesPositioning;
}

/*!
 * \brief Класс виджета позиционирования платформ The PlatesPositioning class
 */
class PlatesPositioning : public QWidget
{
    Q_OBJECT

public:
    explicit PlatesPositioning(QWidget *parent = nullptr);
    ~PlatesPositioning() override;

    void setBackgroundColor(const QColor color);
    QColor backgroundColor() const;

    void setPlatformColor(const QColor color);
    QColor platformColor() const;

    void setFontColor(const QColor color);
    QColor fontColor() const;

    void setPlate1Size(const int w, const int h);
    void setPlate2Size(const int w, const int h);

    void setPlate1(const int x, const int y);
    void setPlate2(const int x, const int y);

signals:
    /*!
     * \brief Сообщает миру о перемещении платформы
     * \param plate - номер платформы 0 - первая, 1 - вторая
     * \param x, y - новые координаты в мм
     */
    void movePlate(const int plate, const int x, const int y);

    /*!
     * \brief Сообщает миру об изменении размеров платформы
     * \param plate - номер платформы 0 - первая, 1 - вторая
     * \param w, h - новые размеры
     */
    void resizePlate(const int plate, const int w, const int h);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::PlatesPositioning *ui;

    QColor m_backgroundColor {Qt::black};
    QColor m_platformsColor {Qt::lightGray};
    QColor m_fontColor {Qt::black};

    QRect m_plate1 {QRect(0, 500, 500, 500)};
    QRect m_plate2 {QRect(500, 500, 500, 500)};

    int m_x0 {0};
    int m_y0 {0};
    int m_dx {0};
    int m_dy {0};
    double m_prop {1};
    QRect m_rectPlate1 {QRect(0, 0, 0, 0)};
    QRect m_rectPlate2 {QRect(0, 0, 0, 0)};

    int m_selectedPlate {-1};  ///< Номер выбранной платформы -1 - нет, 0 - первая, 1 - вторая
};

#endif // PLATESPOSITIONING_H
