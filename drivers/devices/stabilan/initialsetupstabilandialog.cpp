#include "initialsetupstabilandialog.h"
#include "ui_initialsetupstabilandialog.h"

#include "aanalyserapplication.h"

#include <QDebug>

InitialSetupStabilanDialog::InitialSetupStabilanDialog(DeviceProtocols::Ports port,
                                                       const QJsonObject &params,
                                                       const QString &comment,
                                                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InitialSetupStabilanDialog)
  , m_port(port)
  , m_params(params)
  , m_comment(comment)
{
    ui->setupUi(this);
    ui->frWizard->setVisible(false);
    setStyleSheet(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow()->styleSheet());
    ui->lblMainTitle->setStyleSheet("font-size: 18pt; color: rgb(0, 0, 0)");
    ui->lblInvitationInfo->setStyleSheet("font-size: 14pt;");

    ui->lblMainTitle->setText(tr("Устройство: Стабилан-01.  Порт:") + DeviceProtocols::portName(m_port) + ".  " +
                              tr("Комментарий:") + " " + m_comment);
}

InitialSetupStabilanDialog::~InitialSetupStabilanDialog()
{
    delete ui;
}

void InitialSetupStabilanDialog::on_settingsClicked()
{
    ui->frInvitation->setVisible(false);
    ui->frWizard->setVisible(true);
}

void InitialSetupStabilanDialog::on_nextClicked()
{

}
