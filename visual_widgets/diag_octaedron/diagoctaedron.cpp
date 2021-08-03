#include "diagoctaedron.h"
#include "ui_diagoctaedron.h"

#include <QPainter>
#include <QDebug>

namespace
{
struct ArrowIndexes
{
    int from;
    int to;
    ArrowIndexes(const int f, const int t)
        : from(f), to(t) {}
};

//! Порядок индексов для прохода в кольцевом режиме по часовой стрелке
static const QList<ArrowIndexes> CircleCWIndexes =
{
    ArrowIndexes(0, 1),
    ArrowIndexes(1, 2),
    ArrowIndexes(2, 3),
    ArrowIndexes(3, 4),
    ArrowIndexes(4, 5),
    ArrowIndexes(5, 6),
    ArrowIndexes(6, 7),
    ArrowIndexes(7, 0)
};

//! Порядок индексов для прохода в кольцевом режиме против часовой стрелки
static const QList<ArrowIndexes> CircleCCWIndexes =
{
    ArrowIndexes(0, 7),
    ArrowIndexes(1, 0),
    ArrowIndexes(2, 1),
    ArrowIndexes(3, 2),
    ArrowIndexes(4, 3),
    ArrowIndexes(5, 4),
    ArrowIndexes(6, 5),
    ArrowIndexes(7, 6)
};

}

DiagOctaedron::DiagOctaedron(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiagOctaedron)
{
    ui->setupUi(this);
}

DiagOctaedron::~DiagOctaedron()
{
    delete ui;
}

void DiagOctaedron::setData(const int idx, const int value)
{
    Q_ASSERT(idx >= 0 && idx < 8);
    Q_ASSERT(value >= 0 && value <= 100);
    m_data[idx] = value;
}

int DiagOctaedron::data(const int idx) const
{
    return m_data[idx];
}

void DiagOctaedron::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setBrush(QBrush(m_backgroundColor, Qt::SolidPattern));
    painter.setPen(QPen(m_backgroundColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    int mx = geometry().width() / 2;
    int my = geometry().height() / 2;
    int radius = static_cast<int>(geometry().height() / 2 * 0.85);
    const int sizeLabel = 40;

    auto font = painter.font();
    font.setPixelSize(18);
    font.setBold(true);
    painter.setFont(font);

    //! Прямоугольники меток
    QList<QRect> labels = {
        QRect(mx - sizeLabel / 2, my - radius - sizeLabel / 2, sizeLabel, sizeLabel),
        QRect(static_cast<int>(mx - radius * cos(M_PI_4) - sizeLabel / 2),
              static_cast<int>(my - radius * sin(M_PI_4) - sizeLabel / 2), sizeLabel, sizeLabel),
        QRect(mx - radius - sizeLabel / 2, my - sizeLabel / 2, sizeLabel, sizeLabel),
        QRect(static_cast<int>(mx - radius * 0.8 * cos(M_PI_4) - sizeLabel / 2),
              static_cast<int>(my + radius * 0.8 * sin(M_PI_4) - sizeLabel / 2), sizeLabel, sizeLabel),
        QRect(mx - sizeLabel / 2, static_cast<int>(my + radius * 0.7 - sizeLabel / 2), sizeLabel, sizeLabel),
        QRect(static_cast<int>(mx + radius * 0.8 * cos(M_PI_4) - sizeLabel / 2),
              static_cast<int>(my + radius * 0.8 * sin(M_PI_4) - sizeLabel / 2), sizeLabel, sizeLabel),
        QRect(mx + radius - sizeLabel / 2, my - sizeLabel / 2, sizeLabel, sizeLabel),
        QRect(static_cast<int>(mx + radius * cos(M_PI_4) - sizeLabel / 2),
              static_cast<int>(my - radius * sin(M_PI_4) - sizeLabel / 2), sizeLabel, sizeLabel)
    };

    //! Диаграмма
    painter.setBrush(QBrush(m_labelColor, Qt::SolidPattern));
    painter.setPen(QPen(m_lineColor, 5, Qt::SolidLine, Qt::FlatCap));

    //! Радиальный режим
    if (m_mode == 0)
    {
        for (int i = 0; i < labels.size(); ++i)
        {
            int lx = labels.at(i).center().x();
            int ly = labels.at(i).center().y();
            painter.drawLine(mx, my, mx + (lx - mx) * m_data[i] / 100, my + (ly - my) * m_data[i] / 100);
        }
    }
    else
    //! Кольцевой режим
    if (m_mode == 1)
    {
        QList<ArrowIndexes> indexes = CircleCWIndexes;
        if (m_direction == 2)
            indexes = CircleCCWIndexes;
        for (int i = 0; i < labels.size(); ++i)
        {
            int lx = labels.at(indexes.at(i).to).center().x();
            int ly = labels.at(indexes.at(i).to).center().y();
            int tx = labels.at(indexes.at(i).from).center().x();
            int ty = labels.at(indexes.at(i).from).center().y();
            painter.drawLine(lx, ly, lx + (tx - lx) * m_data[i] / 100, ly + (ty - ly) * m_data[i] / 100);
        }
    }

    //! Метки
    for (int i = 0; i < labels.size(); ++i)
    {
        painter.setPen(QPen(m_labelFrameColor, 1, Qt::SolidLine, Qt::FlatCap));
        painter.drawRect(labels.at(i));

        painter.setPen(QPen(m_labelTextColor, 1, Qt::SolidLine, Qt::FlatCap));
        painter.drawText(labels.at(i), QString::number(m_data[i]), QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
    }

}


