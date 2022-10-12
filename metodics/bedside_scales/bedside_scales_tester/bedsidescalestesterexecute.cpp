#include "bedsidescalestesterexecute.h"
#include "ui_bedsidescalestesterexecute.h"

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "testresultdata.h"
#include "driver.h"
#include "executewidget.h"
#include "weightplatesignal.h"
#include "balistogram.h"
#include "weighingresultdata.h"
#include "baseutils.h"

#include <QTimer>
#include <QMessageBox>
#include <QDebug>

namespace
{
QList<Oscilloscope*> TesterOscillAreases;
QList<QLabel*> TesterValues;
QList<QLabel*> TesterADCValues;

}

BedsideScalesTesterExecute::BedsideScalesTesterExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BedsideScalesTesterExecute)
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    for (int i = 0; i < 4; ++i)
        m_offsets << 0;

    setupUI();
    connect(ui->wgtParticalWeighting, &DynamicDiagram::selectItem, this, &BedsideScalesTesterExecute::on_selectItem);

    for (int i = 0; i < m_N; ++i)
        m_fa[i] = 0.0;
    ui->lblMassa1->setVisible(false);
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

    m_isAutoRecord = probeParams["auto_record"].toBool(false);

    if (m_isAutoRecord)
        ui->lblLenRec->setText(tr("До начала записи"));

    auto startTime = probeParams["start_time"].toString("0:00:20");
    m_startTime = QTime::fromString(startTime, "h:mm:ss");

    auto recLength = probeParams["rec_length"].toString("0:10:00");
    m_recLength = QTime::fromString(recLength, "h:mm:ss");

    m_isSound = probeParams["sound"].toBool(true);

    m_isIntervalScaling = probeParams["is_interval_scaling"].toBool(true);

    auto scalingInterval = probeParams["scaling_interval"].toString("05:00");
    m_scalingInterval = QTime::fromString(scalingInterval, "mm:ss");

    m_averageTime = probeParams["average_time"].toInt(3);

    m_isSignalsRecord = probeParams["is_signals_record"].toBool(true);
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

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
//        ui->lblProbeTitle->setText(probeParams().name + " - " + m_kard.fio);
        m_trd->newTest(m_kard.uid, mi.uid);
        m_trd->newProbe(mi.name);

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

        if (m_mode == BedsideScalesDefines::bsmSleepResearch || m_mode == BedsideScalesDefines::bsmTester)
        {
            m_wps = new WeightPlateSignal(ChannelsDefines::chanWeightPlate, m_driver->frequency(ChannelsDefines::chanWeightPlate), 4);
            m_trd->addChannel(m_wps);
        }
        else
        if (m_mode == BedsideScalesDefines::bsmScales)
        {
            if ((m_isIntervalScaling && m_isSignalsRecord) || (!m_isIntervalScaling))
            {
                m_weight = new Balistogram(ChannelsDefines::chanWeight, m_driver->frequency(ChannelsDefines::chanWeightPlate));
                m_trd->addChannel(m_weight);
            }
            if (m_isIntervalScaling)
            {
                m_wrd = new WeighingResultData(ChannelsDefines::chanParticalWeighting);
                m_wrd->setAverageTime(m_averageTime);
                m_wrd->setScalingInterval(m_scalingInterval);
                m_trd->addChannel(m_wrd);
                m_scalingIntervalPt = (m_scalingInterval.minute() * 60 + m_scalingInterval.second()) *
                        m_driver->frequency(ChannelsDefines::chanWeightPlate);
                m_averageTimePt = m_averageTime * m_driver->frequency(ChannelsDefines::chanWeightPlate);
            }
        }

        m_isCalibrated = m_tenzoControl->isCalibrated();
    }
    else
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->showDB();
    }
}

//void BedsideScalesTesterExecute::on_scaleChange(int scaleId)
//{
//    int scale = 1;
//    for (int i = 0; i < scaleId; ++i)
//        scale *= 2;

//    double min = 0;
//    double max0 = 50;
//    double max1 = 50;
//    double max2 = 50;
//    double max3 = 50;
//    if (m_tenzoControl)
//    {
//        m_tenzoControl->getTensoValueDiapasone(0, min, max0);
//        m_tenzoControl->getTensoValueDiapasone(1, min, max1);
//        m_tenzoControl->getTensoValueDiapasone(2, min, max2);
//        m_tenzoControl->getTensoValueDiapasone(3, min, max3);
//    }

//    if (m_mode == BedsideScalesDefines::bsmScales)
//    {
//        ui->wgtWeighting->setDiapazone(0, 0, (max0 + max1 + max2 + max3) / scale);
//    }
//    else
//    if (m_mode == BedsideScalesDefines::bsmTester)
//    {
//        ui->wgtOscillTester0->setDiapazone(0, 0, max0 / scale);
//        ui->wgtOscillTester1->setDiapazone(0, 0, max0 / scale);
//        ui->wgtOscillTester2->setDiapazone(0, 0, max0 / scale);
//        ui->wgtOscillTester3->setDiapazone(0, 0, max0 / scale);
//    }
//    if (m_mode == BedsideScalesDefines::bsmSleepResearch)
//    {
//        ui->wgtOscillApnoe->setDiapazone(0, min, max0 / scale);
//        ui->wgtOscillApnoe->setDiapazone(1, min, max1 / scale);
//        ui->wgtOscillApnoe->setDiapazone(2, min, max2 / scale);
//        ui->wgtOscillApnoe->setDiapazone(3, min, max3 / scale);
//    }
//}

void BedsideScalesTesterExecute::on_scaleChange(int scaleId)
{
    m_curScaleId = scaleId;
    int scale = 1;
    for (int i = 0; i < scaleId; ++i)
        scale *= 2;

    double min = 0;
    double max0 = 50;
    double max1 = 50;
    double max2 = 50;
    double max3 = 50;
    if (m_tenzoControl)
    {
        m_tenzoControl->getTensoValueDiapasone(0, min, max0);
        m_tenzoControl->getTensoValueDiapasone(1, min, max1);
        m_tenzoControl->getTensoValueDiapasone(2, min, max2);
        m_tenzoControl->getTensoValueDiapasone(3, min, max3);
    }

    auto setDiapazone = [&](Oscilloscope *area, double offset, double diap, int idx = 0)
    {
        double min = offset - diap / 2;
        double max = offset + diap / 2;
        if (min < 0)
        {
            min = 0;
            max = min + diap;
        }
        area->setDiapazone(idx, min, max);
    };


    if (m_mode == BedsideScalesDefines::bsmScales)
    {
        double diap = (max0 + max1 + max2 + max3) / scale;
        double offs = 0;
        foreach (auto ofs, m_offsets)
            offs += ofs;
        setDiapazone(ui->wgtWeighting, offs, diap);
    }
    else
    if (m_mode == BedsideScalesDefines::bsmTester)
    {
        setDiapazone(ui->wgtOscillTester0, m_offsets.at(0), (max0 / scale));
        setDiapazone(ui->wgtOscillTester1, m_offsets.at(1), (max1 / scale));
        setDiapazone(ui->wgtOscillTester2, m_offsets.at(2), (max2 / scale));
        setDiapazone(ui->wgtOscillTester3, m_offsets.at(3), (max3 / scale));
    }
    if (m_mode == BedsideScalesDefines::bsmSleepResearch)
    {
        setDiapazone(ui->wgtOscillApnoe, m_offsets.at(0), (max0 / scale), 0);
        setDiapazone(ui->wgtOscillApnoe, m_offsets.at(1), (max0 / scale), 1);
        setDiapazone(ui->wgtOscillApnoe, m_offsets.at(2), (max0 / scale), 2);
        setDiapazone(ui->wgtOscillApnoe, m_offsets.at(3), (max0 / scale), 3);
    }
}

void BedsideScalesTesterExecute::on_zeroing()
{
    for (int i = 0; i < m_offsets.size(); ++i)
    {
        auto v = m_offsets.at(i);
        v = 0;
        m_offsets.replace(i, v);
    }
    foreach (auto rec, m_values)
    {
        for (int i = 0; i < rec.size(); ++i)
        {
            if (i < m_offsets.size())
            {
                auto v = m_offsets.at(i);
                v += rec.at(i);
                m_offsets.replace(i, v);
            }
        }
    }
    for (int i = 0; i < m_offsets.size(); ++i)
    {
        auto v = m_offsets.at(i);
        v /= m_values.size();
        m_offsets.replace(i, v);
    }

    on_scaleChange(m_curScaleId);
}

void BedsideScalesTesterExecute::getData(DeviceProtocols::DeviceData *data)
{
    if (data->channelId() == ChannelsDefines::chanWeightPlate)
        getDataWeight(data);
    else
    if (data->channelId() == ChannelsDefines::chanADCValues)
        getDataADC(data);
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
    if (m_tenzoControl)
    {
        m_tenzoControl->calibrateTenso(ChannelsDefines::chanWeightPlate);
        m_isCalibrated = true;
    }
}

void BedsideScalesTesterExecute::recording()
{
    ui->lblLenTime->setText(BaseUtils::getTimeBySecCount(0));
    if (m_isCalibrated)
    {
        m_isRecording = ! m_isRecording;
        m_recCounter = 0;

        if (m_isRecording)
        {
            ui->btnRecord->setIcon(QIcon(":/images/SaveOK.png"));
            ui->btnRecord->setText(tr("Завершить запись"));
        }
        else
        {
            ui->btnRecord->setIcon(QIcon(":/images/Save.png"));
            ui->btnRecord->setText(tr("Запись"));

            m_trd->saveTest();
            static_cast<ExecuteWidget*>(parent())->showDB();
        }
    }
    else
    {
        m_isAutoRecord = false;
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Сначала необходимо откалибровать устройство"));
    }
}

void BedsideScalesTesterExecute::on_selectItem(const int idx)
{
    if (idx >= 0 && idx < ui->wgtParticalWeighting->size())
    {
        auto value = ui->wgtParticalWeighting->value(idx);
        ui->wgtParticalWeighting->setBottomText(tr("Измерение") + (" (") + QString::number(idx + 1) + ") : " +
                                              QString::number(value) + " " + tr("кг"));
    }
}

void BedsideScalesTesterExecute::setupUI()
{
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
    TesterADCValues.clear();
    TesterADCValues << ui->lblValADCTester0 << ui->lblValADCTester1 << ui->lblValADCTester2 << ui->lblValADCTester3;

    QTimer::singleShot(0, this, &BedsideScalesTesterExecute::start);
    ui->lblCommunicationError->setVisible(false);

    foreach (auto val, TesterValues)
        val->setStyleSheet("font-size: 24pt;");
    ui->lblMassa->setStyleSheet("font-size: 24pt;");

    ui->cbScale->addItems(QStringList() << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128");

    ui->lblLenRec->setStyleSheet("font-size: 24pt; color: rgb(230, 230, 0);");
    ui->lblLenTime->setStyleSheet("font-size: 24pt; color: rgb(230, 230, 0);");
    ui->frLenRecTimer->setStyleSheet("background-color: rgb(20, 20, 0);");
}

void BedsideScalesTesterExecute::getDataWeight(DeviceProtocols::DeviceData *data)
{
    DeviceProtocols::WeightPlateDvcData *wData = static_cast<DeviceProtocols::WeightPlateDvcData*>(data);

    double massa = 0;
    QVector<double> recS;
    for (int i = 0; i < wData->subChanCount(); ++i)
    {
        massa += wData->value(i).toDouble();
        recS << wData->value(i).toDouble();
    }

    m_values.enqueue(recS);
    if (m_values.size() == 100)
        m_values.dequeue();


    if (m_mode == BedsideScalesDefines::bsmSleepResearch)
    {
        QVector<double> rec;
        for (int i = 0; i < wData->subChanCount(); ++i)
        {
            rec << wData->value(i).toDouble();
        }
        ui->wgtOscillApnoe->addValue(rec);

        if (m_isRecording)
            m_wps->addValue(rec);
    }
    else
    if (m_mode == BedsideScalesDefines::bsmTester)
    {
        QVector<double> recW;
        for (int i = 0; i < wData->subChanCount(); ++i)
        {
            QVector<double> rec;
            rec << wData->value(i).toDouble();
            recW << wData->value(i).toDouble();
            TesterOscillAreases.at(i)->addValue(rec);
            TesterValues.at(i)->setText(QString::number(wData->value(i).toDouble(), 'f', 3));
        }

        if (m_isRecording)
            m_wps->addValue(recW);
    }
    else
    if (m_mode == BedsideScalesDefines::bsmScales)
    {
        QVector<double> rec;
        rec << massa;
        ui->wgtWeighting->addValue(rec);
        if (m_isRecording)
        {
            if ((m_isIntervalScaling && m_isSignalsRecord) || (!m_isIntervalScaling))
                m_weight->addValue(massa);
            if (m_isIntervalScaling)
            {
                ++m_pwmCounter;
                if (m_pwm == pwmWait)
                {
                    if (m_pwmCounter >= m_scalingIntervalPt)
                    {
                        m_pwm = pwmAveraging;
                        m_pwmCounter = 0;
                    }
                }
                else
                if (m_pwm == pwmAveraging)
                {
                    if (m_pwmCounter < m_averageTimePt)
                    {
                        //m_pwmWeight_ += massa;
                        m_pwmWeight << massa;
                    }
                    else
                    {
                        m_pwm = pwmWait;
                        m_pwmCounter = 0;
                        double value = computeMassaAverage(); //m_pwmWeight_ / m_averageTimePt;
                        writeToFile(value);

                        m_wrd->addWeight(value, QDateTime::currentDateTime(), m_recCounter - m_pwmWeight.size(), m_recCounter);
                        ++m_partWeightCount;
                        ui->wgtParticalWeighting->appendItem(new DiagItem(value, QString::number(m_partWeightCount)));
                        ui->wgtParticalWeighting->setKind(DynamicDiagram::KindBar);
                        ui->wgtParticalWeighting->setVolume(DynamicDiagram::Volume3D);
                        //m_pwmWeight_ = 0;
                        m_pwmWeight.clear();
                    }
                }
            }
        }
    }

    //! Если установлено автоматическое управление записью, то начать запись
    ++m_counter;
    if (!m_isRecording && m_isAutoRecord)
    {
        int secCount =  abs(m_startTime.secsTo(QTime(0, 0))) -
                m_counter / m_driver->frequency(ChannelsDefines::chanWeightPlate);
        ui->lblLenTime->setText(BaseUtils::getTimeBySecCount(secCount));

        if (m_counter >= abs(m_startTime.secsTo(QTime(0, 0)) * m_driver->frequency(ChannelsDefines::chanWeightPlate)))
        {
            recording();
            if (m_isSound)
            {
                m_player.setMedia(QUrl("qrc:/sound/03.wav"));
                m_player.play();
            }
            ui->lblLenRec->setText(tr("Время записи"));

        }
    }

    m_mv = m_mv + (massa - m_fa[m_n]);
    m_fa[m_n] = massa;
    m_n = (m_n + 1) % m_N;

    ui->lblMassa->setText(QString::number(m_mv / m_N, 'f', 3) + " " + tr("кг"));
    ui->lblMassa1->setText(QString::number(massa, 'f', 3) + " " + tr("кг"));
    if (m_isRecording)
    {
        ++m_recCounter;
        ui->lblLenTime->setText(BaseUtils::getTimeBySecCount(m_recCounter / m_driver->frequency(ChannelsDefines::chanWeightPlate)));
    }

    if (m_isRecording)
    {
        if (m_isAutoRecord && m_isSound &&
                m_recCounter ==
                (abs(m_recLength.secsTo(QTime(0, 0))) - 2) * m_driver->frequency(ChannelsDefines::chanWeightPlate))
        {
            m_player.setMedia(QUrl("qrc:/sound/05.wav"));
            m_player.play();
        }
        //! Если установлено автоматическое управление записью, то завершить запись
        if (m_isAutoRecord &&
                m_recCounter >= abs(m_recLength.secsTo(QTime(0, 0))) * m_driver->frequency(ChannelsDefines::chanWeightPlate))
            recording();
    }
}

void BedsideScalesTesterExecute::getDataADC(DeviceProtocols::DeviceData *data)
{
    DeviceProtocols::ADCValuesDvcData *adcData = static_cast<DeviceProtocols::ADCValuesDvcData*>(data);

    if (m_mode == BedsideScalesDefines::bsmTester)
    {
        for (int i = 0; i < adcData->subChanCount(); ++i)
        {
            quint16 v = static_cast<quint16>(adcData->value(i).toUInt());
            QString s = QString::number(v, 2);
            QString sz = "";
            for (int i = 0; i < 16 - s.size(); ++i)
                sz += "0";
            TesterADCValues.at(i)->setText(sz + s);
        }
    }
}

double BedsideScalesTesterExecute::computeMassaAverage()
{
    double mid = 0;
    double q = 0;
    double min = INT_MAX;
    double max = -INT_MAX;

    for (int i = 0; i < m_pwmWeight.size(); ++i)
    {
        auto val = m_pwmWeight.at(i);
        mid += val;
        if (val < min) min = val;
        if (val > max) max = val;
    }
    mid /= m_pwmWeight.size();

    for (int i = 0; i < m_pwmWeight.size(); ++i)
    {
        auto val = m_pwmWeight.at(i);
        q += pow(val - mid, 2) / m_pwmWeight.size();
    }
    q = sqrt(q);

    double retval = 0;
    int n = 0;
    for (int i = 0; i < m_pwmWeight.size(); ++i)
    {
        auto val = m_pwmWeight.at(i);
        if (val >= mid - q && val <= mid + q)
        {
            retval += val;
            ++n;
        }
    }
    retval /= n;

    qDebug() << min << "   " << mid - q << mid << mid + q << "   " << max << "  |  "
             << n << m_pwmWeight.size() << static_cast<double>(n)/m_pwmWeight.size();

    return retval;
}

void BedsideScalesTesterExecute::writeToFile(const double massa)
{
    QString path = DataDefines::aanalyserDocumentsPath();

    QFile file(path + "weight.txt");
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        QString line = QString::number(massa) + "\n";
        out << line;
    }
    file.close();
}

