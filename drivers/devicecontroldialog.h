#ifndef DEVICECONTROLDIALOG_H
#define DEVICECONTROLDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

#include "checkboxdelegate.h"
#include "comboboxdelegate.h"
#include "deviceprotocols.h"

namespace Ui {
class DeviceControlDialog;
}

/*!
 * \brief Класс модели для списка управления оборудованием DeviceControlModel class
 */
class DeviceControlModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit DeviceControlModel(QObject *parent = nullptr);

    /*!
     * \brief Список колонок
     */
    enum columns
    {
          ColActive = 0
        , ColDriver
        , ColPort
        , ColComment
    };

    /*!
     * \brief Список ролей
     */
    enum DeviceControlRoles
    {
          ActiveRole = Qt::UserRole + 1   ///< Признак активности. bool. В колонке ColActive
        , DriverUidRole                   ///< uid драйвера. QString. В колонке ColDriver
        , ParamsRole                      ///< Параметры. QJsonObject. В колонке ColDriver
        , PortCodeRole                    ///< Код порта. int. В колонке ColPort
        , PortsByDriverRole               ///< Список допустимых портов для драйвера QList<DeviceProtocols::port>. В колонке ColPort
    };

    void load();

    void appendRow(const bool active,
                   const QString &drvUid, const QString &drvName, const QJsonObject &params,
                   const DeviceProtocols::Ports port, //const QList<DeviceProtocols::Ports> ports,
                   const QString &comment);
};

///<=============================================================================================

/*!
 * \brief Класс делегата для поля активности ActiveDelegate class
 */
class ActiveDelegate : public CheckBoxDelegate
{
    Q_OBJECT
public:
    explicit ActiveDelegate(QObject *parent = nullptr)
        : CheckBoxDelegate(parent) {}

protected:
    int role() const override {return DeviceControlModel::ActiveRole;}

};

///<=============================================================================================


class ComPortDeledate : public ComboBoxDelegate
{
public:
    explicit ComPortDeledate(QObject *parent = nullptr)
        : ComboBoxDelegate(parent) {}

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;
};

///<=============================================================================================


/*!
 * \brief Класс окна диалога для управления оборудованием DeviceControlDialog class
 */
class DeviceControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceControlDialog(QWidget *parent = nullptr);
    ~DeviceControlDialog() override;

private slots:
    void addConnect();
    void editConnect();
    void delConnect();

    void upPriority();
    void downPriority();

    void on_doubleClicked(const QModelIndex &index);

    void on_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    Ui::DeviceControlDialog *ui;

    DeviceControlModel *m_model;
};

#endif // DEVICECONTROLDIALOG_H
