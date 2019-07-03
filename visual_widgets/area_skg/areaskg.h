#ifndef AREASKG_H
#define AREASKG_H

#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
class AreaSKG;
}

class GridSKG;
class TraceSKG;

class AreaSKG : public QWidget
{
    Q_OBJECT

public:
    explicit AreaSKG(QWidget *parent = 0);
    ~AreaSKG();

    int diap() const;
    void setDiap(const int diap);

    /*!
     * \brief Устанавливает маркер
     * \param x и y - координаты маркера
     */
    void setMarker(const double x, const double y);

private:
    Ui::AreaSKG *ui;

    void setAreaSKG();

    QGraphicsScene* m_sceneSKG {nullptr};
    GridSKG* m_gridSKG {nullptr};
    TraceSKG* m_traceSKG {nullptr};
    QGraphicsRectItem* m_marker {nullptr};

    int m_diap {128};
};

#endif // AREASKG_H
