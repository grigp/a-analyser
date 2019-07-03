#include "viewskg.h"

ViewSKG::ViewSKG(QWidget* parent)
    : QGraphicsView(parent)
{

}

void ViewSKG::resizeEvent(QResizeEvent *event)
{
    emit doResize(event->size());
    QGraphicsView::resizeEvent(event);
}
