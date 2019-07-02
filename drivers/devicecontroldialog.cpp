#include "devicecontroldialog.h"
#include "ui_devicecontroldialog.h"

#include "aanalyserapplication.h"
#include "stringvaluedelegate.h"
#include "addconnectiondialog.h"

#include <QApplication>
#include <QComboBox>
#include <QMessageBox>
#include <QVariant>
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
                                                new ComPortDeledate(ui->tvConnections));
    ui->tvConnections->setItemDelegateForColumn(DeviceControlModel::ColComment,
                                                new StringValueDelegate(ui->tvConnections));

    ui->tvConnections->header()->resizeSection(DeviceControlModel::ColActive, 60);
    ui->tvConnections->header()->resizeSection(DeviceControlModel::ColDriver, 150);
    ui->tvConnections->header()->resizeSection(DeviceControlModel::ColPort, 80);

    connect(m_model, &DeviceControlModel::dataChanged, this, &DeviceControlDialog::on_dataChanged);
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
        // Редактирование параметров
        QJsonObject params;
        static_cast<AAnalyserApplication*>(QApplication::instance())->editParamsConnecton(-1, dlg.driverUid(), params);

        m_model->appendRow(true, dlg.driverUid(), dlg.driverName(), params, dlg.port(), dlg.comment());
        static_cast<AAnalyserApplication*>(QApplication::instance())->
                addConnection(true, dlg.driverUid(), params, dlg.port(), dlg.comment());
    }
}

void DeviceControlDialog::editConnect()
{
    auto selIdxs = ui->tvConnections->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        const int rowIdx = selIdxs.at(0).row();
        auto drvUid = m_model->index(rowIdx, DeviceControlModel::ColDriver).
                data(DeviceControlModel::DriverUidRole).toString();
        auto params = m_model->index(rowIdx, DeviceControlModel::ColDriver).
                data(DeviceControlModel::ParamsRole).toJsonObject();

        if (static_cast<AAnalyserApplication*>(QApplication::instance())->editParamsConnecton(rowIdx, drvUid, params))
            m_model->setData(m_model->index(rowIdx, DeviceControlModel::ColDriver),
                             params, DeviceControlModel::ParamsRole);
    }
}

void DeviceControlDialog::delConnect()
{
    auto selIdxs = ui->tvConnections->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        const int rowIdx = selIdxs.at(0).row();
        auto driver = m_model->index(rowIdx, DeviceControlModel::ColDriver).data().toString();
        auto port = m_model->index(rowIdx, DeviceControlModel::ColPort).data().toString();
        if (QMessageBox::question(this,
                                  tr("Предупреждение"),
                                  QString(tr("Удалить подключение %1 (%2)?")).arg(driver).arg(port))
                ==
                QMessageBox::Yes)
        {
            m_model->removeRow(rowIdx);
            static_cast<AAnalyserApplication*>(QApplication::instance())->deleteConnection(rowIdx);
        }
    }
}

void DeviceControlDialog::upPriority()
{
    auto selIdxs = ui->tvConnections->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        const int rowIdx = selIdxs.at(0).row();
        if (rowIdx > 0)
        {
            QList<QStandardItem*> row = m_model->takeRow(rowIdx);
            m_model->insertRow(rowIdx - 1, row);
            ui->tvConnections->setCurrentIndex(m_model->index(rowIdx - 1, 0, selIdxs.at(0).parent()));
            static_cast<AAnalyserApplication*>(QApplication::instance())->moveConnectionUp(rowIdx);
        }
    }
}

void DeviceControlDialog::downPriority()
{
    auto selIdxs = ui->tvConnections->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        const int rowIdx = selIdxs.at(0).row();
        if (rowIdx < m_model->rowCount() - 1)
        {
            QList<QStandardItem*> row = m_model->takeRow(rowIdx);
            m_model->insertRow(rowIdx + 1, row);
            ui->tvConnections->setCurrentIndex(m_model->index(rowIdx + 1, 0, selIdxs.at(0).parent()));
            static_cast<AAnalyserApplication*>(QApplication::instance())->moveConnectionDown(rowIdx);
        }
    }
}

void DeviceControlDialog::on_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_UNUSED(bottomRight);
    QVariant value;
    switch (topLeft.column()) {
    case 0:
        value = topLeft.data(DeviceControlModel::ActiveRole);
        break;
    case 2:
        value = topLeft.data(DeviceControlModel::PortCodeRole);
        break;
    case 3:
        value = topLeft.data();
        break;
    default:
        break;
    }

    static_cast<AAnalyserApplication*>(QApplication::instance())->
            dataChangedConnection(topLeft.row(), topLeft.column(), value);
}


///<=============================================================================================
///< DeviceControlModel

DeviceControlModel::DeviceControlModel(QObject *parent)
    : QStandardItemModel(parent)
{

    setHorizontalHeaderLabels(QStringList() << "Актив." << "Драйвер" << "Порт" << "Комментарий");
}

void DeviceControlModel::load()
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
                                   const DeviceProtocols::Ports port, const QString &comment)
{
    auto *itemActive = new QStandardItem();
    itemActive->setData(active, ActiveRole);

    auto *itemDriver = new QStandardItem(drvName);
    itemDriver->setData(drvUid, DriverUidRole);
    itemDriver->setData(params, ParamsRole);
    itemDriver->setEditable(false);

    auto *itemPort = new QStandardItem(DeviceProtocols::portName(port));
    itemPort->setData(port, PortCodeRole);
    QVariant var;
    QList<DeviceProtocols::Ports> ports = static_cast<AAnalyserApplication*>(QApplication::instance())->getDriverPorts(drvUid);
    var.setValue<QList<DeviceProtocols::Ports>>(ports);
    itemPort->setData(var, PortsByDriverRole);

    auto itemComment = new QStandardItem(comment);

    QStandardItemModel::appendRow(QList<QStandardItem*>() << itemActive << itemDriver << itemPort << itemComment);
}


///<=============================================================================================
///< ComPortDeledate

QWidget *ComPortDeledate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    Q_UNUSED(option);
    QComboBox *editor = new QComboBox(parent);

    QVariant var = index.data(DeviceControlModel::PortsByDriverRole);
    if (var != QVariant())
    {
        QList<DeviceProtocols::Ports> ports = var.value<QList<DeviceProtocols::Ports>>();
        for (int i = 0; i < ports.size(); ++i)
        {
            editor->addItem(DeviceProtocols::portName(ports.at(i)));
            editor->setItemData(i, ports.at(i), ValueIdRole);
        }
    }

    return editor;
}

void ComPortDeledate::setModelData(QWidget *editor,
                                   QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    ComboBoxDelegate::setModelData(editor, model, index);
    QComboBox *edit = static_cast<QComboBox*>(editor);
    model->setData(index, edit->currentData(ValueIdRole), DeviceControlModel::PortCodeRole);
}
