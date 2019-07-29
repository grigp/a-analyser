#ifndef AANALYSERAPPLICATION_H
#define AANALYSERAPPLICATION_H

#include "datadefines.h"
#include "metodicdefines.h"
#include "factorsdefines.h"
#include "connection.h"

#include <QMainWindow>
#include <QObject>
#include <QApplication>


class DataBase;
class MetodicsFactory;
class DriversFactory;
class FactorsFactory;
class Driver;

class AAnalyserApplication : public QApplication
{
    Q_OBJECT
public:
    AAnalyserApplication(int &argc, char **argv);
    ~AAnalyserApplication();

    QMainWindow* mainWindow() const;
    void setMainWindow(QMainWindow *mw);

    /*!
     * \brief Показывает страницу с заданным uid
     * \param uidPage - uid страницы
     *     static_cast<AAnalyserApplication*>(QApplication::instance())->showClientPage(ClientWidgets::uidExecuteWidgetUid);
     */
    void showClientPage(const QString &uidPage);

    /*!
     * \brief Возвращает указатель на БД.
     * В клиентских модулях не пользоваться!
     * Вызывать продедуры из namespace DataProvider!
     */
    DataBase* getDB();

    /*!
     * \brief Возвращает карточку выбранного пациента
     */
    DataDefines::PatientKard getSelectedPatient() const;

    /*!
     * \brief Возвращает данные о выбранной методике
     */
    MetodicDefines::MetodicInfo getSelectedMetodic() const;

    /*!
     * \brief Возвращает указатель на список методик
     * В клиентских модулях не пользоваться!
     * Вызывать продедуры из namespace DataProvider!
     */
    MetodicsFactory* getMetodics();

    /*!
     * \brief Получение данных о выбранной записи о пациенте
     * \param uid - uid выбранного пациента
     */
    void doSelectPatient(const QString &uid);

    /*!
     * \brief Получение информации о выбранной методике
     * \param uid - uid выбранной методики
     */
    void doSelectMetodic(const QString &uid);

    /*!
     * \brief Получение иформации о выбранном тесте
     * \param uid
     */
    void doSelectTest(const QString &uid);

    /*!
     * \brief Выполнение методики
     * Должны быть выбраны предварительно пациент и методика
     * Если нет, то действие не будет выполнено
     */
    void executeMetodic();

    /*!
     * \brief Выбор теста и открытие его результатов
     * \param uid - uid теста
     */
    void openTest(const QString &uid);

    /*!
     * \brief Показывает виджет базы данных
     */
    void showDataBase();

    /*!
     * \brief Возвращает список uid подключенных драйверов
     */
    QStringList getDrivers() const;

    /*!
     * \brief Возвращает название драйвера по uid
     * \param drvUid
     */
    QString getDriverName(const QString &drvUid) const;

    /*!
     * \brief Возвращает список подключений
     * Прежде всего для вывода в окне редактирования
     */
    QList<Connection> getConnections() const;

    /*!
     * \brief Добавляет подключение в список
     * \param active - флаг активности
     * \param drvUid - uid драйвера
     * \param params - параметры драйвера
     * \param port - порт
     * \param comment - комментарий
     */
    void addConnection(const bool active,
                       const QString &drvUid, const QJsonObject &params,
                       const DeviceProtocols::Ports port, const QString &comment);

    /*!
     * \brief Перемещает подключение вверх на одну позицию
     * \param connectIdx - перемещаемый индекс в массиве подключений
     */
    void moveConnectionUp(const int connectIdx);

    /*!
     * \brief Перемещает подключение вниз на одну позицию
     * \param connectIdx - перемещаемый индекс в массиве подключений
     */
    void moveConnectionDown(const int connectIdx);

    /*!
     * \brief Удаляет подключение
     * \param connectIdx - удаляемый индекс в массиве подключений
     */
    void deleteConnection(const int connectIdx);

    /*!
     * \brief Изменены основные параметры подключения
     * \param connectIdx - индекс в массиве подключений
     * \param paramIdx - код параметра 0 - active, 1 - x, 2 - port, 3 - comment
     * \param value - значение
     */
    void dataChangedConnection(const int connectIdx, const int paramIdx, const QVariant value);

    /*!
     * \brief Вызывает диалог редактирования параметров драйвера для подключения
     * \param connectIdx - индекс в массиве подключений
     * \param drvUid - uid драйвера
     * \param params - редактируемые параметры
     * \return true, если диалог завершился, как accept
     */
    bool editParamsConnecton(const int connectIdx, const QString &drvUid, QJsonObject &params);

    /*!
     * \brief Возвращает экземпляр драйвера, соответствующего перечню протоколов
     * \param protocols - перечень протоколов
     * \param index - номер в списке
     */
    Driver* getDriver(const QStringList &protocols, const int index = 0) const;

    /*!
     * \brief Возвращает список допустимых портов для драйвера по его uid
     * \param drvUid - uid драйвера
     */
    QList<DeviceProtocols::Ports> getDriverPorts(const QString &drvUid) const;


    ///<-----------------------------------------------------------------------------
    ///< Показатели

    /*!
     * \brief Возвращает информацию о показателе
     * \param uid - uid показателя
     */
    FactorsDefines::FactorInfo getFactorInfo(const QString &uid) const;

    /*!
     * \brief Возвращает название группы показателей по uid группы
     * \param groupUid - uid группы показателей
     */
    QString getGroupName(const QString &groupUid) const;

    /*!
     * \brief Регистрирует показатель и информацию о нем.
     * Этот метод вызывают факторсгруппы
     */
    void registerFactor(const QString &uid, const QString &groupUid,
                        const QString &name, const QString &shortName,
                        const QString &measure, const int format);

    /*!
     * \brief Регистрирует группу показателей
     * \param uid - uid группы
     * \param name - название группы
     */
    void registerGroup(const QString &uid, const QString &name);


    ///<-----------------------------------------------------------------------------

signals:
    void dbConnected();

    void dbDisconnected();

    void newTest(const QString &testUid);

    void selectPatient(const QString &patientUid);
    void selectMetodic(const QString &metodicUid);
    void selectTest(const QString &testUid);

    void sendData(DeviceProtocols::DeviceData *data);

private:
    QMainWindow *m_mw;
    DataBase *m_database {nullptr};
    MetodicsFactory *m_metodics {nullptr};
    DriversFactory *m_drivers {nullptr};
    FactorsFactory *m_factors {nullptr};

    QString m_patientUid = "";  ///< uid выбранного пациента
    QString m_metodicUid = "";  ///< uid выбранной методики
    QString m_testUid = "";     ///< uid выбранного теста
};

#endif // AANALYSERAPPLICATION_H
