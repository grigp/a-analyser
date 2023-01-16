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

    void networkRequest(QNetworkAccessManager *m_netManager, const QUrl url, QJsonObject obj = QJsonObject());
    void networkAnswer();
    /*!
     * \brief Возвращает true, если устройство в списке (уже зарегистрировано)
     * \param list - список устройств
     */
    bool isDeviceOnList(const QString list);

    void networkRegisterUser(const QString userUid);
    void networkAddDevice(const QString userId);
    void networkGetListDevice();
    void networkGetTimeMarker();
    void networkSendData();

    void on_sendToWeb();

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

    QString m_email {""};
    QString m_password {""};
    QNetworkAccessManager *m_netManager {nullptr};
    QNetworkReply *m_reply {nullptr};
    QString m_answer {""};
    QString m_userId {""};
    QString m_deviceId {""};
    QString m_timeMarker {""};
    NWStage m_netWebSendStage {nwsNone};
};

#endif // BEDSIDESCALESTESTVISUALIZE_H
