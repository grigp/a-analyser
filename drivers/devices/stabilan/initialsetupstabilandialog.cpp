#include "initialsetupstabilandialog.h"
#include "ui_initialsetupstabilandialog.h"

#include "channelsdefines.h"
#include "channelsutils.h"
#include "aanalyserapplication.h"
#include "amessagebox.h"
#include "stabilan01.h"
#include "driverdefines.h"
#include "settingsprovider.h"

#include <QDebug>

namespace  {

static QMap<InitialSetupStabilanDialog::Stages, QString> ButtonTexts =
{
      std::pair<InitialSetupStabilanDialog::Stages, QString> (InitialSetupStabilanDialog::stgCalibrateUp, QApplication::tr("Калибровка через заданное время >>"))
    , std::pair<InitialSetupStabilanDialog::Stages, QString> (InitialSetupStabilanDialog::stgCalibrateDown, QApplication::tr("Калибровка >>"))
    , std::pair<InitialSetupStabilanDialog::Stages, QString> (InitialSetupStabilanDialog::stgFinal, QApplication::tr("Готово"))
};

static const QString StgCommentCalibrUp =
        QApplication::tr("Нажмите кнопку [Калибровка через заданное время >>] \nи поднимите стабилоплатформу над полом");
static const QString StgCommentCalibrDn =
        QApplication::tr("Отрегулируйте ножку до близких значений на реакциях опор \nи нажмите кнопку [Калибровка >>]");
static const QString StgCommentFinal =
        QApplication::tr("Если значения на реакцих опор близки к нулю, \nто калибровка проведена удачно. \nНажмите кнопку [Готово]");

static QMap<InitialSetupStabilanDialog::Stages, QString> StageComment =
{
      std::pair<InitialSetupStabilanDialog::Stages, QString> (InitialSetupStabilanDialog::stgCalibrateUp, StgCommentCalibrUp)
    , std::pair<InitialSetupStabilanDialog::Stages, QString> (InitialSetupStabilanDialog::stgCalibrateDown, StgCommentCalibrDn)
    , std::pair<InitialSetupStabilanDialog::Stages, QString> (InitialSetupStabilanDialog::stgFinal, StgCommentFinal)
};

}

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
    ui->lblInvitationInfo->setStyleSheet("font-size: 18pt;");
    ui->lblWizardInfo->setStyleSheet("font-size: 18pt;");

    ui->lblMainTitle->setText(tr("Устройство: Стабилан-01.  Порт:") + DeviceProtocols::portName(m_port) + ".  " +
                              tr("Комментарий:") + " " + m_comment);
    ui->lblCommunicationError->setStyleSheet("font-size: 18pt; color: rgb(255, 0, 0)");
    ui->lblCommunicationError->setVisible(false);
    ui->lblErrorInDriver->setVisible(false);
    ui->btnBack->setVisible(false);

    auto time = SettingsProvider::valueFromRegAppCopy("InitialSetupStabilanDialog", "TimeBeforeCalibrate", 5).toInt();
    ui->edTimeBeforeCalibrate->setValue(time);
    ui->frSetupSupportResult->setVisible(false);

    ui->lblTimer->setVisible(false);
    ui->lblTimer->setStyleSheet("font-size: 30pt; color: rgb(255, 0, 0)");

    m_maxDiff = m_params["max_different"].toDouble(0.2);
    m_minWeight = m_params["min_weight"].toDouble(0.1);
}

InitialSetupStabilanDialog::~InitialSetupStabilanDialog()
{
    delete ui;
}

void InitialSetupStabilanDialog::timerEvent(QTimerEvent *event)
{
    QDialog::timerEvent(event);
    if (event->timerId() == m_tmCalibrDelay)
    {
        if (m_timerCount > 1)
        {
            --m_timerCount;
            ui->lblTimer->setText(tr("Осталось") + " " + QString::number(m_timerCount) + " " + tr("секунд"));
        }
        else
        {
            if (m_stage == stgCalibrateUp)
                if (m_stabControl)
                    m_stabControl->calibrate(ChannelsDefines::chanStab);

            changeStage(true);
            ui->btnWizardNext->setEnabled(true);
            ui->btnBack->setEnabled(true);

            killTimer(m_tmCalibrDelay);
            m_tmCalibrDelay = -1;
            ui->lblTimer->setVisible(false);
            ui->lblWizardInfo->setVisible(true);
            ui->frWizardTimeBeforeCalibrate->setVisible(true);
        }
    }
}

void InitialSetupStabilanDialog::closeEvent(QCloseEvent *event)
{
    auto mr = AMessageBox::question(this, tr("Предупреждение"), tr("Вы уверены, что хотите прервать юстировку Stabilan-01?"));
    if (mr == AMessageBox::No)
    {
        event->ignore();
        return;
    }

    if (m_driver)
    {
        m_driver->stop();
        m_driver->deleteLater();
    }

    event->accept();
}

void InitialSetupStabilanDialog::on_settingsClicked()
{
    ui->frInvitation->setVisible(false);
    ui->frWizard->setVisible(true);
    m_stage = stgCalibrateUp;
    ui->btnWizardNext->setText(ButtonTexts.value(m_stage));
    ui->lblWizardInfo->setText(StageComment.value(m_stage));

    connectToDevice();
}

void InitialSetupStabilanDialog::on_nextClicked()
{
    if (m_stage == stgFinal)
        accept();
    else
    if (m_stage == stgCalibrateUp)
    {
//        m_tmCalibrDelay = startTimer(1000 * ui->edTimeBeforeCalibrate->value());
        m_tmCalibrDelay = startTimer(1000);
        m_timerCount = ui->edTimeBeforeCalibrate->value();
        ui->btnWizardNext->setEnabled(false);
        ui->btnBack->setEnabled(false);
        ui->lblTimer->setVisible(true);
        ui->lblTimer->setText(tr("Осталось") + " " + QString::number(m_timerCount) + " " + tr("секунд"));
        ui->lblWizardInfo->setVisible(false);
        ui->frWizardTimeBeforeCalibrate->setVisible(false);
    }
    else
    {
        if (m_stage == stgCalibrateDown)
            if (m_stabControl)
                m_stabControl->calibrate(ChannelsDefines::chanStab);
        changeStage(true);
    }
}

void InitialSetupStabilanDialog::on_backClicked()
{
    changeStage(false);
}

void InitialSetupStabilanDialog::on_timeBeforeCalibrateChanged(int value)
{
    SettingsProvider::setValueToRegAppCopy("InitialSetupStabilanDialog", "TimeBeforeCalibrate", value);
}

void InitialSetupStabilanDialog::getData(DeviceProtocols::DeviceData *data)
{
    ui->lblCommunicationError->setVisible(false);

    if (ChannelsUtils::instance().channelType(data->channelId()) == ChannelsDefines::ctStabilogram)
    {
        DeviceProtocols::StabDvcData *stabData = static_cast<DeviceProtocols::StabDvcData*>(data);

        ui->lblReactAValue->setText(QString::number(stabData->a(), 'f', 2));
        ui->lblReactBValue->setText(QString::number(stabData->b(), 'f', 2));
        ui->lblReactCValue->setText(QString::number(stabData->c(), 'f', 2));
        ui->lblReactDValue->setText(QString::number(stabData->d(), 'f', 2));

        auto summ = stabData->a() + stabData->b() + stabData->c() + stabData->d();
        ui->lblSumm->setText(QString(tr("Сумма") + " = %1").arg(QString::number(summ, 'f', 2)));

        ui->pbValueA->setValue(static_cast<int>(stabData->a() / 40.0 * ui->pbValueA->maximum()));
        ui->pbValueB->setValue(static_cast<int>(stabData->b() / 40.0 * ui->pbValueB->maximum()));
        ui->pbValueC->setValue(static_cast<int>(stabData->c() / 40.0 * ui->pbValueC->maximum()));
        ui->pbValueD->setValue(static_cast<int>(stabData->d() / 40.0 * ui->pbValueD->maximum()));

        if (m_stage == stgCalibrateDown)
        {
            double max = 0;
            double maxDiff = 0;
            supportStat(stabData->a(), stabData->b(), stabData->c(), stabData->d(), max, maxDiff);
            if (max > 5 && max < 15)
            {
                if (maxDiff < m_maxDiff)
                {
                    if (!ui->btnWizardNext->isEnabled())
                    {
                        if (m_player.state() != QMediaPlayer::PlayingState)
                        {
                            m_player.setMedia(QUrl("qrc:/sound/ok.wav"));
                            m_player.play();
                        }
                    }
                    ui->lblSetupSupportResult->setText(tr("Разница в значениях усилия в пределах допустимого"));
                    ui->lblSetupSupportResult->setStyleSheet("font-size: 18pt; color: rgb(0, 150, 0)");
                    ui->btnWizardNext->setEnabled(true);
                }
                else
                {
                    if (ui->btnWizardNext->isEnabled())
                    {
                        if (m_player.state() != QMediaPlayer::PlayingState)
                        {
                            m_player.setMedia(QUrl("qrc:/sound/error.wav"));
                            m_player.play();
                        }
                    }
                    ui->lblSetupSupportResult->setText(tr("Разница в значениях усилия больше") + " " + QString::number(m_maxDiff) + " " + tr("кг"));
                    ui->lblSetupSupportResult->setStyleSheet("font-size: 18pt; color: rgb(200, 0, 0)");
                    ui->btnWizardNext->setEnabled(false);
                }
            }
            else
            {
                ui->lblSetupSupportResult->setText(tr("Масса стабилоплатформы некорректна"));
                ui->lblSetupSupportResult->setStyleSheet("font-size: 18pt; color: rgb(200, 0, 0)");
                ui->btnWizardNext->setEnabled(false);
            }
        }
        else
        if (m_stage == stgFinal)
        {
            double max = 0;
            double maxDiff = 0;
            supportStat(stabData->a(), stabData->b(), stabData->c(), stabData->d(), max, maxDiff);
            if (max > -m_minWeight && max < m_minWeight)
            {
                if (!ui->btnWizardNext->isEnabled())
                {
                    if (m_player.state() != QMediaPlayer::PlayingState)
                    {
                        m_player.setMedia(QUrl("qrc:/sound/ok.wav"));
                        m_player.play();
                    }
                }
                ui->lblSetupSupportResult->setText(tr("Масса платформы в пределах допустимого"));
                ui->lblSetupSupportResult->setStyleSheet("font-size: 18pt; color: rgb(0, 150, 0)");
                ui->btnWizardNext->setEnabled(true);
            }
            else
            {
                if (ui->btnWizardNext->isEnabled())
                {
                    if (m_player.state() != QMediaPlayer::PlayingState)
                    {
                        m_player.setMedia(QUrl("qrc:/sound/error.wav"));
                        m_player.play();
                    }
                }
                ui->lblSetupSupportResult->setText(tr("Масса платформы недопустимо отличается от нуля"));
                ui->lblSetupSupportResult->setStyleSheet("font-size: 18pt; color: rgb(200, 0, 0)");
                ui->btnWizardNext->setEnabled(false);
            }
        }
    }
}

void InitialSetupStabilanDialog::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    Q_UNUSED(drvName);
    Q_UNUSED(port);
    Q_UNUSED(errorCode);
    ui->lblCommunicationError->setVisible(true);

}

void InitialSetupStabilanDialog::on_error(const int errorCode)
{
    auto eName = DriverDefines::errorName.value(static_cast<DriverDefines::ErrorCodes>(errorCode));
    ui->lblErrorInDriver->setText(QString(tr("Ошибка в драйвере") + ": %1").arg(eName));
    ui->lblErrorInDriver->setVisible(true);
}

void InitialSetupStabilanDialog::supportStat(const double a, const double b, const double c, const double d, double &max, double &maxDiff)
{
    max = a + b + c + d;
    maxDiff = fabs(b - a);
    if (fabs(c - b) > maxDiff)
        maxDiff = fabs(c - b);
    if (fabs(d - c) > maxDiff)
        maxDiff = fabs(d - c);
    if (fabs(a - d) > maxDiff)
        maxDiff = fabs(a - d);
    if (fabs(a - c) > maxDiff)
        maxDiff = fabs(a - c);
    if (fabs(b - d) > maxDiff)
        maxDiff = fabs(b - d);
}

void InitialSetupStabilanDialog::changeStage(const bool isNext)
{
    int stg = static_cast<int>(m_stage);
    if (isNext)
        ++stg;
    else
        --stg;
    m_stage = static_cast<Stages>(stg);
    ui->btnWizardNext->setText(ButtonTexts.value(m_stage));
    ui->lblWizardInfo->setText(StageComment.value(m_stage));

    ui->frWizardTimeBeforeCalibrate->setVisible(m_stage == stgCalibrateUp);
    ui->frSetupSupportResult->setVisible(m_stage == stgCalibrateDown || m_stage == stgFinal);

    ui->btnBack->setVisible(stg > stgCalibrateUp);
    ui->btnWizardNext->setEnabled(true);
}

void InitialSetupStabilanDialog::connectToDevice()
{
    m_driver = new Stabilan01();
    if (m_driver)
    {
        m_driver->setParams(m_port, m_params);

        m_stabControl = static_cast<DeviceProtocols::StabControl*>(m_driver->getDeviceControl(DeviceProtocols::uid_StabControl));
        connect(m_driver, &Driver::sendData, this, &InitialSetupStabilanDialog::getData);
        connect(m_driver, &Driver::communicationError, this, &InitialSetupStabilanDialog::on_communicationError);
        connect(m_driver, &Driver::error, this, &InitialSetupStabilanDialog::on_error);

        m_driver->start();
    }
}
