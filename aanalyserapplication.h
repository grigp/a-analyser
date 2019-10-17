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
class NormsManager;

class AAnalyserApplication : public QApplication
{
    Q_OBJECT
public:
    AAnalyserApplication(int &argc, char **argv, const QString &languargeCode);
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
     * \brief Смена базы данных
     * \param dataBaseFolder - путь к новой БД
     */
    void changeDatabase(const QString &dataBaseFolder);

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
     * \brief Получение информации о выбранном тесте
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
     * \brief Удаление теста, выбранного ранее (в doSelectTest)
     */
    void deleteTest();
    void deleteTest(const QString &testUid);

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
                        const QString &measure, const int format,
                        const double multiplier, const FactorsDefines::NormSide side, const int agingPeriod);

    /*!
     * \brief Регистрирует группу показателей
     * \param uid - uid группы
     * \param name - название группы
     */
    void registerGroup(const QString &uid, const QString &name);

    ///<-----------------------------------------------------------------------------


    ///<-----------------------------------------------------------------------------
    ///< Индивидуальные нормы

    /*!
     * \brief Возвращает список условий проведения в виде списка uid
     */
    QStringList getTestConditions();

    /*!
     * \brief Возвращает данные об условиях проведения
     * \param uid - uid условий проведения
     * \param ci - информация об условиях проведения
     * \return true, если удалось вернуть
     */
    bool getTestConditionInfo(const QString &uid, DataDefines::TestConditionInfo &ci);

    /*!
     * \brief Получение индивидуальных норм для всех показателей теста
     * \param testUid - uid теста
     * \param pnil - возвращаемый список индивидуальных норм для теста
     * \return true, если нормы получены
     */
    bool getPersonalNorm(const QString &testUid, QList<DataDefines::PersonalNormInfo> &pnil) const;

    /*!
     * \brief Возвращает количество нормообразующих обследований для нормы, которую будет использовать тест
     * \param testUid - uid теста
     */
    int getPersonalNormContainedTestCount(const QString &testUid) const;

    /*!
     * \brief Устанавливает для теста признак включения в нормообразующие в true или false
     * \param testUid - uid теста
     * \param isNormContained - будет ли тест нормообразующим
     */
    void setTestNormContained(const QString &testUid, const bool isNormContained) const;

    /*!
     * \brief Расчет индивидуальной нормы для пациента по методике и условиям проведения
     * \param patientUid - uid пациента
     * \param methodUid - uid методики
     * \param conditionUid - uid условий проведения
     */
    void calculatePersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid) const;

    /*!
     * \brief Расчет всех индивидуальных норм
     */
    void calculateAllPersonalNorms();

    /*!
     * \brief Возвращает список групповых норм для заданной методике по заданным условиям проведения
     * \param methodicUid - uid методики
     * \param consitionUid - uid условий проведения
     * \param gni - список индивидуальных норм
     * \return true, если удалось вернуть
     */
    bool getGroupNorms(const QString &methodicUid, const QString &conditionUid,
                       QList<DataDefines::GroupNormInfo> &gni);

    ///<-----------------------------------------------------------------------------

    /*!
     * \brief Запоминает код установленной локализации
     * \param code - код установленного языка
     */
    void setLanguargeCode(const QString &code);

    /*!
     * \brief Возвращает код установленной локализации
     */
    QString languargeCode() const;

signals:
    void dbConnected();

    void dbDisconnected();

    void newTest(const QString &testUid);
    void removeTest(const QString &testUid);

    void selectPatient(const QString &patientUid);
    void selectMetodic(const QString &metodicUid);
    void selectTest(const QString &testUid);

    void sendData(DeviceProtocols::DeviceData *data);

    /*!
     * \brief Сигнал о том, что индивидуальная норма пересчитана
     * \param patientUid - uid пациента
     * \param methodUid - uid методики
     * \param conditionUid - uid условий проведения
     */
    void personalNormRecalculated(const QString &patientUid, const QString &methodUid, const QString &conditionUid);

    /*!
     * \brief Сигнал о том, что индивидуальная норма удалена
     * \param patientUid - uid пациента
     * \param methodUid - uid методики
     * \param conditionUid - uid условий проведения
     */
    void personalNormDeleted(const QString &patientUid, const QString &methodUid, const QString &conditionUid);

protected:
    //! Обработка исключений основного цикла программы
    bool notify(QObject *, QEvent *) override;

private:
    QMainWindow *m_mw;
    QString m_languargeCode = DataDefines::LANG_CODE_RUS;

    DataBase *m_database {nullptr};
    MetodicsFactory *m_metodics {nullptr};
    DriversFactory *m_drivers {nullptr};
    FactorsFactory *m_factors {nullptr};
    NormsManager *m_normsManager {nullptr};

    QString m_patientUid = "";  ///< uid выбранного пациента
    QString m_metodicUid = "";  ///< uid выбранной методики
    QString m_testUid = "";     ///< uid выбранного теста

};

#endif // AANALYSERAPPLICATION_H
