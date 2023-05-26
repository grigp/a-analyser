#ifndef FIVEZONEAREA_H
#define FIVEZONEAREA_H

#include <QWidget>

namespace Ui {
class FiveZoneArea;
}

/*!
 * \brief Класс виджета онлайновой выделенной зоны FiveZoneArea class
 */
class FiveZoneArea : public QWidget
{
    Q_OBJECT

public:
    explicit FiveZoneArea(QWidget *parent = nullptr);
    ~FiveZoneArea() override;

    void setDiap(const int diap);
    int diap() const;

    void setMarker(const double x, const double y);

    void setBackgroundColor(const QColor color);
    QColor backgroundColor() const;

    void setGridColor(const QColor color);
    QColor gridColor() const;

    void setMarkerColor(const QColor color);
    QColor markerColor() const;

    void setMarkerSize(const int size);
    int markerSize() const;

protected:
    void paintEvent(QPaintEvent *event) override;


private:
    Ui::FiveZoneArea *ui;

    int m_diap {128};
    double m_x {0};
    double m_y {0};
    int m_markerSize {200};

    QColor m_backgroundColor {Qt::darkGray};
    QColor m_gridColor {Qt::lightGray};
    QColor m_markerColor {Qt::red};
};

#endif // FIVEZONEAREA_H
