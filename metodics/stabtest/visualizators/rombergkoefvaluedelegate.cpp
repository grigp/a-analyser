#include "rombergkoefvaluedelegate.h"

#include <QPainter>
#include <QDebug>

#include "stabsignalstestwidget.h"
#include "datadefines.h"

RombergKoefValueDelegate::RombergKoefValueDelegate(QObject *parent)
    : QStyledItemDelegate (parent)
{

}

void RombergKoefValueDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    DataDefines::NormSideValue nv = static_cast<DataDefines::NormSideValue>(index.data(StabSignalsTestWidget::NormRole).toInt());

    painter->save();

    auto rv = option.rect;
    if (index.row() <= 1 && nv != DataDefines::nsvMissing)
    {
        QColor bColor = Qt::gray;
        QColor fColor = Qt::darkGray;
        if (nv == DataDefines::nsvNorm)
        {
            bColor = QColor(150, 255, 150); // Qt::green;
            fColor = QColor(0, 100, 50); // Qt::darkGreen;
        }
        else
        if (nv == DataDefines::nsvBelow)
        {
            bColor = QColor(255, 255, 150); //Qt::yellow;
            fColor = QColor(100, 100, 0); // Qt::darkYellow;
        }
        else
        if (nv == DataDefines::nsvAbove)
        {
            bColor = QColor(255, 100, 150); //Qt::red;
            fColor = QColor(100, 0, 0); //Qt::darkRed;
        }

        painter->setBrush(QBrush(bColor));
        painter->setPen(fColor);
        auto rn = option.rect;
        painter->drawRect(rn);
        painter->drawText(rn, Qt::AlignLeft, "  " + index.data().toString());
    }
    else
        painter->drawText(rv, Qt::AlignLeft, index.data().toString());

    painter->restore();
}
