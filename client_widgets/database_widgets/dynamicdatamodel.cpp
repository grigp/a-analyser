#include "dynamicdatamodel.h"

#include "factorsfactory.h"
#include "aanalyserapplication.h"

#include <QDebug>

DynamicDataModel::DynamicDataModel(QObject *parent)
    : QStandardItemModel(parent)
{

}

void DynamicDataModel::addTestData(const QList<FactorsDefines::FactorValueAdvanced> &factors,
                                   const QString &testUid, const QDateTime &dt)
{
    //! Для нулквой колонки - названия показателей
    if (columnCount() == 0)
    {
        QList<QStandardItem*> column;
        foreach (auto factor, factors)
        {
            auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factor.uid());

            QString caption = QString(tr("Проба") + " %1").arg(factor.probeNum() + 1) + ") " +
                              factor.description() + "\n" + fi.name();
            auto *item = new QStandardItem(caption);
            item->setData(factor.uid(), FactorUidRole);
            item->setData(factor.probeNum(), ProbeNumRole);
            item->setData(factor.description(), DescriptionRole);
            item->setData(factor.channelId(), ChannelIdRole);
            item->setEditable(false);

            column << item;
        }
        appendColumn(column);
        setHorizontalHeaderItem(0, new QStandardItem(""));
    }

    //! Значения показателей
    QList<QStandardItem*> column;
    foreach (auto factor, factors)
    {
        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factor.uid());
        auto *item = new QStandardItem(QString::number(factor.value(), 'f', fi.format()));
        item->setData(factor.value(), ValueRole);
        item->setEditable(false);
        column << item;
    }

    auto *headerItem = new QStandardItem(dt.toString("dd.MM.yyyy hh:mm"));
    headerItem->setEditable(false);
    headerItem->setData(dt, DateTimeRole);
    headerItem->setData(testUid, TestUidRole);

    auto colNum = getColumnNumberByDateTimeTest(dt);
    if (colNum == -1)
    {
        appendColumn(column);
        setHorizontalHeaderItem(columnCount() - 1, headerItem);
    }
    else
    {
        insertColumn(colNum, column);
        setHorizontalHeaderItem(colNum, headerItem);
    }
}

int DynamicDataModel::getColumnNumberByDateTimeTest(const QDateTime &dt) const
{
    int retval = -1;
    for (int i = 1; i < columnCount(); ++i)
    {
        auto testDt = horizontalHeaderItem(i)->data(DateTimeRole).toDateTime();
        if (dt < testDt)
        {
            retval = i;
            break;
        }
    }
    return retval;
}
