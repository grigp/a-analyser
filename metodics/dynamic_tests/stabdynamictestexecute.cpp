#include "stabdynamictestexecute.h"
#include "ui_stabdynamictestexecute.h"

#include "testresultdata.h"
#include "aanalyserapplication.h"
#include "driver.h"
#include "channelsutils.h"
#include "settingsprovider.h"
#include "executewidget.h"

#include <QTimer>
#include <QMessageBox>
#include <QDebug>

StabDynamicTestExecute::StabDynamicTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabDynamicTestExecute)
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    ui->lblCommunicationError->setVisible(false);
    QTimer::singleShot(0, this, &StabDynamicTestExecute::start);

    ui->cbScale->addItem("1");
    ui->cbScale->addItem("2");
    ui->cbScale->addItem("4");
    ui->cbScale->addItem("8");
    ui->cbScale->addItem("16");
    ui->cbScale->addItem("32");
    ui->cbScale->addItem("64");
    ui->cbScale->addItem("128");

    ui->wgtAdvChannels->setVisible(false);
    ui->btnCalibrate->setVisible(false);
}

StabDynamicTestExecute::~StabDynamicTestExecute()
{
    delete ui;
}

void StabDynamicTestExecute::setParams(const QJsonObject &params)
{
    Q_UNUSED(params);
}

void StabDynamicTestExecute::closeEvent(QCloseEvent *event)
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

void StabDynamicTestExecute::setTitle(const QString &title)
{
    ui->lblProbeTitle->setText(title);
}

void StabDynamicTestExecute::isShowValues(const bool isShow)
{
    ui->frValues->setVisible(isShow);
}

void StabDynamicTestExecute::isTraceControl(const bool isTrace)
{
    ui->cbShowTrace->setVisible(isTrace);
}

void StabDynamicTestExecute::start()
{
    //! Запрашиваем не протокол, а формат канала, тогда будем работать с любыми данными,
    //! представляющими собой точку в декартовых координатах, а не только стабилограмму
    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByFormats(QStringList() << ChannelsDefines::cfDecartCoordinates);
    if (m_driver)
    {
        m_stabControl = dynamic_cast<DeviceProtocols::StabControl*>(m_driver);
        m_freqStab = m_driver->frequency(ChannelsDefines::chanStab);
        m_freqZ = m_driver->frequency(ChannelsDefines::chanZ);

        connect(m_driver, &Driver::sendData, this, &StabDynamicTestExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &StabDynamicTestExecute::on_communicationError);

        setChannels();

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();
        m_trd->newTest(m_kard.uid, mi.uid);

//        m_patientWinPresent = SettingsProvider::valueFromRegAppCopy("", "PatientWindow", static_cast<QVariant>(true)).toBool();
//        if (m_patientWinPresent && QApplication::desktop()->screenCount() > 1)
//            showPatientWindow(m_params.at(m_probe).stimulCode);
        ui->cbScale->setCurrentIndex(0);

        ui->wgtAdvChannels->assignDriver(m_driver, m_trd);
        //! Стабилограмма будет записана всегда
        ui->wgtAdvChannels->setAllwaysRecordingChannel(ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString());
        auto val = SettingsProvider::valueFromRegAppCopy("AdvancedChannelsWidget", "SplitterProbePosition").toByteArray();
        ui->splitter->restoreState(val);

        m_driver->start();
    }
    else
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->showDB();
    }
}

void StabDynamicTestExecute::getData(DeviceProtocols::DeviceData *data)
{
    //! Любой канал от драйвера надо передавать окну дополнительных каналов
    ui->wgtAdvChannels->getData(data);
//    if (m_isRecording)
//    {
//        //! Запись, если не задержка привыкания
//        if (m_recCount >= probeParams().latentTime * m_freqStab)
//            ui->wgtAdvChannels->record(data);
//    }


    //! Выбранный в переключателе канал, а не просто данные.
    //! Если драйвер будет передавать несколько стабилограмм, то отображать здесь мы будем только одну
    if (ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString() == data->channelId())
    {
        ui->lblCommunicationError->setVisible(false);

        DeviceProtocols::StabDvcData *stabData = static_cast<DeviceProtocols::StabDvcData*>(data);
        ui->lblX->setText(QString("X = %1").arg(stabData->x(), 0, 'f', 2));
        ui->lblY->setText(QString("Y = %1").arg(stabData->y(), 0, 'f', 2));
        ui->lblZ->setText(QString("Z = %1").arg(stabData->z(), 0, 'f', 2));
        ui->wgtSKG->setMarker(stabData->x(), stabData->y());

//        if (m_patientWin)
//            m_patientWin->setMarker(stabData->x(), stabData->y());

//        if (m_isRecording)
//        {
//            ++m_recCount;
//            //! Вывод времени теста и прогресса
//            if (probeParams().autoEnd)
//            {
//                double rc = m_recCount - probeParams().latentTime * m_freqStab;
//                if (rc < 0)
//                    rc = 0;
//                if (m_recCount < probeParams().latentTime * m_freqStab)
//                    ui->lblRecLen->setText(tr("Задержка привыкания"));
//                else
//                    ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(rc / m_freqStab) + " / " +
//                                           BaseUtils::getTimeBySecCount(probeParams().time));
//                double mrc = probeParams().time * m_freqStab;
//                ui->pbRec->setValue(rc / mrc * 100);
//            }
//            else
//                ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(m_recCount / m_freqStab));

//            //! Смена пробы и окончание
//            if (probeParams().autoEnd && (m_recCount >= (probeParams().time + probeParams().latentTime) * m_freqStab))
//            {
//                //! Следующая проба
//                nextProbe();
//                //! Пробы кончились - завершение
//                if (m_probe >= m_params.size())
//                    finishTest();
//            }
//        }
    }
}

void StabDynamicTestExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{

}

void StabDynamicTestExecute::recording()
{

}

void StabDynamicTestExecute::scaleChange(int scaleId)
{

}

void StabDynamicTestExecute::zeroing()
{
    if (m_stabControl)
        m_stabControl->zeroing(ChannelsDefines::chanStab);
}

void StabDynamicTestExecute::calibrate()
{

}

void StabDynamicTestExecute::on_advChannelsClicked(bool checked)
{
    ui->wgtAdvChannels->setVisible(checked);
}

void StabDynamicTestExecute::splitterMoved(int pos, int index)
{

}

void StabDynamicTestExecute::setChannels()
{
    auto listChanUid = m_driver->getChannelsByFormat(ChannelsDefines::cfDecartCoordinates);
    foreach (auto channelUid, listChanUid)
    {
        auto subChanCnt = m_driver->getSubChannelsCount(channelUid);

        for (int i = 0; i < subChanCnt; ++i)
        {
            auto name = ChannelsUtils::instance().channelName(channelUid);
            ui->cbSelectChannel->addItem(name);
            ui->cbSelectChannel->setItemData(ui->cbSelectChannel->count() - 1, channelUid, ChannelsUtils::ChannelUidRole);
            ui->cbSelectChannel->setItemData(ui->cbSelectChannel->count() - 1, i, ChannelsUtils::SubChanNumRole);
        }
    }
    ui->frSelectChannel->setVisible(listChanUid.size() > 1);
}
