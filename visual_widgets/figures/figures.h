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
    explicit Figures(QWidget *parent = 0);
    ~Figures();

    void setSize(const int size);
    void setFigureType(const FigureType ft);
    void setColor(const QColor color);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Figures *ui;

    int m_size {100};
    FigureType m_ft {ftEllipse};
    QColor m_color {Qt::red};
};

#endif // FIGURES_H
