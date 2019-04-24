#include "metodicsmodel.h"

#include "dataprovider.h"

#include <QJsonDocument>

MetodicsModel::MetodicsModel(QObject *parent)
    : QStandardItemModel(parent)
{

}

void MetodicsModel::editMetodicParams(QWidget *parent, const int rowNum)
{
    if (index(rowNum, ColName).isValid())
    {
        auto idx = index(rowNum, ColName);
        QString uid = idx.data(MetodicUidRole).toString();

        if (uid != "")
            if (DataProvider::editMetodicParams(parent, uid))
            {
            }
    }
}

void MetodicsModel::load()
{
    QList<MetodicDefines::MetodicInfo> list = DataProvider::getListMetodisc();
    foreach (auto met, list)
    {
        QStandardItem *itemName = new QStandardItem(met.name);
        itemName->setData(met.uid, MetodicUidRole);
        itemName->setEditable(false);
        appendRow(itemName);
    }
}
