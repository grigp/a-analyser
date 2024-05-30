#include "mwcompetitivestickexecute.h"
#include "ui_mwcompetitivestickexecute.h"

#include <QTimer>

#include "baseutils.h"
#include "driver.h"
#include "driverdefines.h"
#include "aanalyserapplication.h"
#include "testresultdata.h"
#include "executewidget.h"
#include "amessagebox.h"
#include "dynamosignal.h"

MWCompetitiveStickExecute::MWCompetitiveStickExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MWCompetitiveStickExecute)
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    QTimer::singleShot(0, this, &MWCompetitiveStickExecute::start);
    ui->lblCommunicationError->setVisible(false);
    ui->lblErrorInDriver->setVisible(false);

    ui->wgtDynamoOscill->appendArea("", 1);
    ui->frValue->setStyleSheet("background-color: rgb(0, 0, 0);");
    ui->lblValue->setStyleSheet("font-size: 24pt; color: rgb(200, 200, 200);");
    ui->frMaxVal->setStyleSheet("background-color: rgb(0, 0, 0);");
    ui->lblMaxVal->setStyleSheet("font-size: 24pt; color: rgb(200, 200, 200);");

    ui->cbScale->addItems(QStringList() <<  "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128");
}

MWCompetitiveStickExecute::~MWCompetitiveStickExecute()
{
    delete ui;
}

void MWCompetitiveStickExecute::setParams(const QJsonObject &probeParams)
{
    Q_UNUSED(probeParams);
    m_isAutoEnd = probeParams["autoend"].toBool(true);
    auto srl = probeParams["time"].toString("00:00");
    auto trl = QTime::fromString(srl, "mm:ss");
    m_recLength = trl.minute() * 60 + trl.second();

    if (m_isAutoEnd)
        ui->lblRecLenTitle->setText(tr("Длительность записи") + " - " + srl + " " + tr("мм:сс"));
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
            getDriverByFormats(QStringList() << ChannelsDefines::cfSinglePositive);
    if (m_driver)
    {
        m_control = static_cast<DeviceProtocols::TensoControl*>(m_driver->getDeviceControl(DeviceProtocols::uid_TensoControl));
        m_freq = m_driver->frequency(ChannelsDefines::chanTenso1);
        ui->wgtDynamoOscill->setFrequency(m_freq);

        connect(m_driver, &Driver::sendData, this, &MWCompetitiveStickExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &MWCompetitiveStickExecute::on_communicationError);
        connect(m_driver, &Driver::error, this, &MWCompetitiveStickExecute::on_error);

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        ui->lblProbeTitle->setText(mi.name + " - " + m_kard.fio);
        m_trd->newTest(m_kard.uid, mi.uid);

//        if (QApplication::desktop()->screenCount() > 1)
//            showPatientWindow(m_params.at(m_probe).stimulCode);
//        ui->cbScale->setCurrentIndex(m_params.at(m_probe).scale);
        QTimer::singleShot(100, [&]  //! Пока процесс создания не завершен, масштаб отображается некорректно, если просто вызывать
        {
//            scaleChange(ui->cbScale->currentIndex());
        });
        m_trd->newProbe(mi.name);

        m_control->getTensoValueDiapasone(0, m_minVal, m_maxVal);
        ui->lblDiagMin->setText(QString::number(m_minVal, 'f', 0));
        ui->lblDiagMid->setText(QString::number((m_minVal + m_maxVal) / 2, 'f', 0));
        ui->lblDiagMax->setText(QString::number(m_maxVal, 'f', 0));
        ui->pbDiag->setRange(static_cast<int>(m_minVal), static_cast<int>(m_maxVal));
        ui->wgtDynamoOscill->setDiapazone(0, m_minVal, m_maxVal);

        m_driver->start();

    }
    else
    {
        AMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->closeExecutePage(); //showDB();
    }
}

void MWCompetitiveStickExecute::getData(DeviceProtocols::DeviceData *data)
{
    ui->lblCommunicationError->setVisible(false);

    DeviceProtocols::TensoDvcData *dataDyn = static_cast<DeviceProtocols::TensoDvcData*>(data);
    m_value = dataDyn->value(0).toDouble();
    auto sv = QString::number(m_value, 'f', 0);
    ui->lblValue->setText(sv);
    ui->lblDiagValue->setText(sv + " " + tr("кг"));
    ui->pbDiag->setValue(static_cast<int>(m_value));

    QVector<double> recDyn;
    recDyn << m_value;
    ui->wgtDynamoOscill->addValue(recDyn);

    if (m_value > m_valueMax)
    {
        m_valueMax = m_value;
        ui->lblMaxVal->setText(QString::number(m_valueMax, 'f', 1));
    }

//    if (m_patientWin)
//        m_patientWin->setMarker(stabData->x(), stabData->y());

    if (m_isRecording)
    {
        //! Запись
        if (m_dynamo)
            m_dynamo->addValue(m_value);

        ++m_recCount;
        //! Вывод времени теста и прогресса
        if (m_isAutoEnd)
        {
            ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(static_cast<int>(m_recCount / m_freq)) + " / " +
                                   BaseUtils::getTimeBySecCount(m_recLength));
            double mrc = m_recLength * m_freq;
            ui->pbRec->setValue(static_cast<int>(m_recCount / mrc * 100));
        }
        else
            ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(m_recCount / m_freq));

        //! Окончание записи
        if (m_isAutoEnd && (m_recCount >= m_recLength * m_freq))
             finishTest();
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

void MWCompetitiveStickExecute::on_calibrate()
{
    m_control->calibrateTenso(ChannelsDefines::chanMWStickForce);
}

void MWCompetitiveStickExecute::on_resetMax()
{
    m_valueMax = 0;
}

void MWCompetitiveStickExecute::on_scaleChange(int scaleIdx)
{
    int scale = 1;
    for (int i = 0; i < scaleIdx; ++i)
        scale *= 2;

    double maxV = m_maxVal / scale;
    ui->wgtDynamoOscill->setDiapazone(0, m_minVal, maxV);
}

void MWCompetitiveStickExecute::on_record()
{
    m_isRecording = ! m_isRecording;
    if (m_isRecording)
    {
        if (m_isAutoEnd)
        {
            ui->btnRecord->setIcon(QIcon(":/images/SaveNO.png"));
            ui->btnRecord->setText(tr("Прервать"));
        }
        else
        {
            ui->btnRecord->setIcon(QIcon(":/images/SaveOK.png"));
            ui->btnRecord->setText(tr("Завершить"));
        }
        if (!m_dynamo)
            m_dynamo = new DynamoSignal(ChannelsDefines::chanMWStickForce, ui->wgtDynamoOscill->frequency());
    }
    else
    {
        ui->btnRecord->setIcon(QIcon(":/images/Save.png"));
        ui->btnRecord->setText(tr("Запись"));
        if (! m_isAutoEnd)
        {
            finishTest();
        }
        else
        {
            if (m_dynamo)
                m_dynamo->clear();
        }
    }

    ui->pbRec->setValue(0);
    ui->lblRecLen->setText("00:00");
    m_recCount = 0;
}

void MWCompetitiveStickExecute::finishTest()
{
//    hidePatientWindow();
    m_isRecording = false;
    if (m_dynamo)
        m_trd->addChannel(m_dynamo);
    m_trd->saveTest();
    static_cast<ExecuteWidget*>(parent())->closeExecutePage(); //showDB();
}
