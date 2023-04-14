#include "dualstatediagrampainter.h"

#include <QDebug>

namespace
{
static const int Field = 10;

}


DualStateDiagramPainter::DualStateDiagramPainter()
{

}

DualStateDiagramPainter::DualStateDiagramPainter(QPainter *painter, QRect geometry)
 : m_painter(painter), m_geometry(geometry)
{

}

DualStateDiagramPainter::~DualStateDiagramPainter()
{

}

void DualStateDiagramPainter::setCanvas(QPainter *painter, QRect geometry, QWidget *wgt)
{
    m_painter = painter;
    m_geometry = geometry;
    m_widget = wgt;
}

void DualStateDiagramPainter::doPaint(const double ratio)
{
    QColor backColor = Qt::white;
    if (m_widget)
        backColor = m_widget->palette().background().color();
    m_painter->setBrush(QBrush(backColor , Qt::SolidPattern));
    m_painter->setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->drawRect(m_geometry);

    int x0 = m_geometry.left() + m_geometry.width() / 2;
    int ym = m_geometry.top() + m_geometry.height() / 2;
    int yh = m_geometry.height() / 4;
    double prop = (static_cast<double>(m_geometry.width()) / 2 - Field) / m_diap;

    //! Зоны незначительных изменений
    m_painter->setBrush(QBrush(m_insignificantlyColor, Qt::SolidPattern));
    m_painter->setPen(QPen(m_insignificantlyColor, 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->drawRect(static_cast<int>(x0 + m_insignificantlyLo * prop), ym - yh / 2,
                        static_cast<int>((m_moderateLo - m_insignificantlyLo) * prop), yh);
    m_painter->drawRect(static_cast<int>(x0 - m_moderateLo * prop), ym - yh / 2,
                        static_cast<int>((m_moderateLo - m_insignificantlyLo) * prop), yh);

    //! Зоны умеренных изменений
    m_painter->setBrush(QBrush(m_moderateColor, Qt::SolidPattern));
    m_painter->setPen(QPen(m_moderateColor, 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->drawRect(static_cast<int>(x0 + m_moderateLo * prop), ym - yh / 2,
                        static_cast<int>((m_averageLo - m_moderateLo) * prop), yh);
    m_painter->drawRect(static_cast<int>(x0 - m_averageLo * prop), ym - yh / 2,
                        static_cast<int>((m_averageLo - m_moderateLo) * prop), yh);

    //! Зоны выраженных изменений
    m_painter->setBrush(QBrush(m_averageColor, Qt::SolidPattern));
    m_painter->setPen(QPen(m_averageColor, 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->drawRect(static_cast<int>(x0 + m_averageLo * prop), ym - yh / 2,
                        static_cast<int>((m_diap - m_averageLo) * prop), yh);
    m_painter->drawRect(static_cast<int>(x0 - m_diap * prop), ym - yh / 2,
                        static_cast<int>((m_diap - m_averageLo) * prop), yh);

    //! Рамка диаграммы
    m_painter->setPen(QPen(m_frameColor, 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->setBrush(QBrush(backColor, Qt::NoBrush));
    m_painter->drawRect(m_geometry.left() + Field, ym - yh / 2, m_geometry.width() - 2 * Field, yh);

    m_painter->setFont(QFont("Arial", static_cast<int>(8/* * ratio*/), 0, false));
//    qDebug() << ratio << 8; // * ratio;
    m_painter->setPen(QPen(m_labelsColor, 1, Qt::SolidLine, Qt::FlatCap));

    //! Метка нуля
    m_painter->drawLine(x0, ym - yh / 2 - 5, x0, ym + yh / 2 + 5);
    m_painter->drawText(x0 - 5,
                        static_cast<int>(ym + yh / 2 + 20 * ratio),
                        "0");
    m_painter->drawText(m_geometry.left() + Field,
                        static_cast<int>(ym + yh / 2 + 20 * ratio),
                        "-" + QString::number(m_diap));
    m_painter->drawText(m_geometry.left() + m_geometry.width() - Field - 25,
                        static_cast<int>(ym + yh / 2 + 20 * ratio),
                        QString::number(m_diap));

    //! Описания осей
    QFontMetrics fm(m_painter->font());
    auto lw = fm.width(m_descriptionRight);
    m_painter->drawText(m_geometry.left() + Field, ym - yh / 2 - 5, m_descriptionLeft);
    m_painter->drawText(m_geometry.left() + m_geometry.width() - Field - lw - 5, ym - yh / 2 - 5, m_descriptionRight);

    //! Значение
    m_painter->setPen(QPen(m_frameColor, 1, Qt::SolidLine, Qt::FlatCap));
    m_painter->setBrush(QBrush(m_valuerColor, Qt::SolidPattern));
    m_painter->drawRect(static_cast<int>(x0 + m_value * prop - 3 * ratio),
                        static_cast<int>(ym - yh / 2 - 10 * ratio),
                        static_cast<int>(6 * ratio),
                        static_cast<int>(yh + 20 * ratio));
}

void DualStateDiagramPainter::doUpdate()
{

}
