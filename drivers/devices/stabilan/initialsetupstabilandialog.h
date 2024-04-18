#ifndef INITIALSETUPSTABILANDIALOG_H
#define INITIALSETUPSTABILANDIALOG_H

#include <QDialog>
#include <QJsonObject>

#include "deviceprotocols.h"

namespace Ui {
class InitialSetupStabilanDialog;
}

/*!
 * \brief Класс диалога начальной настройки стабилоанализатора Стабилан-01 The InitialSetupStabilanDialog class
 */
class InitialSetupStabilanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitialSetupStabilanDialog(DeviceProtocols::Ports port,
                                        const QJsonObject &params,
                                        const QString &comment,
                                        QWidget *parent = nullptr);
    ~InitialSetupStabilanDialog();

private slots:
    void on_settingsClicked();
    void on_nextClicked();

private:
    Ui::InitialSetupStabilanDialog *ui;

    DeviceProtocols::Ports m_port;
    QJsonObject m_params;
    QString m_comment;
};

#endif // INITIALSETUPSTABILANDIALOG_H
