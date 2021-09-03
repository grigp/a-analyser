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

    qreal cubeSize = static_cast<double>(geometry().width()) / 4;

    for (int i = 0; i < m_figure.size(); ++i)
    {
        for (int j = 0; j < m_figure.at(i).size(); ++j)
        {
            qreal x = geometry().width() / 2 - m_figure.at(i).size() * cubeSize / 2 + j * cubeSize;
            qreal y = geometry().height() / 2 - m_figure.size() * cubeSize / 2 + i * cubeSize;

//            QRectF rect(x, y, cubeSize, cubeSize);

            QColor color = m_figure[i][j];
//            int colorCode = color.red() * 16777216 + m_figure[i][j].green() * 65536 + m_figure[i][j].blue() * 256 + m_figure[i][j].alpha();
            QPixmap cube(m_cube);
            cube.scaled(static_cast<int>(cubeSize), static_cast<int>(cubeSize), Qt::KeepAspectRatio);
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
