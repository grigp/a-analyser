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
static const QString DeviceDataType = "bed_side_weight";//"Type1";//"bed_side_weight";

static const QString HealthLineName = "health_line_line1";
static const QString HealthLinePassword = "health_line_line2";

static const QString DevicePrivateKey = "cd4c11893d8ca16992387e8405a713eaa8edfa2905ef068670c0c7f4e0bdcf9f";
static const QString DevicePublicKey = "04f4c453191f38cf67c8c32472fc36196848bc1d3b61e2e2ebdbc85082d4ec830724b3dd79a352e2b97b56156e338a798c51003001fa18d4ec2da62c1333d470a9";

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

    //! Создание запроса
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    //! Формирование запроса
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    //! Передача запроса
    m_reply = mgr->post(request, data);

    //! Подключение для получения ответа на запрос
    connect(m_reply, &QNetworkReply::finished, this, &BedsideScalesTestVisualize::networkAnswer);
}

void BedsideScalesTestVisualize::networkAnswer()
{
    //! Запрос реально был
    if (m_reply)
    {
        //! Проверка на ошибку
        bool ar = m_reply->error() == QNetworkReply::NoError;
        if(ar)  //! Не было ошибки
            m_answer = QString::fromUtf8(m_reply->readAll());
        else
        {       //! Была ошибка
            m_answer = m_reply->errorString();
            qDebug() << "---- error ----" << QString::fromUtf8(m_reply->readAll());
        }
        qDebug() << "Ответ :" << m_answer;
        m_reply->deleteLater();

        //! При отсутствии ошибки следующий запрос
        if (ar)
        {
            //! Зарегистрировали пациента - получим список устройств
            if (m_netWebSendStage == nwsRegUser)
            {
                m_userId = m_answer;
                networkGetListDevice();
            }
            else
            //! Получили список устройств - если нашего устройства нет, зарегистрируем его, иначе получим TimrMarker
            if (m_netWebSendStage == nwsGetListDevice)
            {
                if (!isDeviceOnList(m_answer))
                    networkAddDevice(m_userId);
                else
                    networkGetTimeMarker();
            }
            else
            //! Добавили устройство - получим TimrMarker
            if (m_netWebSendStage == nwsAddDevice)
            {
                m_deviceId = m_answer;
                networkGetTimeMarker();
            }
            else
            //! Получили TimrMarker - запросим данные
            if (m_netWebSendStage == nwsTimeMarker)
            {
                m_timeMarker = m_answer;
                //! Разберем TimeMarker - выделим значение
                QJsonDocument doc(QJsonDocument::fromJson(m_timeMarker.toUtf8()));
                auto obj = doc.object();
                m_timeMarkerValue = obj["value"].toString();
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
    obj["username"] = m_email; //"grig_p@mail.ru";
    obj["password"] = m_password; //"MVSfWhPb5HqWvj2";
    obj["publickey"] = userUid;
    //! Передача запроса на сервер через единую процедуру
    networkRequest(m_netManager, QUrl(QStringLiteral("https://med-api.nordavind.ru/api/protocol/user/register")), obj);
}

void BedsideScalesTestVisualize::networkAddDevice(const QString userId)
{
    m_netWebSendStage = nwsAddDevice;
    QJsonObject obj;
    obj["user"] = userId;
    obj["publickey"] = DevicePublicKey;
    obj["type"] = DeviceDataType;
    //! Передача запроса на сервер через единую процедуру
    networkRequest(m_netManager, QUrl(QStringLiteral("https://med-api.nordavind.ru/api/protocol/medicalDevice/register")), obj);
}

void BedsideScalesTestVisualize::networkGetListDevice()
{
    m_netWebSendStage = nwsGetListDevice;
    //! Передача запроса на сервер через единую процедуру
    networkRequest(m_netManager,
                   QUrl(QStringLiteral("https://med-api.nordavind.ru/api/protocol/medicalDevice/getList?userAddress=") + m_userId),
                   QJsonObject());
}

void BedsideScalesTestVisualize::networkGetTimeMarker()
{
    m_netWebSendStage = nwsTimeMarker;
    //! Передача запроса на сервер через единую процедуру
    networkRequest(m_netManager,
                   QUrl(QStringLiteral("https://med-api.nordavind.ru/api/protocol/getTimeMarker?user=") + m_userId),
                   QJsonObject());
}

void BedsideScalesTestVisualize::networkSendData()
{
    m_netWebSendStage = nwsSendData;

    //! Подготовим данные для передачи в виде байтового массива baData
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
    QByteArray baData = doc.toJson();

    //! Запрос в виде множественного сообщения
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    //! Добавим пациента по id от сервера
    QString body = m_userId;
    QString header = "form-data; name=\"User\"";
    QHttpPart tpUser;
    tpUser.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(header));
    tpUser.setBody(body.toUtf8());
    qDebug() << "------  Данные для передачи данных ------------";
    qDebug() << "header :" << header;
    qDebug() << "body :" << body.toUtf8();

    //! Добавим идентификатор устройства по id от сервера
    body = m_deviceId;
    header = "form-data; name=\"Device.Address\"";
    QHttpPart tpDvcAddr;
    tpDvcAddr.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(header));
    tpDvcAddr.setBody(body.toUtf8());
    qDebug() << "header :" << header;
    qDebug() << "body :" << body.toUtf8();

    //! Добавим тип данных
    body = DeviceDataType;
    header = "form-data; name=\"Device.Type\"";
    QHttpPart tpDvcType;
    tpDvcType.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(header));
    tpDvcType.setBody(body.toUtf8());
    qDebug() << "header :" << header;
    qDebug() << "body :" << body.toUtf8();

    //! Добавим TimeMarker
    body = m_timeMarkerValue;
    header = "form-data; name=\"TimeMarker\"";
    QHttpPart tpTimeMarker;
    tpTimeMarker.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(header));
    tpTimeMarker.setBody(body.toUtf8());
    qDebug() << "header :" << header;
    qDebug() << "body :" << body.toUtf8();

    //! Разделим TimeMarker на две части, передавать для сигнатуры только вторую
    auto tmv_sep = m_timeMarkerValue.split(':');
    QString tmv2 = m_timeMarkerValue;
    if (tmv_sep.size() > 1)
        tmv2 = tmv_sep.at(1);
    //! Добавим сигнатуру
    body = getSignature(convertToBytes(m_userId.mid(2)),
                        convertToBytes(m_deviceId.mid(2)),
                        DeviceDataType.toUtf8(),
                        convertToBytes(tmv2.mid(2)),
                        baData,
                        convertToBytes(DevicePrivateKey));
    QHttpPart tpSignature;
    header = "form-data; name=\"Signature\"";
    tpSignature.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(header));
    tpSignature.setBody(body.toUtf8());
    qDebug() << "header :" << header;
    qDebug() << "body :" << body.toUtf8();

    //! Добавим данные
    QHttpPart tpData;
    header = "form-data; name=\"data\"; filename=\"data\"";
    tpData.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
    tpData.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(header));
    tpData.setBody(baData);
    qDebug() << "header :" << header;
    qDebug() << "body :" << baData;

    //! Собираем все в кучу
    multiPart->append(tpUser);
    multiPart->append(tpDvcAddr);
    multiPart->append(tpDvcType);
    multiPart->append(tpTimeMarker);
    multiPart->append(tpSignature);
    multiPart->append(tpData);

    qDebug() << ++n << ") ------";
    qDebug() << "Запрос :" << QUrl(QStringLiteral("https://med-api.nordavind.ru/api/protocol/request"));
    //! И передаем запрос
    QNetworkRequest request(QUrl(QStringLiteral("https://med-api.nordavind.ru/api/protocol/request")));
    m_reply = m_netManager->post(request, multiPart);
    multiPart->setParent(m_reply);

    //! Подключаемся и ждем ответ
    connect(m_reply, &QNetworkReply::finished, this, &BedsideScalesTestVisualize::networkAnswer);
}

QString BedsideScalesTestVisualize::getSignature(QByteArray userAddress,
                                                 QByteArray deviceAddress,
                                                 QByteArray deviceType,
                                                 QByteArray timeMarker,
                                                 QByteArray dataBytes,
                                                 QByteArray key)
{
    QByteArray buffer;

    buffer.append(userAddress);
    buffer.append(deviceAddress);
    buffer.append(deviceType);
    buffer.append(timeMarker);
    buffer.append(dataBytes);
    buffer.append(key);

    QByteArray signatureHash = QCryptographicHash::hash(buffer, QCryptographicHash::Sha256);
    return convertToString(signatureHash);
}

QByteArray BedsideScalesTestVisualize::convertToBytes(QString hexString)
{
    QByteArray result;
    bool ok;
    for(int i = 0; i < hexString.length() / 2; ++i)
    {
        QString hexByte = hexString.mid(i*2,2);
        auto num = quint8(hexByte.toUShort(&ok, 16));
        result.append(static_cast<char>(num));
    }
    return result;
}

QString BedsideScalesTestVisualize::convertToString(QByteArray array)
{
    QString output = "0x";
    int length = array.length();
    for(int i = 0; i < length; ++i)
    {
        auto value = quint8(array.at(i));
        output.append(QString::number(value, 16).rightJustified(2, '0'));
    }
    return output;
}


void BedsideScalesTestVisualize::on_sendToWeb()
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(m_testUid, ti))
    {
        //! Запрос аутентификационных данных для сервера HealthLine (https://med.nordavind.ru/)
        AuthorizationDialog dialog(this);
        dialog.setTitle("Введите регистрационые данные");
        dialog.setInfo("Необходимо ввести регистрационые данные, адрес электронной почты и пароль,\nс которыми Вы регистрировались на сайте HealthLine (https://med.nordavind.ru/)");
        dialog.setTextName("Адрес электронной почты :");
        dialog.setTextPassword("Пароль :");

        //! Данные хранятся в карточке пациента, как свободные данные пациента
        auto name = DataProvider::patientData(ti.patientUid, HealthLineName).toString();
        auto password = DataProvider::patientData(ti.patientUid, HealthLinePassword).toString();
        dialog.setName(BaseUtils::stringDecrypt(name));
        dialog.setPassword(BaseUtils::stringDecrypt(password));

        //! Диалог
        if (dialog.exec() == QDialog::Accepted)
        {
            //! Сохранение
            m_email = dialog.name();
            m_password = dialog.password();
            DataProvider::patientSetData(ti.patientUid, HealthLineName, BaseUtils::stringEncrypt(m_email));
            DataProvider::patientSetData(ti.patientUid, HealthLinePassword, BaseUtils::stringEncrypt(m_password));
            //! Передача на сервер
            sendToWeb();
        }
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
    //! Получение uid пациента для данного теста
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(m_testUid, ti))
    {
        DataDefines::PatientKard patient;
        if (DataProvider::getPatient(ti.patientUid, patient))
        {
            n = 0;
            m_netManager = new QNetworkAccessManager(this);
            //! Первым этапом - регистрация пациента
            networkRegisterUser(BaseUtils::removeSignesFromUuid(ti.patientUid));
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

