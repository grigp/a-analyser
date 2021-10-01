#include "paintmix.h"
#include "ui_paintmix.h"

#include <QPainter>

PaintMix::PaintMix(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintMix)
{
    ui->setupUi(this);
}

PaintMix::~PaintMix()
{
    delete ui;
}

void PaintMix::setBackground(const QString fn)
{
    QPixmap pm;
    pm.load(fn);
    m_backgroung = pm.scaled(geometry().width(), geometry().height());
    update();
}

void PaintMix::addPicture(const int code, const QString fn, const QRectF rect, const bool isVisible)
{
    PixmapInfo pi;
    QPixmap pm;
    pm.load(fn);
    pi.pixmap = pm.scaled(static_cast<int>(rect.width()), static_cast<int>(rect.height()));
    pi.rect = rect;
    pi.isVisible = isVisible;

    m_pixmaps.insert(code, pi);
    update();
}

void PaintMix::setIsVisible(const int code, const bool isVisible)
{
    auto pi = m_pixmaps.value(code);
    pi.isVisible = isVisible;
    m_pixmaps.insert(code, pi);
}

void PaintMix::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawPixmap(geometry().x(), geometry().y(), m_backgroung);


    foreach (auto pixmap, m_pixmaps)
    {
        if (pixmap.isVisible)
        {
            painter.drawPixmap(static_cast<int>(pixmap.rect.x()), static_cast<int>(pixmap.rect.y()), pixmap.pixmap);
        }
    }
}
