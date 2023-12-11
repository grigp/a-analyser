#ifndef BUBBLESIRRIANT_H
#define BUBBLESIRRIANT_H

#include <QObject>
#include <QTime>

#include "videoirritant.h"
#include "baseutils.h"

class BubblesIrriant : public Irriant
{
public:
    BubblesIrriant();

    /*!
     * \brief Возвращает уникальный идентификатор раздражителя
     */
    QString uid() override {return tr("bubbles");}
    /*!
     * \brief Возвращает показываемое название раздражителя
     */
    QString name() override {return tr("Пузырьки");}

    /*!
     * \brief Выполняет прорисовку раздражителя
     * \param painter - рисователь
     * \param option - опции
     * \param widget - виджет
     * \param boundingRect - границы зоны рисования
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, QRectF boundingRect) override;

    /*!
     * \brief Возвращает указатель на виджет редактирования параметров раздражителя
     */
    QWidget* getSettingsWidget() override;

    /*!
     * \brief Доступ к m_bubblesCount
     */
    int bubblesCount() const {return m_bubblesCount;}
    void setBubblesCount(const int cnt);

    /*!
     * \brief Доступ к m_maxSize
     */
    int maxSize() const {return m_maxSize;}
    void setMaxSize(const int maxSize);

    /*!
     * \brief досуп к m_color
     */
    QColor color() const {return m_color;}
    void setColor(const QColor color) {m_color = color;}

private:
    void initBubbles();

    int m_bubblesCount {50};
    int m_maxSize {40};
    QColor m_color {Qt::black};

    /*!
     * \brief Данные о пузырьке BubbleInfo struct
     */
    struct BubbleInfo
    {
        BubbleInfo(int _size, qreal _x, qreal _y, int _alpha, int _chAlpha)
            : size(_size)
            , x(_x)
            , y(_y)
            , alpha(_alpha)
            , chAlpha(_chAlpha)
        {}
        int size;
        qreal x;
        qreal y;
        int alpha;
        int chAlpha;
    };
    QList<BubbleInfo> m_bubbles;
};

#endif // BUBBLESIRRIANT_H
