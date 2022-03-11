#include "bedsidescalestesterexecute.h"
#include "ui_bedsidescalestesterexecute.h"

#include "aanalyserapplication.h"
#include "testresultdata.h"
#include "driver.h"
#include "executewidget.h"

#include <QTimer>
#include <QMessageBox>
#include <QDebug>

namespace
{
QList<Oscilloscope*> TesterOscillAreases;
QList<QLabel*> TesterValues;

}

BedsideScalesTesterExecute::BedsideScalesTesterExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BedsideScalesTesterExecute)
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    for (int i = 0; i < 4; ++i)
        ui->wgtOscillApnoe->appendArea(tr("Канал") + " " + QString::number(i + 1), 1);
    ui->wgtOscillTester0->appendArea(tr("Канал") + " 1", 1);
    ui->wgtOscillTester1->appendArea(tr("Канал") + " 2", 1);
    ui->wgtOscillTester2->appendArea(tr("Канал") + " 3", 1);
    ui->wgtOscillTester3->appendArea(tr("Канал") + " 4", 1);
    ui->wgtWeighting->appendArea(tr("Масса"), 1);
    TesterOscillAreases.clear();
    TesterOscillAreases << ui->wgtOscillTester0 << ui->wgtOscillTester1 << ui->wgtOscillTester2 << ui->wgtOscillTester3;
    TesterValues.clear();
    TesterValues << ui->lblValTester0 << ui->lblValTester1 << ui->lblValTester2 << ui->lblValTester3;

    QTimer::singleShot(0, this, &BedsideScalesTesterExecute::start);
    ui->lblCommunicationError->setVisible(false);

    foreach (auto val, TesterValues)
        val->setStyleSheet("font-size: 24pt;");
    ui->lblMassa->setStyleSheet("font-size: 24pt;");
}

BedsideScalesTesterExecute::~BedsideScalesTesterExecute()
{
    delete ui;
}

void BedsideScalesTesterExecute::setParams(const QJsonObject &probeParams)
{
    auto sMode = probeParams["mode"].toString();
    m_mode = BedsideScalesDefines::modeCode.value(sMode);

    ui->frTester->setVisible(m_mode == BedsideScalesDefines::bsmTester);
    ui->frWeighting->setVisible(m_mode == BedsideScalesDefines::bsmScales);
    ui->frApnoe->setVisible(m_mode == BedsideScalesDefines::bsmSleepResearch);
}

void BedsideScalesTesterExecute::closeEvent(QCloseEvent *event)
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

void BedsideScalesTesterExecute::start()
{
    //! Запрашиваем не протокол, а формат канала, тогда будем работать с любыми данными,
    //! представляющими собой точку в декартовых координатах, а не только стабилограмму
    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByFormats(QStringList() << ChannelsDefines::cfSinglePositive);
    if (m_driver)
    {
        m_tenzoControl = dynamic_cast<DeviceProtocols::TensoControl*>(m_driver);
        m_freq = m_driver->frequency(ChannelsDefines::chanWeightPlate);

        connect(m_driver, &Driver::sendData, this, &BedsideScalesTesterExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &BedsideScalesTesterExecute::on_communicationError);

       // setChannels();

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();
//        ui->lblProbeTitle->setText(probeParams().name + " - " + m_kard.fio);
        m_trd->newTest(m_kard.uid, mi.uid);

//        ui->cbScale->setCurrentIndex(m_params.at(m_probe).scale);
//        QTimer::singleShot(100, [&]  //! Пока процесс создания не завершен, масштаб отображается некорректно, если просто вызывать
//        {
//            scaleChange(ui->cbScale->currentIndex());
//        });


        if (m_mode == BedsideScalesDefines::bsmSleepResearch)
            ui->wgtOscillApnoe->setFrequency(m_driver->frequency(ChannelsDefines::chanWeightPlate));
        double mmin = 0;
        double mmax = 0;
        for (int i = 0; i < ui->wgtOscillApnoe->areasesCount(); ++i)
        {
            double min = 0;
            double max = 10;
            if (m_tenzoControl)
                m_tenzoControl->getTensoValueDiapasone(i, min, max);
            mmin += min;
            mmax += max;
            if (m_mode == BedsideScalesDefines::bsmSleepResearch)
                ui->wgtOscillApnoe->setDiapazone(i, min, max);
            if (m_mode == BedsideScalesDefines::bsmTester)
            {
                TesterOscillAreases.at(i)->setDiapazone(0, min, max);
                TesterOscillAreases.at(i)->setFrequency(m_driver->frequency(ChannelsDefines::chanWeightPlate));
            }
        }
        if (m_mode == BedsideScalesDefines::bsmScales)
        {
            ui->wgtWeighting->setDiapazone(0, mmin, mmax);
            ui->wgtWeighting->setFrequency(m_driver->frequency(ChannelsDefines::chanWeightPlate));
        }

        m_driver->start();
    }
    else
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->showDB();
    }
}

void BedsideScalesTesterExecute::scaleChange(int scaleId)
{

}

void BedsideScalesTesterExecute::getData(DeviceProtocols::DeviceData *data)
{
    if (data->channelId() == ChannelsDefines::chanWeightPlate)
    {
        DeviceProtocols::WeightPlateDvcData *wData = static_cast<DeviceProtocols::WeightPlateDvcData*>(data);

        double massa = 0;
        for (int i = 0; i < wData->subChanCount(); ++i)
            massa += wData->value(i).toDouble();

        if (m_mode == BedsideScalesDefines::bsmSleepResearch)
        {
            QVector<double> rec;
            for (int i = 0; i < wData->subChanCount(); ++i)
            {
                rec << wData->value(i).toDouble();
            }
            ui->wgtOscillApnoe->addValue(rec);
        }
        else
        if (m_mode == BedsideScalesDefines::bsmTester)
        {
            for (int i = 0; i < wData->subChanCount(); ++i)
            {
                QVector<double> rec;
                rec << wData->value(i).toDouble();
                TesterOscillAreases.at(i)->addValue(rec);
                TesterValues.at(i)->setText(QString::number(wData->value(i).toDouble()));
            }
        }
        else
        if (m_mode == BedsideScalesDefines::bsmScales)
        {
            QVector<double> rec;
            rec << massa;
            ui->wgtWeighting->addValue(rec);
        }

        ui->lblMassa->setText(QString::number(massa) + tr("кг"));
    }
}

void BedsideScalesTesterExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    Q_UNUSED(errorCode);
    ui->lblCommunicationError->setText(tr("Ошибка связи с устройством:") + " " + drvName + ". " +
                                       tr("Порт:") + " " + port + ". ");
    ui->lblCommunicationError->setVisible(true);
}

void BedsideScalesTesterExecute::calibrate()
{

}

void BedsideScalesTesterExecute::recording()
{

}
