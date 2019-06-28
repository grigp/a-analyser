#ifndef DEVICECONTROLDIALOG_H
#define DEVICECONTROLDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

#include "checkboxdelegate.h"

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
    };

    load();

    void appendRow(const bool active,
                   const QString &drvUid, const QString &drvName, const QJsonObject &params,
                   const QString &port, const QString &comment);
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

/*!
 * \brief Класс окна диалога для управления оборудованием DeviceControlDialog class
 */
class DeviceControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceControlDialog(QWidget *parent = 0);
    ~DeviceControlDialog();

private slots:
    void addConnect();
    void editConnect();
    void delConnect();

    void upPriority();
    void downPriority();

private:
    Ui::DeviceControlDialog *ui;

    DeviceControlModel *m_model;
};

#endif // DEVICECONTROLDIALOG_H
