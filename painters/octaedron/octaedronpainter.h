#ifndef OCTAEDRONPAINTER_H
#define OCTAEDRONPAINTER_H

#include <QObject>
#include <QWidget>
#include <QPainter>

/*!
 * \brief Класс рисователя диаграммы теста "Октаэдр" The OctaedronPainter class
 */
class OctaedronPainter
{
public:
    explicit OctaedronPainter(QPainter* painter, QRect geometry);
    explicit OctaedronPainter();
    virtual ~OctaedronPainter();

    /*!
     * \brief Задает канву прорисовки. Будет испльзоваться в режиме виджета для задания
     * \param painter - рисователь
     * \param geometry - размер области прорисовки
     */
    void setCanvas(QPainter* painter, QRect geometry, QWidget *wgt = nullptr);

    void setLineColor(const QColor color) {m_lineColor = color;}
    QColor lineColor() const {return m_lineColor;}

    void setLabelColor(const QColor color) {m_labelColor = color;}
    QColor labelColor() const {return m_labelColor;}

    void setLabelTextColor(const QColor color) {m_labelTextColor = color;}
    QColor labelTextColor() const {return m_labelTextColor;}

    void setLabelFrameColor(const QColor color) {m_labelFrameColor = color;}
    QColor labelFrameColor() const {return m_labelFrameColor;}

    void setCirceRoundRuleMode(const int mode) {m_mode = mode;}
    int circeRoundRuleMode() const {return m_mode;}

    void setDirection(const int dir) {m_direction = dir;}
    int direction() const {return m_direction;}

    void setData(const int idx, const int value);
    int data(const int idx) const;

    /*!
     * \brief Процедура прорисовки на рисователе m_painter  в рамках m_geometry
     */
    void doPaint(const double ratio);


protected:
    virtual void doUpdate();

private:
    QPainter* m_painter {nullptr};
    QRect m_geometry {QRect(0, 0, 0, 0)};
    QWidget* m_widget {nullptr};

    QColor m_lineColor {QColor(0, 90, 150)};
    QColor m_labelColor {QColor(0, 90, 150)};
    QColor m_labelTextColor {Qt::white};
    QColor m_labelFrameColor {Qt::black};

    int m_mode {0};      ///< Режим просмотра 0 - радиальный, 1 - кольцевой
    int m_direction {1}; ///< Направление обхода 1 - по часовой, 2 - против часовой (для кольцевого режима)
    int m_data[8] {0, 0, 0, 0, 0, 0, 0, 0};   ///< Данные m_data[0] - верх -> далее против часовой до m_data[7]
};

#endif // OCTAEDRONPAINTER_H
