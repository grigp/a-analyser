#include "bedsidescalestesterexecute.h"
#include "ui_bedsidescalestesterexecute.h"

#include "aanalyserapplication.h"
#include "testresultdata.h"
#include "driver.h"
#include "executewidget.h"

#include <QTimer>
#include <QMessageBox>
#include <QDebug>

BedsideScalesTesterExecute::BedsideScalesTesterExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BedsideScalesTesterExecute)
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    QTimer::singleShot(0, this, &BedsideScalesTesterExecute::start);
    ui->lblCommunicationError->setVisible(false);

    for (int i = 0; i < 4; ++i)
        ui->wgtAreaGraph->appendArea(tr("Канал") + " " + QString::number(i + 1), 1);
}

BedsideScalesTesterExecute::~BedsideScalesTesterExecute()
{
    delete ui;
}

void BedsideScalesTesterExecute::setParams(const QJsonObject &probeParams)
{

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

        ui->wgtAreaGraph->setFrequency(m_driver->frequency(ChannelsDefines::chanWeightPlate));
        for (int i = 0; i < ui->wgtAreaGraph->areasesCount(); ++i)
        {
            double min = 0;
            double max = 10;
            if (m_tenzoControl)
                m_tenzoControl->getTensoValueDiapasone(i, min, max);
            ui->wgtAreaGraph->setDiapazone(i, min, max);
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
        QVector<double> rec;
        for (int i = 0; i < wData->subChanCount(); ++i)
        {
//            s += QString::number(wData->value(i).toDouble()) + " ";
            rec << wData->value(i).toDouble();
        }
        ui->wgtAreaGraph->addValue(rec);
    }
}

void BedsideScalesTesterExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
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
