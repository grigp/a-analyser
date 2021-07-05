#include "dualstatediagram.h"
#include "ui_dualstatediagram.h"

#include <QPainter>
#include <QDebug>

namespace
{
static const int Field = 10;

}

DualStateDiagram::DualStateDiagram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DualStateDiagram)
{
    ui->setupUi(this);
}

DualStateDiagram::~DualStateDiagram()
{
    delete ui;
}

void DualStateDiagram::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setBrush(QBrush(m_backgroundColor, Qt::SolidPattern));
    painter.setPen(QPen(m_backgroundColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(geometry());

    int x0 = geometry().width() / 2;
    int ym = geometry().height() / 2;
    int yh = geometry().height() / 4;
    double prop = (static_cast<double>(geometry().width()) / 2 - Field) / m_diap;

    //! Зоны незначительных изменений
    painter.setBrush(QBrush(m_insignificantlyColor, Qt::SolidPattern));
    painter.setPen(QPen(m_insignificantlyColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(static_cast<int>(x0 + m_insignificantlyLo * prop), ym - yh / 2,
                     static_cast<int>((m_moderateLo - m_insignificantlyLo) * prop), yh);
    painter.drawRect(static_cast<int>(x0 - m_moderateLo * prop), ym - yh / 2,
                     static_cast<int>((m_moderateLo - m_insignificantlyLo) * prop), yh);

    //! Зоны умеренных изменений
    painter.setBrush(QBrush(m_moderateColor, Qt::SolidPattern));
    painter.setPen(QPen(m_moderateColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(static_cast<int>(x0 + m_moderateLo * prop), ym - yh / 2,
                     static_cast<int>((m_averageLo - m_moderateLo) * prop), yh);
    painter.drawRect(static_cast<int>(x0 - m_averageLo * prop), ym - yh / 2,
                     static_cast<int>((m_averageLo - m_moderateLo) * prop), yh);

    //! Зоны выраженных изменений
    painter.setBrush(QBrush(m_averageColor, Qt::SolidPattern));
    painter.setPen(QPen(m_averageColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(static_cast<int>(x0 + m_averageLo * prop), ym - yh / 2,
                     static_cast<int>((m_diap - m_averageLo) * prop), yh);
    painter.drawRect(static_cast<int>(x0 - m_diap * prop), ym - yh / 2,
                     static_cast<int>((m_diap - m_averageLo) * prop), yh);

    //! Рамка диаграммы
    painter.setPen(QPen(m_frameColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(m_backgroundColor, Qt::NoBrush));
    painter.drawRect(Field, ym - yh / 2, geometry().width() - 2 * Field, yh);

    auto font = painter.font();
    font.setPixelSize(16);
    painter.setFont(font);
    painter.setPen(QPen(m_labelsColor, 1, Qt::SolidLine, Qt::FlatCap));

    //! Метка нуля
    painter.drawLine(x0, ym - yh / 2 - 5, x0, ym + yh / 2 + 5);
    painter.drawText(x0 - 5, ym + yh / 2 + 20, "0");
    painter.drawText(Field, ym + yh / 2 + 20, "-" + QString::number(m_diap));
    painter.drawText(geometry().width() - Field - 25, ym + yh / 2 + 20, QString::number(m_diap));

    //! Описания осей
    QFontMetrics fm(painter.font());
    auto lw = fm.width(m_descriptionRight);
    painter.drawText(Field, ym - yh / 2 - 5, m_descriptionLeft);
    painter.drawText(geometry().width() - Field - lw - 5, ym - yh / 2 - 5, m_descriptionRight);

    //! Значение
    painter.setPen(QPen(m_frameColor, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(m_valuerColor, Qt::SolidPattern));
    painter.drawRect(static_cast<int>(x0 + m_value * prop - 3), ym - yh / 2 - 10, 6, yh + 20);
}
