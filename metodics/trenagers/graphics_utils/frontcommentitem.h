#ifndef FRONTCOMMENTITEM_H
#define FRONTCOMMENTITEM_H

#include <QObject>
#include <QGraphicsItem>

class FrontCommentItem : public QGraphicsItem
{
public:
    explicit FrontCommentItem(const QRectF &rect, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setText(const QString& text);

private:
    QRectF m_rect;
    QString m_text;
};

#endif // FRONTCOMMENTITEM_H
