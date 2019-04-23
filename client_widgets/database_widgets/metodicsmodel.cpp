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
//        QByteArray ba = idx.data(MetodicParamsRole).toByteArray();
//        QJsonDocument doc(QJsonDocument::fromJson(ba));
//        QJsonObject params = doc.object();


        if (uid != "")
            if (DataProvider::editMetodicParams(parent, uid))
            {
//                QJsonDocument doc(params);
//                QByteArray ba = doc.toJson();
//                itemFromIndex(idx)->setData(ba, MetodicParamsRole);
            }
    }
}

void MetodicsModel::load()
{
    QList<MetodicDefines::MetodicInfo> list = DataProvider::getListMetodisc();
    foreach (auto met, list)
    {
        QStandardItem *itemName = new QStandardItem(met.name);
//        itemName->setData(met.templateId, TemplateUidRole);
        itemName->setData(met.uid, MetodicUidRole);
//        QJsonDocument doc(met.params);
//        QByteArray ba = doc.toJson();
//        itemName->setData(ba, MetodicParamsRole);
        appendRow(itemName);
    }
}
