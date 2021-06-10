#ifndef FIGURES_H
#define FIGURES_H

#include <QWidget>

namespace Ui {
class Figures;
}

class Figures : public QWidget
{
    Q_OBJECT

    ///< Типы фигур
    enum FigureType
    {
          ftEllipse = 0
        , ftRect
        , ftTriangle
    };

public:
    explicit Figures(QWidget *parent = nullptr);
    ~Figures() override;

    void setSize(const int size);
    void setFigureType(const FigureType ft);

    void setColor(const QColor color);
    QColor color() const;

    void setBackgroundColor(const QColor color);
    QColor backgroundColor() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Figures *ui;

    int m_size {100};
    FigureType m_ft {ftEllipse};
    QColor m_color {Qt::white};
    QColor m_backgroundColor {Qt::darkGray};
};

#endif // FIGURES_H
