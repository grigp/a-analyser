#ifndef AREAGRAPH_H
#define AREAGRAPH_H

#include <QWidget>

#include "graphpainter.h"

namespace Ui {
class AreaGraph;
}



/*!
 * \brief Класс виджета построения графиков AreaGraph class
 */
class AreaGraph : public QWidget, public GraphPainter
{
    Q_OBJECT

    Q_PROPERTY(QColor line1_color MEMBER m_line1Color DESIGNABLE true)
    Q_PROPERTY(QColor line2_color MEMBER m_line2Color DESIGNABLE true)
    Q_PROPERTY(QColor line3_color MEMBER m_line3Color DESIGNABLE true)
    Q_PROPERTY(QColor line4_color MEMBER m_line4Color DESIGNABLE true)
    Q_PROPERTY(QColor line5_color MEMBER m_line5Color DESIGNABLE true)
    Q_PROPERTY(QColor line6_color MEMBER m_line6Color DESIGNABLE true)
    Q_PROPERTY(QColor fillBetweenSubchans_color READ colorFillBetweenSubchans WRITE setColorFillBetweenSubchans DESIGNABLE true)

public:
    explicit AreaGraph(QWidget *parent = nullptr);
    ~AreaGraph() override;

    /*!
     * \brief Добавляет зону построения графика с (блэкджеком и шлюхами) сигналом и номером отведения
     * \param signal - сигнал
     * \param name - название зоны
     * \param numSubChan - номер отведения
     */


signals:
    void moveCursor();

    /*!
     * \brief Сигнал нажатия мышки на теле виджета
     * \param x, y - координаты нажатия
     * \param buttons - нажатые кнопки
     */
    void press(const int x, const int y, const Qt::MouseButtons buttons);
    /*!
     * \brief Сигнал отпускания мышки на теле виджета
     * \param x, y - координаты отпускания
     * \param buttons - нажатые кнопки
     */
    void release(const int x, const int y, const Qt::MouseButtons buttons);
    /*!
     * \brief Сигнал переноса мышки по телу виджета
     * \param x, y - координаты положения
     * \param buttons - нажатые кнопки
     */
    void move(const int x, const int y, const Qt::MouseButtons buttons);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void doUpdate() override;
    void doMoveCursor() override;

private:
    Ui::AreaGraph *ui;


};

#endif // AREAGRAPH_H
