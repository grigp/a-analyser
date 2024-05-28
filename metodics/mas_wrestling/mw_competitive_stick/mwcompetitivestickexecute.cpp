#include "mwcompetitivestickexecute.h"
#include "ui_mwcompetitivestickexecute.h"

#include <QTimer>

#include "driver.h"
#include "driverdefines.h"
#include "aanalyserapplication.h"
#include "testresultdata.h"
#include "executewidget.h"
#include "amessagebox.h"

MWCompetitiveStickExecute::MWCompetitiveStickExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MWCompetitiveStickExecute)
{
    ui->setupUi(this);
}

MWCompetitiveStickExecute::~MWCompetitiveStickExecute()
{
    delete ui;
}

void MWCompetitiveStickExecute::setParams(const QJsonObject &probeParams)
{
    Q_UNUSED(probeParams);
}

void MWCompetitiveStickExecute::closeEvent(QCloseEvent *event)
{
    //! Переехало из деструктора. Подозрение на нерегулярный сбой.
    //! Но для срабатывания необходимо перед delete вызывать close();
    if (m_driver)
    {
        m_driver->stop();
        m_driver->deleteLater();
    }
    QWidget::closeEvent(event);
}

void MWCompetitiveStickExecute::timerEvent(QTimerEvent *event)
{
    QWidget::timerEvent(event);
}

void MWCompetitiveStickExecute::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void MWCompetitiveStickExecute::start()
{
    //! Запрашиваем не протокол, а формат канала, тогда будем работать с любыми данными,
    //! представляющими собой точку в декартовых координатах, а не только стабилограмму
    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByFormats(QStringList() << ChannelsDefines::cfDecartCoordinates);
    if (m_driver)
    {
        m_control = static_cast<DeviceProtocols::TensoControl*>(m_driver->getDeviceControl(DeviceProtocols::uid_TensoControl));
        m_freq = m_driver->frequency(ChannelsDefines::chanTenso1);

        connect(m_driver, &Driver::sendData, this, &MWCompetitiveStickExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &MWCompetitiveStickExecute::on_communicationError);
        connect(m_driver, &Driver::error, this, &MWCompetitiveStickExecute::on_error);

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
//        ui->lblProbeTitle->setText(probeParams().name + " - " + m_kard.fio);
        m_trd->newTest(m_kard.uid, mi.uid);

//        if (QApplication::desktop()->screenCount() > 1)
//            showPatientWindow(m_params.at(m_probe).stimulCode);
//        ui->cbScale->setCurrentIndex(m_params.at(m_probe).scale);
        QTimer::singleShot(100, [&]  //! Пока процесс создания не завершен, масштаб отображается некорректно, если просто вызывать
        {
//            scaleChange(ui->cbScale->currentIndex());
        });

        m_driver->start();

    }
    else
    {
        AMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->closeExecutePage(); //showDB();
    }
}

void MWCompetitiveStickExecute::scaleChange(int scaleId)
{

}

void MWCompetitiveStickExecute::getData(DeviceProtocols::DeviceData *data)
{
    ui->lblCommunicationError->setVisible(false);

    DeviceProtocols::TensoDvcData *dataDyn = static_cast<DeviceProtocols::TensoDvcData*>(data);
//    ui->lblX->setText(QString("X = %1").arg(dataDyn->x(), 0, 'f', 2));

//    if (m_patientWin)
//        m_patientWin->setMarker(stabData->x(), stabData->y());

    if (m_isRecording)
    {
        //! Запись, если не задержка привыкания
//            if (m_recCount >= probeParams().latentTime * m_freqStab)
//            {
//                ui->wgtAdvChannels->record(data);
//            }

        ++m_recCount;
        //! Вывод времени теста и прогресса
//        if (true) //probeParams().autoEnd)
//        {
//            ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(static_cast<int>(rc / m_freqStab)) + " / " +
//                                   BaseUtils::getTimeBySecCount(probeParams().time));
//            double mrc = probeParams().time * m_freq;
//            ui->pbRec->setValue(static_cast<int>(rc / mrc * 100));
//        }
//        else
//            ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(m_recCount / m_freqStab));

        //! Смена пробы и окончание
//        if (probeParams().autoEnd && (m_recCount >= (probeParams().time) * m_freq))
//        {
//             finishTest();
//        }
    }
}

void MWCompetitiveStickExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    Q_UNUSED(errorCode);
    ui->lblCommunicationError->setText(QString(tr("Ошибка связи с устройством") + ": %1 (" + tr("порт") + ": %2)").
                                       arg(drvName).arg(port));
    ui->lblCommunicationError->setVisible(true);

    //! Озвучка
    if (m_player.state() != QMediaPlayer::PlayingState)
    {
        m_player.setMedia(QUrl("qrc:/sound/com_error.wav"));
        m_player.play();
    }
}

void MWCompetitiveStickExecute::on_error(const int errorCode)
{
    auto eName = DriverDefines::errorName.value(static_cast<DriverDefines::ErrorCodes>(errorCode));
    ui->lblErrorInDriver->setText(QString(tr("Ошибка в драйвере") + ": %1").arg(eName));
    ui->lblErrorInDriver->setVisible(true);
}

void MWCompetitiveStickExecute::finishTest()
{
//    hidePatientWindow();
    m_isRecording = false;
    m_trd->saveTest();
    static_cast<ExecuteWidget*>(parent())->closeExecutePage(); //showDB();
}
