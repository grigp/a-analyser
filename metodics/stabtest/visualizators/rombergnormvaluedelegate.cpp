#include "rombergnormvaluedelegate.h"

#include "stabsignalstestwidget.h"
#include "datadefines.h"
#include <QPainter>

RombergNormValueDelegate::RombergNormValueDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void RombergNormValueDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    DataDefines::NormValue nv = static_cast<DataDefines::NormValue>(index.data(StabSignalsTestWidget::NormRole).toInt());
    auto value = index.data(StabSignalsTestWidget::ValueRole).toDouble();
    auto format = index.data(StabSignalsTestWidget::FormatRole).toInt();

    painter->save();

    auto rv = option.rect;
    rv.setWidth(50);
    painter->drawText(rv, Qt::AlignCenter, QString::number(value, 'f', format));

    QString sNorm = "";
    QColor bColor = Qt::gray;
    QColor fColor = Qt::darkGray;
    if (nv == DataDefines::Normal)
    {
        sNorm = tr("В норме");
        bColor = QColor(150, 255, 150); // Qt::green;
        fColor = QColor(0, 100, 50); // Qt::darkGreen;
    }
    else
    if (nv == DataDefines::ConditionNormal)
    {
        sNorm = tr("Условно в норме");
        bColor = QColor(255, 255, 150); //Qt::yellow;
        fColor = QColor(100, 100, 0); // Qt::darkYellow;
    }
    else
    if (nv == DataDefines::NotNormal)
    {
        sNorm = tr("Не в норме");
        bColor = QColor(255, 100, 150); //Qt::red;
        fColor = QColor(100, 0, 0); //Qt::darkRed;
    }

    painter->setBrush(QBrush(bColor));
    painter->setPen(fColor);
    auto rn = option.rect;
    rn.setLeft(option.rect.left() + 50);
    painter->drawRect(rn);
    painter->drawText(rn, Qt::AlignCenter, sNorm);

    painter->restore();

    QStyledItemDelegate::paint(painter, option, index);
}
