#include "dynamicteststimul.h"
#include "ui_dynamicteststimul.h"

#include <QPainter>
#include <QFontMetrics>

DynamicTestStimul::DynamicTestStimul(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DynamicTestStimul)
{
    ui->setupUi(this);
    m_targets.clear();
}

DynamicTestStimul::~DynamicTestStimul()
{
    delete ui;
}

void DynamicTestStimul::setMarker(const double x, const double y)
{
    m_x = x;
    m_y = y;
    update();
}

void DynamicTestStimul::initTargets(const int count, const QColor color, const int size)
{
    for (int i = 0; i < count; ++i)
        addTarget(0, 0, color, size);
}

void DynamicTestStimul::addTarget(const double x, const double y, const QColor color, const int size, const bool visible, const QString label)
{
    TargetInfo ti;
    ti.x = x;
    ti.y = y;
    ti.color = color;
    ti.size = size;
    ti.visible = visible;
    ti.label = label;
    m_targets.append(ti);
}

void DynamicTestStimul::setTargetInfo(const int idx, const QColor color, const int size, const QString label)
{
    Q_ASSERT((idx >= 0) && (idx < m_targets.size()));
    TargetInfo ti = m_targets.at(idx);
    ti.color = color;
    ti.size = size;
    ti.label = label;
    m_targets.replace(idx, ti);
}

void DynamicTestStimul::setTarget(const double x, const double y, const int idx)
{
    Q_ASSERT((idx >= 0) && (idx < m_targets.size()));
    TargetInfo ti = m_targets.at(idx);
    ti.x = x;
    ti.y = y;
    m_targets.replace(idx, ti);
}

void DynamicTestStimul::clearTargets()
{
    if (m_targets.size() > 0)
        m_targets.clear();
}

void DynamicTestStimul::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    //! Фон
    painter.setBrush(QBrush(m_backgroundColor, Qt::SolidPattern));
    painter.setPen(QPen(m_backgroundColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    int radius = geometry().width() / 2;
    if (geometry().width() > geometry().height())
        radius = geometry().height() / 2;

    //! Цели
    foreach (auto target, m_targets)
    {
        if (target.visible)
        {
            painter.setBrush(QBrush(target.color, Qt::SolidPattern));
            painter.setPen(QPen(target.color, 1, Qt::SolidLine, Qt::FlatCap));
            painter.drawRect(geometry().width() / 2 + target.x / m_diap * radius - target.size / 2,
                             geometry().height() / 2 - target.y / m_diap * radius - target.size / 2,
                             target.size, target.size);
        }
    }

    //! Маркер
    if (m_visibleMarker)
    {
        painter.setBrush(QBrush(m_markerColor, Qt::SolidPattern));
        painter.setPen(QPen(m_markerColor, 1, Qt::SolidLine, Qt::FlatCap));
        painter.drawRect(geometry().width() / 2 + m_x / m_diap * radius - m_markerSize / 2,
                         geometry().height() / 2 - m_y / m_diap * radius - m_markerSize / 2,
                         m_markerSize, m_markerSize);
    }
}
