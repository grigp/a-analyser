#include "vectorcloud.h"
#include "ui_vectorcloud.h"

#include <QPainter>

#include "baseutils.h"

namespace  {
///< Отступы от краев виджета
static const int LeftSpace = 20;
static const int RightSpace = 10;
static const int TopSpace = 30;
static const int BottomSpace = 20;
}

VectorCloud::VectorCloud(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VectorCloud)
{
    ui->setupUi(this);
}

VectorCloud::~VectorCloud()
{
    delete ui;
}

void VectorCloud::add(const QPointF vector)
{
    m_vectors << vector;
}

void VectorCloud::addDiapLimit(const double diap)
{
    m_diaps << diap;
}

void VectorCloud::setDiapazone(const double diap)
{
    m_diap = diap;
    update();
}

void VectorCloud::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.save();

    //! Фон
    auto backColor = palette().background().color();
    painter.setBrush(QBrush(backColor, Qt::SolidPattern));
    painter.setPen(QPen(backColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(0, 0, width(), height());

    QRect diagRect;
    if (width() - LeftSpace - RightSpace < height() - TopSpace - BottomSpace)
    {
        int w = width() - LeftSpace - RightSpace;
        diagRect = QRect(LeftSpace, height() / 2 - w / 2, w,w);
    }
    else
    {
        int h = height() - TopSpace - BottomSpace;
        diagRect = QRect(width() / 2 - h / 2, TopSpace, h, h);
    }

    //! Оси
    painter.setPen(QPen(m_axizColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(diagRect);
    painter.drawLine(diagRect.x(), diagRect.center().y(), diagRect.x() + diagRect.width(), diagRect.center().y());
    painter.drawLine(diagRect.center().x(), diagRect.y(), diagRect.center().x(), diagRect.y() + diagRect.height());

    //! Заголовок
    painter.setFont(QFont("Sans", 10, QFont::Bold, false));
    auto size = BaseUtils::getTextSize(&painter, m_title);
    painter.drawText(width() / 2 - size.width() / 2, 2 + size.height(), m_title);

    //! Пропорция
    double prop = (static_cast<double>(diagRect.width()) / 2) / (m_diap);

    //! Облако векторов
    painter.setPen(QPen(m_vectorColor, 1, Qt::SolidLine, Qt::FlatCap));
    foreach (auto vector, m_vectors)
        painter.drawEllipse(static_cast<int>(diagRect.x() + diagRect.width() / 2 + vector.x() * prop - 1),
                            static_cast<int>(diagRect.y() + diagRect.height() / 2 + vector.y() * prop - 1),
                            2, 2);

    //! Круги диапазонов
    painter.setPen(QPen(m_zonesColor, 1, Qt::DashLine, Qt::FlatCap));
    painter.setBrush(QBrush(backColor, Qt::NoBrush));
    foreach (auto diap, m_diaps)
        painter.drawEllipse(static_cast<int>(diagRect.x() + diagRect.width() / 2 - diap * prop),
                            static_cast<int>(diagRect.y() + diagRect.height() / 2 - diap * prop),
                            static_cast<int>(diap * prop * 2), static_cast<int>(diap * prop * 2));

    painter.restore();
}
