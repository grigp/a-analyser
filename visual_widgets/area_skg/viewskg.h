#ifndef VIEWSKG_H
#define VIEWSKG_H

#include <QObject>
#include <QGraphicsView>
#include <QResizeEvent>

class ViewSKG : public QGraphicsView
{
    Q_OBJECT
public:
    ViewSKG(QWidget* parent = Q_NULLPTR);

signals:
    void doResize(QSize size);

protected:
    virtual void resizeEvent(QResizeEvent *event);

};

#endif // VIEWSKG_H
