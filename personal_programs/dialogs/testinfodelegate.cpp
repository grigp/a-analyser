#include "testinfodelegate.h"

#include <QPainter>
#include <QAbstractItemView>
#include <QDebug>

#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "personalprogramdefines.h"
#include "personalprogrammanager.h"
#include "baseutils.h"
#include "dataprovider.h"


TestInfoDelegate::TestInfoDelegate(QObject *parent)
    : QStyledItemDelegate (parent)
{

}

void TestInfoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto uidMethod = index.data(PersonalProgramDefines::PersonalProgram::MethodUidRole).toString();

    if (uidMethod != "")
    {
        MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
        MetodicDefines::MetodicInfo mi = metFactory->metodic(uidMethod);
        auto uidTest = index.data(PersonalProgramDefines::PersonalProgram::TestUidRole).toString();
        auto valSuccess = getSuccessFactorValue(uidTest);

        painter->save();

        QColor colBackground = QColor(250, 250, 250);
        QColor colText = Qt::gray;
        if (uidTest != "")
        {
            colBackground = QColor(0, 200, 255);
            colText = QColor(0, 100, 0);

            if (valSuccess > -1)
                colBackground = PersonalProgramManager::successColor(valSuccess);
        }
        QColor colBackgroundLt = PersonalProgramManager::lightColor(colBackground, 2);
        QColor colBackgroundDk = PersonalProgramManager::darkColor(colBackground, 2);

        auto rn = option.rect;
        rn.setHeight(rn.height() - 2);
        rn.setTop(rn.top() + 1);
        rn.setWidth(rn.width() - 2);
        rn.setLeft(rn.left() + 1);

        QLinearGradient gradient(QPointF(rn.left() + rn.width() / 2, rn.top()), QPointF(rn.left() + rn.width() / 2, rn.top() + rn.height()));
        gradient.setColorAt(0, Qt::white);
        gradient.setColorAt(0.2, colBackgroundLt);
        gradient.setColorAt(0.5, colBackground);
        gradient.setColorAt(0.8, colBackgroundLt);
        gradient.setColorAt(1, Qt::white);
        painter->setBrush(gradient);  //QBrush(colBackground));
        painter->setPen(colBackgroundDk);

        painter->drawRoundRect(rn, 15, 25);

        painter->drawText(rn, Qt::AlignTop | Qt::AlignHCenter, mi.shortName);
        if (valSuccess > -1)
        {
            painter->setFont(QFont("Sans", 18, QFont::Bold, false));
            painter->drawText(rn, Qt::AlignVCenter| Qt::AlignHCenter, QString::number(static_cast<int>(valSuccess)) + " %");
        }
        else
        {
            auto rp = QRect(rn.left(), rn.top() + 15, rn.width(), rn.height() - 15);
            metFactory->paintPreview(painter, rp, uidTest);
        }

        //! Выделение итема
        if (QAbstractItemView* tableView = qobject_cast<QAbstractItemView*>(this->parent()))
        {
            auto selIdxs = tableView->selectionModel()->selectedIndexes();
            if (selIdxs.contains(index))
            {
                painter->setBrush(QBrush(Qt::NoBrush));
                painter->setPen(QPen(Qt::red, 2, Qt::SolidLine));
                painter->drawRect(rn);
            }
        }
        painter->restore();
    }
}

double TestInfoDelegate::getSuccessFactorValue(const QString &uidTest) const
{
    auto factors = DataProvider::getPrimaryFactors(uidTest);
    foreach (auto factor, factors)
        if (factor.uid() == FactorsDefines::CommonFactors::SuccessUid)
            return factor.value();
    return -1;
}

