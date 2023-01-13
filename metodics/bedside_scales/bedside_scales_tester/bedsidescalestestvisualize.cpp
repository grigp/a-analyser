#include "bedsidescalestestvisualize.h"
#include "ui_bedsidescalestestvisualize.h"

#include <QFileDialog>
#include <QTextStream>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QHttpMultiPart>
#include <QDebug>

#include "bedsidescalestestcalculator.h"
#include "baseutils.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "authorizationdialog.h"

namespace
{
static const QString DeviceDataType = "Type1";//"bed_side_weight";

}

BedsideScalesTestVisualize::BedsideScalesTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BedsideScalesTestVisualize)
{
    ui->setupUi(this);

    connect(ui->wgtWeighingResults, &DynamicDiagram::selectItem, this, &BedsideScalesTestVisualize::on_selectItem);
}

BedsideScalesTestVisualize::~BedsideScalesTestVisualize()
{
    delete ui;
}

void BedsideScalesTestVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_testUid = testUid;
        m_calculator = new BedsideScalesTestCalculator(testUid, this);
        m_calculator->calculate();

        int secCnt = m_calculator->signalSize() / m_calculator->signalFrequency();
        auto sSize = BaseUtils::getTimeBySecCount(secCnt, true);
        ui->lblSignalSize->setText(tr("Длительность сигнала") + " : " + sSize);

        ui->frWeighingResults->setVisible(m_calculator->isParticalWeighting());

        showDiagParticalWeighting();
    }
}

void BedsideScalesTestVisualize::print(QPrinter *printer, const QString &testUid)
{
    Q_UNUSED(printer);
    Q_UNUSED(testUid);
}

void BedsideScalesTestVisualize::signalExport()
{
    if (m_calculator)
    {
        QString path = DataDefines::aanalyserDocumentsPath();
        auto fileName = QFileDialog::getSaveFileName(this, tr("Файл для экспорта сигнала"), path, tr("Текстовые файлы (*.txt)"));
        if (fileName != "")
            saveSignal(fileName);
    }
}

void BedsideScalesTestVisualize::on_weighingResultsGraph()
{
    ui->wgtWeighingResults->setKind(DynamicDiagram::KindGraph);
    ui->btnBar->setChecked(false);
}

void BedsideScalesTestVisualize::on_weighingResultsBar()
{
    ui->wgtWeighingResults->setKind(DynamicDiagram::KindBar);
    ui->btnGraph->setChecked(false);
}

void BedsideScalesTestVisualize::on_weighingResults3D(bool is3D)
{
    if (is3D)
        ui->wgtWeighingResults->setVolume(DynamicDiagram::Volume3D);
    else
        ui->wgtWeighingResults->setVolume(DynamicDiagram::Volume2D);
}

int n {0};

void BedsideScalesTestVisualize::networkRequest(QNetworkAccessManager *mgr, const QUrl url, QJsonObject obj)
{
    qDebug() << ++n << ") ------";
    qDebug() << "Запрос :" << url;
    qDebug() << "Данные формы :" << obj;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    m_reply = mgr->post(request, data);

    connect(m_reply, &QNetworkReply::finished, this, &BedsideScalesTestVisualize::networkAnswer);
}

void BedsideScalesTestVisualize::networkAnswer()
{
    if (m_reply)
    {
        bool ar = m_reply->error() == QNetworkReply::NoError;
        if(ar)
        {
            m_answer = QString::fromUtf8(m_reply->readAll());
        }
        else
        {
            m_answer = m_reply->errorString();
            qDebug() << "---- error ----" << QString::fromUtf8(m_reply->readAll());
        }
        qDebug() << "Ответ :" << m_answer;
        m_reply->deleteLater();

        if (ar)
        {
            if (m_netWebSendStage == nwsRegUser)
            {
                m_userId = m_answer;
                networkGetListDevice();
            }
            else
            if (m_netWebSendStage == nwsGetListDevice)
            {
                if (!isDeviceOnList(m_answer))
                    networkAddDevice(m_userId);
                else
                    networkGetTimeMarker();
            }
            else
            if (m_netWebSendStage == nwsAddDevice)
            {
                m_deviceId = m_answer;
                networkGetTimeMarker();
            }
            else
            if (m_netWebSendStage == nwsTimeMarker)
            {
                m_timeMarker = m_answer;
                networkSendData();
            }
        }
    }
}

bool BedsideScalesTestVisualize::isDeviceOnList(const QString list)
{
    QJsonDocument doc = QJsonDocument::fromJson("{\"devices\":" + list.toUtf8() + "}");
    QJsonObject obj = doc.object();
    QJsonArray arr = obj["devices"].toArray();
    for (int i = 0; i < arr.size(); ++i)
    {
        QJsonObject obj = arr.at(i).toObject();
        if (obj["type"].toString() == DeviceDataType)
        {
            m_deviceId = obj["address"].toString();
            return true;
        }
    }
    return false;
}

void BedsideScalesTestVisualize::networkRegisterUser(const QString userUid)
{
    m_netWebSendStage = nwsRegUser;
    QJsonObject obj;
    obj["username"] = "grig_p@mail.ru";
    obj["password"] = "MVSfWhPb5HqWvj2";
    obj["publickey"] = userUid;
    networkRequest(m_netManager, QUrl(QStringLiteral("https://med-api.nordavind.ru/api/protocol/user/register")), obj);
}

void BedsideScalesTestVisualize::networkAddDevice(const QString userId)
{
    m_netWebSendStage = nwsAddDevice;
    QJsonObject obj;
    obj["user"] = userId;
    obj["publickey"] = "04f4c453191f38cf67c8c32472fc36196848bc1d3b61e2e2ebdbc85082d4ec830724b3dd79a352e2b97b56156e338a798c51003001fa18d4ec2da62c1333d470a9";
    obj["type"] = DeviceDataType;
    networkRequest(m_netManager, QUrl(QStringLiteral("https://med-api.nordavind.ru/api/protocol/medicalDevice/register")), obj);
}

void BedsideScalesTestVisualize::networkGetListDevice()
{
    m_netWebSendStage = nwsGetListDevice;
//    QJsonObject obj;
//    obj["userAddress"] = m_userId;
    networkRequest(m_netManager, QUrl(QStringLiteral("https://med-api.nordavind.ru/api/protocol/medicalDevice/getList?userAddress=") + m_userId), QJsonObject());
}

void BedsideScalesTestVisualize::networkGetTimeMarker()
{
    m_netWebSendStage = nwsTimeMarker;
    //QJsonObject obj;
    networkRequest(m_netManager, QUrl(QStringLiteral("https://med-api.nordavind.ru/api/protocol/getTimeMarker?user=") + m_userId), QJsonObject());
}

void BedsideScalesTestVisualize::networkSendData()
{
    m_netWebSendStage = nwsSendData;

      QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

      QString body = "User=" + m_userId;
      QHttpPart tpUser;
      tpUser.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"text\""));
      tpUser.setBody(body.toUtf8());

      body = "Device.Address=" + m_deviceId;
      QHttpPart tpDvcAddr;
      tpDvcAddr.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"text\""));
      tpDvcAddr.setBody(body.toUtf8());

      body = "Device.Type=" + DeviceDataType;
      QHttpPart tpDvcType;
      tpDvcType.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"text\""));
      tpDvcType.setBody(body.toUtf8());

      body = "TimeMarker=" + m_timeMarker;
      QHttpPart tpTimeMarker;
      tpTimeMarker.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"text\""));
      tpTimeMarker.setBody(body.toUtf8());

      QJsonArray data;
      for (int i = 0; i < m_calculator->size(); ++i)
      {
          auto value = m_calculator->weight(i);
          auto dt = m_calculator->dateTime(i);
          QJsonObject valObj;
          valObj["massa"] = value;
          valObj["date_time"] = dt.toString("dd:MM:yyyy hh.mm.ss");

          data << valObj;
      }
      QJsonDocument doc(data);
      QByteArray ba = doc.toJson();
      qDebug() << ba;
      QHttpPart tpData;
      tpData.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\""));
      tpData.setBody(ba);

      multiPart->append(tpUser);
      multiPart->append(tpDvcAddr);
      multiPart->append(tpDvcType);
      multiPart->append(tpTimeMarker);
      multiPart->append(tpData);

      QNetworkRequest request(QUrl(QStringLiteral("https://med-api.nordavind.ru/api/protocol/request")));
//      request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

      m_reply = m_netManager->post(request, multiPart);
      multiPart->setParent(m_reply);

      connect(m_reply, &QNetworkReply::finished, this, &BedsideScalesTestVisualize::networkAnswer);
}

void BedsideScalesTestVisualize::on_sendToWeb()
{
    AuthorizationDialog dialog(this);
    dialog.setTitle("Введите регистрационые данные");
    dialog.setInfo("Необходимо ввести регистрационые данные, адрес электронной почты и пароль,\nс которыми Вы регистрировались на сайте HealthLine (https://med.nordavind.ru/)");
    dialog.setTextName("Адрес электронной почты :");
    dialog.setTextPassword("Пароль :");
    if (dialog.exec() == QDialog::Accepted)
    {
        //sendToWeb();
    }
}

void BedsideScalesTestVisualize::on_selectItem(const int idx)
{
    if (idx >= 0 && idx < m_calculator->size())
    {
        auto value = m_calculator->weight(idx);
        auto dt = m_calculator->dateTime(idx);
        ui->wgtWeighingResults->setBottomText(tr("Измерение") + (" (") + QString::number(idx + 1) + ") : (" +
                                              dt.toString("dd.MM.yyyy hh:mm:ss") + ") - " +
                                              QString::number(value) + " " + tr("кг"));
//        ui->wgtWeighingResults->setBottomText(QString(tr("Тест") + " " + sdt + " - " + QString::number(value)));
    }
}

void BedsideScalesTestVisualize::sendToWeb()
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(m_testUid, ti))
    {
        DataDefines::PatientKard patient;
        if (DataProvider::getPatient(ti.patientUid, patient))
        {
            m_netManager = new QNetworkAccessManager(this);
            networkRegisterUser(BaseUtils::removeSignesFromUuid(ti.patientUid));

//            const QUrl url(QStringLiteral("https://med-api.nordavind.ru/api/protocol/user/register"));
//            QNetworkRequest request(url);
//            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//            QJsonObject obj;
//            obj["username"] = "grig_p@mail.ru";
//            obj["password"] = "MVSfWhPb5HqWvj2";
//            obj["publickey"] = BaseUtils::removeSignesFromUuid(ti.patientUid);
//            QJsonDocument doc(obj);
//            QByteArray data = doc.toJson();
//            qDebug() << data;
//            // or
//            // QByteArray data("{\"key1\":\"value1\",\"key2\":\"value2\"}");
//            QNetworkReply *reply = mgr->post(request, data);

//            QObject::connect(reply, &QNetworkReply::finished, [=]()
//            {
//                if(reply->error() == QNetworkReply::NoError)
//                {
//                    QString contents = QString::fromUtf8(reply->readAll());
//                    qDebug() << contents;
//                }
//                else
//                {
//                    QString err = reply->errorString();
//                    qDebug() << err;
//                }
//                reply->deleteLater();
//            });


//            QNetworkAccessManager *manager = new QNetworkAccessManager(this);

//            QNetworkRequest request(QUrl("https://med-api.nordavind.ru/api/protocol/user/register"));

//            request.setRawHeader("userId", "\"" + patient.uid);
        //    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded;charset=UTF-8");
        //    request.setRawHeader("Authorization","Basic " + (CONSUMER_KEY + ":" + CONSUMER_SECRET).toBase64() + "==");

        //    request.setRawHeader("Accept-Encoding","gzip");

        //    QUrl params;
        //    params.addQueryItem("grant_type","client_credentials");


        //    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(getResponse(QNetworkReply*)));

        //    manager->post(request,params.encodedQuery());
//            manager->deleteLater();
        }

    }
}

void BedsideScalesTestVisualize::saveSignal(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&file);

        for (int i = 0; i < m_calculator->signalSize(); ++i)
        {
            QString line = "";
            for (int j = 0; j < m_calculator->subChansCount(); ++j)
            {
                auto s = QString::number(m_calculator->signalValue(j, i));
                QString sSp = "";
                for (int k = 0; k < 20 - s.size(); ++k)
                    sSp += " ";
                line += s + sSp;
            }

            ts << line + "\n";
        }

        file.close();
    }
}

void BedsideScalesTestVisualize::showDiagParticalWeighting()
{
    for (int i = 0; i < m_calculator->size(); ++i)
    {
        auto value = m_calculator->weight(i);
        auto di = new DiagItem(value, QString::number(i + 1)); //dt.toString("dd.MM.yyyy hh:mm"));
        ui->wgtWeighingResults->appendItem(di);
    }

}

