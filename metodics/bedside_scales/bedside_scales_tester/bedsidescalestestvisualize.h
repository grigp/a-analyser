#ifndef BEDSIDESCALESTESTVISUALIZE_H
#define BEDSIDESCALESTESTVISUALIZE_H

#include <QWidget>
#include <QPrinter>
#include <QJsonObject>
#include <QNetworkAccessManager>

namespace Ui {
class BedsideScalesTestVisualize;
}

class BedsideScalesTestCalculator;
class WebSendResultMessage;

/*!
 * \brief Класс фрейма визуализации результатов для прикроватных весов The BedsideScalesTestVisualize class
 */
class BedsideScalesTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit BedsideScalesTestVisualize(QWidget *parent = nullptr);
    ~BedsideScalesTestVisualize() override;

    void setTest(const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

private slots:
    void signalExport();

    void on_weighingResultsGraph();
    void on_weighingResultsBar();
    void on_weighingResults3D(bool is3D);

    //! ----- Методы передачи данных на сервер HealthLine (https://med.nordavind.ru/) ----------------
    void networkRequest(QNetworkAccessManager *m_netManager, const QUrl url, QJsonObject obj = QJsonObject());
    void networkAnswer();
    /*!
     * \brief Возвращает true, если устройство в списке (уже зарегистрировано)
     * \param list - список устройств
     */
    bool isDeviceOnList(const QString list);

    /*!
     * \brief Регистрация пациента в системе HealthLine
     * \param userUid - uid пациента
     */
    void networkRegisterUser(const QString userUid);
    /*!
     * \brief Добавление устройства для пациента
     * \param userId - uid пациента
     */
    void networkAddDevice(const QString userId);
    /*!
     * \brief Получение списка устройств для пациента
     */
    void networkGetListDevice();
    /*!
     * \brief Получение TimeMarker от сервера
     */
    void networkGetTimeMarker();
    /*!
     * \brief Передача данных на сервер
     */
    void networkSendData();

    /*!
     * \brief Формирование сигнатуры для передачи данных
     * \param userAddress - id пациента от сервера
     * \param deviceAddress - id устройства
     * \param deviceType - тип устройства
     * \param timeMarker
     * \param dataBytes
     * \param key
     * \return строку сигнатуры
     */
    QString getSignature(QByteArray userAddress,
                         QByteArray deviceAddress,
                         QByteArray deviceType,
                         QByteArray timeMarker,
                         QByteArray dataBytes,
                         QByteArray key);
    QByteArray convertToBytes(QString hexString);
    QString convertToString(QByteArray array);

    void on_sendToWeb();
    //! ----------------------

    void on_selectItem(const int idx);

private:
    Ui::BedsideScalesTestVisualize *ui;

    /*!
     * \brief этапы передачи данных The NWStage enum
     */
    enum NWStage
    {
          nwsNone = 0
        , nwsRegUser        ///< Регистрация пользователя
        , nwsGetListDevice  ///< Запрос списка устройств
        , nwsAddDevice      ///< Добавление устройства
        , nwsTimeMarker     ///< Получение time Marker
        , nwsSendData       ///< Передача данных
    };

    void sendToWeb();

    void saveSignal(const QString &fileName);

    void showDiagParticalWeighting();

    BedsideScalesTestCalculator* m_calculator {nullptr};
    QString m_testUid {""};

    int m_numReq {0};

    QString m_email {""};
    QString m_password {""};
    QNetworkAccessManager *m_netManager {nullptr};
    QNetworkReply *m_reply {nullptr};
    QString m_answer {""};
    QString m_userId {""};
    QString m_deviceId {""};
    QString m_timeMarker {""};
    QString m_timeMarkerValue {""};
    NWStage m_netWebSendStage {nwsNone};
    WebSendResultMessage* m_msgResult {nullptr};
};

#endif // BEDSIDESCALESTESTVISUALIZE_H
