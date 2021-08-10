#include "trentetrisexecute.h"
#include "ui_trentetrisexecute.h"

#include <QTimer>
#include <QMessageBox>

#include "aanalyserapplication.h"
#include "driver.h"
#include "executewidget.h"
#include "baseutils.h"
#include "channelsutils.h"
//#include "trentakeputpatientwindow.h"
#include "settingsprovider.h"
#include "testresultdata.h"
#include "trenresultdata.h"
#include "trenresultfactors.h"

TrenTetrisExecute::TrenTetrisExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrenTetrisExecute)
  , m_scene(new QGraphicsScene(-1000, -1000, 2000, 2000))
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    ui->lblCommunicationError->setVisible(false);

    ui->cbScale->addItem("1");
    ui->cbScale->addItem("2");
    ui->cbScale->addItem("4");
    ui->cbScale->addItem("8");
    ui->cbScale->addItem("16");
    ui->cbScale->addItem("32");
    ui->cbScale->addItem("64");
    ui->cbScale->addItem("128");

    ui->wgtAdvChannels->setVisible(false);
}

TrenTetrisExecute::~TrenTetrisExecute()
{
    delete ui;
}

void TrenTetrisExecute::setParams(const QJsonObject &params)
{
    auto scale = params["scale"].toInt();
    ui->cbScale->setCurrentIndex(scale);

    m_recLength = params["time"].toInt();
    auto sRL = BaseUtils::getTimeBySecCount(m_recLength);
    ui->lblRecLenTitle->setText(QString(tr("Длительность записи") + " %1 " + tr("мин:сек")).arg(sRL));

    m_movingMode = TrenTetrisDefines::MovingModeValueIndex.value(params["moving_mode"].toString());
    m_complexityMode = TrenTetrisDefines::ComplexityModeValueIndex.value(params["complexity"].toString());
    m_deletingMode = TrenTetrisDefines::DeletingModeValueIndex.value(params["deleting"].toString());

    m_glassHCount = params["height"].toInt();
    m_glassVCount = params["width"].toInt();

    m_cubeImageFileName = params["cube_image"].toString();

    auto objRdm = params["rows_deleting_mode"].toObject();
    m_cubeColor = BaseUtils::strRGBAToColor(objRdm["cube_color"].toString());
    m_glassColor = BaseUtils::strRGBAToColor(objRdm["glass_color"].toString());
    m_lastColor = BaseUtils::strRGBAToColor(objRdm["last_color"].toString());

    auto objCdm = params["colored_deleting_mode"].toObject();
    m_deletingCubeCount = objCdm["count"].toInt();
    m_colorModeColors.clear();
    auto arrCol = objCdm["colors"].toArray();
    for (int i = 0; i < arrCol.size(); ++i)
    {
        auto obj = arrCol.at(i).toObject();
        QColor color = BaseUtils::strRGBAToColor(obj["color"].toString());
        m_colorModeColors << color;
    }

    auto objAmm = params["auto_moving_mode"].toObject();
    m_autoMovingSpeed = objAmm["speed"].toInt();

    m_markerObj = params["marker"].toObject();
    m_backgroundObj = params["background"].toObject();

    QTimer::singleShot(0, this, &TrenTetrisExecute::start);
}

void TrenTetrisExecute::closeEvent(QCloseEvent *event)
{

}

void TrenTetrisExecute::resizeEvent(QResizeEvent *event)
{

}

void TrenTetrisExecute::start()
{
    m_driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByFormats(QStringList() << ChannelsDefines::cfDecartCoordinates);
    if (m_driver)
    {
        m_dcControl = dynamic_cast<DeviceProtocols::DecartCoordControl*>(m_driver);

        connect(m_driver, &Driver::sendData, this, &TrenTetrisExecute::getData);
        connect(m_driver, &Driver::communicationError, this, &TrenTetrisExecute::on_communicationError);

        m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();
        m_trd->newTest(m_kard.uid, mi.uid);

        //! По формату получаем список каналов этого формата, которые передает драйвер, заносим их в список для выбора
        setChannels();

        auto chanUid = ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString();
        m_frequency = m_driver->frequency(chanUid);

        ui->wgtAdvChannels->assignDriver(m_driver, m_trd);
        //! Стабилограмма будет записана всегда
        ui->wgtAdvChannels->setAllwaysRecordingChannel(ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString());
        auto val = SettingsProvider::valueFromRegAppCopy("AdvancedChannelsWidget", "SplitterProbePosition").toByteArray();
        ui->splitter->restoreState(val);

        m_driver->start();

        showPatientWindow();
        QTimer::singleShot(0, [=]   // Без singleShot другие размеры
        {
            generateNewScene();
        });
    }
    else
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Отсутствует необходимое подключение для работы теста"));
        static_cast<ExecuteWidget*>(parent())->showDB();
    }
}

void TrenTetrisExecute::getData(DeviceProtocols::DeviceData *data)
{
    if (ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString() == data->channelId())
    {
        ui->lblCommunicationError->setVisible(false);

        auto* multiData = static_cast<DeviceProtocols::MultiData*>(data);
        if (multiData->subChanCount() > 0)
        {
            QPointF rec = qvariant_cast<QPointF>(multiData->value(0));
            if (m_marker)
            {
                //! Установка маркера
                double mx = rec.x() / (128 / BaseUtils::scaleMultiplier(ui->cbScale->currentIndex())) * (m_scene->sceneRect().width() / 2);
                double my = - rec.y() / (128 / BaseUtils::scaleMultiplier(ui->cbScale->currentIndex())) * (m_scene->sceneRect().height() / 2);

//                if (mx - m_marker->boundingRect().width() / 2 < m_scene->sceneRect().x() + m_bndLeft * m_propX)
//                    mx = m_scene->sceneRect().x() + m_bndLeft * m_propX + m_marker->boundingRect().width() / 2;
//                if (mx > m_scene->sceneRect().x() + m_scene->sceneRect().width() - m_bndRight * m_propX - m_marker->boundingRect().width() / 2)
//                    mx = m_scene->sceneRect().x() + m_scene->sceneRect().width() - m_bndRight * m_propX - m_marker->boundingRect().width() / 2;
//                if (my - m_marker->boundingRect().height() / 2 < m_scene->sceneRect().y() + m_bndTop * m_propY)
//                    my = m_scene->sceneRect().y() + m_bndTop * m_propY + m_marker->boundingRect().height() / 2;
//                if (my > m_scene->sceneRect().y() + m_scene->sceneRect().height() - m_bndBottom * m_propY - m_marker->boundingRect().height() / 2)
//                    my = m_scene->sceneRect().y() + m_scene->sceneRect().height() - m_bndBottom * m_propY - m_marker->boundingRect().height() / 2;

                m_marker->setPos(mx - m_marker->boundingRect().width() / 2,
                                 my - m_marker->boundingRect().height() / 2);

                ui->wgtAdvChannels->getData(data);

                //! Взаимодействие элементов
                elementsInteraction();

                if (m_isRecording)
                {
                    ++m_recCount;
                    ui->wgtAdvChannels->record(data);
                    ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(m_recCount / m_frequency));
                    ui->pbRec->setValue(100 * m_recCount / (m_recLength * m_frequency));
                    if (m_recCount >= m_recLength * m_frequency)
                    {
                        finishTest();
                        return;
                    }
                }

                m_scene->update(m_scene->sceneRect());
            }
        }
    }
}

void TrenTetrisExecute::on_communicationError(const QString &drvName, const QString &port, const int errorCode)
{
    Q_UNUSED(errorCode);
    ui->lblCommunicationError->setText(QString(tr("Ошибка связи с устройством") + ": %1 (" + tr("порт") + ": %2)").
                                       arg(drvName).arg(port));
    ui->lblCommunicationError->setVisible(true);
}

void TrenTetrisExecute::on_selectChannel(int chanIdx)
{
    Q_UNUSED(chanIdx);
//    qDebug() << chanIdx
//             << ui->cbSelectChannel->itemData(chanIdx, ChannelsUtils::ChannelUidRole)
//             << ui->cbSelectChannel->itemData(chanIdx, ChannelsUtils::SubChanNumRole);
}

void TrenTetrisExecute::on_zeroing()
{
    auto chanUid = ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString();
    if (m_dcControl && chanUid != "")
        m_dcControl->zeroing(chanUid);
}

void TrenTetrisExecute::on_scaleChange(int scaleIdx)
{
    Q_UNUSED(scaleIdx);
}

void TrenTetrisExecute::on_recording()
{
    m_isRecording = ! m_isRecording;

    ui->pbRec->setValue(0);
    ui->lblRecLen->setText("00:00");

    ui->btnZeroing->setEnabled(!m_isRecording);
    ui->frScale->setEnabled(!m_isRecording);
    ui->wgtAdvChannels->enabledControls(!m_isRecording);

    if (m_isRecording)
    {
        ui->btnRecord->setIcon(QIcon(":/images/SaveNO.png"));
        ui->btnRecord->setText(tr("Прервать"));

        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();
        m_trd->newProbe(mi.name);
        ui->wgtAdvChannels->newProbe();
    }
    else
    {
        ui->btnRecord->setIcon(QIcon(":/images/Save.png"));
        ui->btnRecord->setText(tr("Запись"));

        ui->wgtAdvChannels->abortProbe();
    }
    m_recCount = 0;
}

void TrenTetrisExecute::on_advChannelsClicked(bool checked)
{
    ui->wgtAdvChannels->setVisible(checked);
}

void TrenTetrisExecute::generateNewScene()
{

}

void TrenTetrisExecute::setChannels()
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

void TrenTetrisExecute::elementsInteraction()
{

}

void TrenTetrisExecute::showPatientWindow()
{

}

void TrenTetrisExecute::hidePatientWindow()
{

}

void TrenTetrisExecute::finishTest()
{
//    m_isClosed = true;
    doneDriver();
    auto trenRes = new TrenResultData(ChannelsDefines::chanTrenResult);
//    trenRes->addFactor(TrenResultFactorsDefines::ScoreUid, m_score);
//    trenRes->addFactor(TrenResultFactorsDefines::FaultsUid, m_errorsCount);
    m_trd->addChannel(trenRes);

//    hidePatientWindow();
    m_isRecording = false;
    m_trd->saveTest();
    static_cast<ExecuteWidget*>(parent())->showDB();
}

void TrenTetrisExecute::doneDriver()
{
    if (m_driver)
    {
        m_driver->stop();
        m_driver->deleteLater();
    }
}
