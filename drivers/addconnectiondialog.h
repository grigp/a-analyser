#ifndef ADDCONNECTIONDIALOG_H
#define ADDCONNECTIONDIALOG_H

#include "deviceprotocols.h"

#include <QDialog>

namespace Ui {
class AddConnectionDialog;
}

class AddConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddConnectionDialog(QWidget *parent = 0);
    ~AddConnectionDialog();

    /*!
     * \brief Возвращает uid драйвера
     */
    QString driverUid() const;

    /*!
     * \brief Возвращает название драйвера
     */
    QString driverName() const;

    /*!
     * \brief Возвращает порт
     */
    DeviceProtocols::Ports port() const;

    /*!
     * \brief Возвращает комментарий к подключению
     */
    QString comment() const;

private slots:
    void selectDriver(int lineIdx);

private:
    Ui::AddConnectionDialog *ui;

    QMap<QString, QList<DeviceProtocols::Ports>> m_ports;
    void fill();
};

#endif // ADDCONNECTIONDIALOG_H
