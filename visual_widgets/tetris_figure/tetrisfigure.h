#ifndef TETRISFIGURE_H
#define TETRISFIGURE_H

#include <QWidget>

namespace Ui {
class TetrisFigure;
}

/*!
 * \brief Класс виджета, показывающий фигуру TetrisFigure class
 */
class TetrisFigure : public QWidget
{
    Q_OBJECT

public:
    explicit TetrisFigure(QWidget *parent = nullptr);
    ~TetrisFigure() override;

    /*!
     * \brief Доступ к имени файла кубика
     */
    void setCubeFileName(const QString fn);

    /*!
     * \brief Устанавливает фигуру для отображения
     */
    void setFigure(const QVector<QVector<QColor>> figure);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;


private:
    Ui::TetrisFigure *ui;

    QVector<QVector<QColor>> m_figure;  ///< Данные фигуры
    QPixmap m_cube;                     ///< Картинка кубика

};

#endif // TETRISFIGURE_H
