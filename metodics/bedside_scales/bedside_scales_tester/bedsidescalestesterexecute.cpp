#include "bedsidescalestesterexecute.h"
#include "ui_bedsidescalestesterexecute.h"

#include "aanalyserapplication.h"
#include "testresultdata.h"
#include "deviceprotocols.h"
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
        QString s = "";
        for (int i = 0; i < wData->subChanCount(); ++i)
            s += QString::number(wData->value(i).toDouble()) + " ";
        qDebug() << s;
    }
}

void BedsideScalesTesterExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{

}

void BedsideScalesTesterExecute::calibrate()
{

}

void BedsideScalesTesterExecute::recording()
{

}
