#include "devicecontroldialog.h"
#include "ui_devicecontroldialog.h"

#include "aanalyserapplication.h"
#include "stringvaluedelegate.h"
#include "comboboxdelegate.h"
#include "addconnectiondialog.h"

#include <QApplication>
#include <QDebug>

DeviceControlDialog::DeviceControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeviceControlDialog)
  , m_model(new DeviceControlModel(this))
{
    ui->setupUi(this);

    m_model->load();
    ui->tvConnections->setModel(m_model);

    ui->tvConnections->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::CurrentChanged);
    ui->tvConnections->setItemDelegateForColumn(DeviceControlModel::ColActive,
                                                new ActiveDelegate(ui->tvConnections));
    ui->tvConnections->setItemDelegateForColumn(DeviceControlModel::ColPort,
                                                new ComboBoxDelegate(ui->tvConnections));
    ui->tvConnections->setItemDelegateForColumn(DeviceControlModel::ColComment,
                                                new StringValueDelegate(ui->tvConnections));

    ui->tvConnections->header()->resizeSection(DeviceControlModel::ColActive, 60);
    ui->tvConnections->header()->resizeSection(DeviceControlModel::ColDriver, 150);
    ui->tvConnections->header()->resizeSection(DeviceControlModel::ColPort, 80);
}

DeviceControlDialog::~DeviceControlDialog()
{
    delete ui;
}

void DeviceControlDialog::addConnect()
{
    AddConnectionDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        m_model->appendRow(true, dlg.driverUid(), dlg.driverName(), QJsonObject(), dlg.port(), dlg.comment());
    }
}

void DeviceControlDialog::editConnect()
{
    qDebug() << "edit connect";
}

void DeviceControlDialog::delConnect()
{
    qDebug() << "del connect";
}

void DeviceControlDialog::upPriority()
{
    qDebug() << "up";
}

void DeviceControlDialog::downPriority()
{
    qDebug() << "down";
}


///<=============================================================================================
///< DeviceControlModel

DeviceControlModel::DeviceControlModel(QObject *parent)
    : QStandardItemModel(parent)
{

    setHorizontalHeaderLabels(QStringList() << "Актив." << "Драйвер" << "Порт" << "Комментарий");
}

DeviceControlModel::load()
{
    QList<Connection> connections = static_cast<AAnalyserApplication*>(QApplication::instance())->getConnections();
    foreach (auto connection, connections)
        appendRow(connection.active(),
                  connection.driverUid(),
                  connection.driverName(),
                  connection.params(),
                  connection.port(),
                  connection.comment());
}

void DeviceControlModel::appendRow(const bool active,
                                   const QString &drvUid, const QString &drvName, const QJsonObject &params,
                                   const QString &port, const QString &comment)
{
    auto *itemActive = new QStandardItem();
    itemActive->setData(active, ActiveRole);

    auto *itemDriver = new QStandardItem(drvName);
    itemDriver->setData(drvUid, DriverUidRole);
    itemDriver->setData(params, ParamsRole);
    itemDriver->setEditable(false);

    auto *itemPort = new QStandardItem(port);
    itemPort->setData(0, PortCodeRole);

    auto itemComment = new QStandardItem(comment);

    QStandardItemModel::appendRow(QList<QStandardItem*>() << itemActive << itemDriver << itemPort << itemComment);
}
