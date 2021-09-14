#include "tetrisfigure.h"
#include "ui_tetrisfigure.h"

#include "baseutils.h"

#include <QPainter>
#include <QResizeEvent>
#include <QDebug>

TetrisFigure::TetrisFigure(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TetrisFigure)
{
    ui->setupUi(this);
}

TetrisFigure::~TetrisFigure()
{
    delete ui;
}

void TetrisFigure::setCubeFileName(const QString fn)
{
    m_cube.load(fn);
}

void TetrisFigure::setFigure(const QVector<QVector<QColor> > figure)
{
    m_figure = figure;
    update();
}

void TetrisFigure::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    //! Рамка
    painter.setBrush(QBrush(Qt::gray, Qt::NoBrush));
    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine, Qt::FlatCap));
    painter.drawRect(0, 0, geometry().width() - 1, geometry().height() - 1);

    qreal cubeSize = static_cast<double>(geometry().width()) / 4;
    if (cubeSize > qMin(m_cube.width(), m_cube.height()))
        cubeSize = qMin(m_cube.width(), m_cube.height());

    for (int i = 0; i < m_figure.size(); ++i)
    {
        for (int j = 0; j < m_figure.at(i).size(); ++j)
        {
            qreal x = geometry().width() / 2 - m_figure.at(i).size() * cubeSize / 2 + j * cubeSize;
            qreal y = geometry().height() / 2 - m_figure.size() * cubeSize / 2 + i * cubeSize;

            QColor color = m_figure[i][j];
            QPixmap cube = m_cube.scaled(static_cast<int>(cubeSize), static_cast<int>(cubeSize));
            BaseUtils::setColoredPicture(cube, color);

            painter.drawPixmap(static_cast<int>(x), static_cast<int>(y), cube);
        }

    }
}

void TetrisFigure::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    //! Должен быть квадратным, со стороной, равной ширине
    setMinimumSize(event->size().width(), event->size().width());
}
