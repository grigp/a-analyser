#include "metodicsmodel.h"

#include "dataprovider.h"

#include <QJsonDocument>
#include <QUuid>
#include <QDebug>

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
    QList<MetodicDefines::MetodicInfo> list = DataProvider::getListMetodics();
    foreach (auto met, list)
    {
        QStandardItem *itemName = new QStandardItem(met.name);
        itemName->setData(met.uid, MetodicUidRole);
        itemName->setIcon(QIcon(":/images/Methodics/" + met.imageName));
        itemName->setEditable(false);
        appendRow(itemName);
    }
}

MetodicsKindModel::MetodicsKindModel(QObject *parent)
    : QStandardItemModel(parent)
{

}

void MetodicsKindModel::load()
{
    //! Первая строка <Все>
    QStandardItem *itemName = new QStandardItem("<Все>");
    itemName->setData(QUuid().toString(), MetodicKindUidRole);
    itemName->setIcon(QIcon(":/images/Methodics/Empty.png"));
    itemName->setEditable(false);
    appendRow(itemName);

    QList<MetodicDefines::MetodicKindInfo> list = DataProvider::getListMetodicKinds();
    foreach (auto kind, list)
    {
        QStandardItem *itemName = new QStandardItem(kind.name);
        itemName->setData(kind.uid, MetodicKindUidRole);
        itemName->setIcon(QIcon(":/images/Methodics/" + kind.imageName));
        itemName->setEditable(false);
        appendRow(itemName);
    }

    setHorizontalHeaderLabels(QStringList() << tr("Типы методик"));
}
