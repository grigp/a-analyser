#ifndef TARGETAREA_H
#define TARGETAREA_H

#include <QWidget>

namespace Ui {
class TargetArea;
}

/*!
 * \brief Класс виджета онлайновой мишени TargetArea class
 */
class TargetArea : public QWidget
{
    Q_OBJECT

public:
    explicit TargetArea(QWidget *parent = nullptr);
    ~TargetArea() override;

    void setDiap(const int diap);
    int diap() const;

    void setMarker(const double x, const double y);

    void setBackgroundColor(const QColor color);
    QColor backgroundColor() const;

    void setSpaceColor(const QColor color);
    QColor spaceColor() const;

    void setCenterColor(const QColor color);
    QColor centerColor() const;

    void setMarkerColor(const QColor color);
    QColor markerColor() const;

    void setMarkerSize(const int size);
    int markerSize() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::TargetArea *ui;

    int m_diap {128};
    double m_x {0};
    double m_y {0};
    int m_markerSize {20};

    QColor m_backgroundColor {Qt::darkGray};
    QColor m_spaceColor {Qt::white};
    QColor m_centerColor {Qt::black};
    QColor m_markerColor {Qt::red};
};

#endif // TARGETAREA_H
