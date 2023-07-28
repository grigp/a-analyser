#ifndef AANALYSERAPPLICATION_H
#define AANALYSERAPPLICATION_H

#include "basedefines.h"
#include "datadefines.h"
#include "metodicdefines.h"
#include "factorsdefines.h"
#include "visualdefines.h"
#include "connection.h"
#include "summarydefines.h"

#include <QMainWindow>
#include <QObject>
#include <QApplication>
#include <QStandardItemModel>


class DataBase;
class MetodicsFactory;
class DriversFactory;
class FactorsFactory;
class SignalTransformFactory;
class SignalTransformer;
class Driver;
class NormsManager;
class VisualsFactory;
class VisualDescriptor;
class TestVisual;
class ProbeVisual;
class ChannelVisual;
class MultiFactorDescriptor;
class AddTestToSummaryDialog;
class SignalTransformerParamsWidget;
class PersonalProgramManager;
class PatientsModel;
class PatientsProxyModel;

class AAnalyserApplication : public QApplication
{
    Q_OBJECT
public:
    AAnalyserApplication(int &argc, char **argv);
    ~AAnalyserApplication() override;

    QMainWindow* mainWindow() const;
    void setMainWindow(QMainWindow *mw);

    /*!
     * \brief Возвращает геометрию окна пациента, положение и размеры
     * Использует положение окна MainWindow и геометрию рабочего стола.
     * Если в системе один монитор, то возвращает его геометрию.
     * Если более одного, то, если MainWindow на первом, то возвращает геометрию второго, а если
     * MainWindow на втором или далее, то возвращает первый
     */
    QRect getPatientWindowGeometry();

    /*!
     * \brief Показывает страницу с заданным uid
     * \param uidPage - uid страницы
     *     static_cast<AAnalyserApplication*>(QApplication::instance())->showClientPage(ClientWidgets::uidExecuteWidgetUid);
     */
    void showClientPage(const QString &uidPage);

    /*!
     * \brief Восстанавливает предыдущую страницу из стека последних показанных страниц
     * \return Возвращает true, если было, что восстанавливать
     */
    bool restoreClientPage();

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
    DataDefines::PatientKard getCurrentPatient() const;

    /*!
     * \brief Возвращает данные о выбранной методике
     */
    MetodicDefines::MetodicInfo getCurrentMetodic() const;

    /*!
     * \brief Возвращает информацию о выбранном тесте
     */
    DataDefines::TestInfo getCurrentTest() const;

    /*!
     * \brief Возвращает кол-во выделенных тестов
     */
    int selectedTestsCount() const;

    /*!
     * \brief Возвращает uid выделенного теста
     * \param idx - индекс теста в списке
     */
    QString selectedTest(const int idx) const;

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
     * \brief Получает данные о кол-ве методик в БД и осуществляет рассылку
     */
    void getMethodicCount(const int count);

    /*!
     * \brief Получение информации о выбранном тесте
     * \param uid
     */
    void doSelectBugTest(const QString &uid);

    /*!
     * \brief Получение информации о выбранном тесте
     * \param uid
     */
    void doSelectTest(const QString &uid);

    /*!
     * \brief Получение информации о выделенных тестах
     * \param tests - список выделенных тестов
     */
    void setSelectedTests(QStringList &tests);

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
     * \brief Показывает виджет индивидуальных программ
     */
    void showPersonalPrograms();

    /*!
     * \brief Переход к окну анализа сигналов
     */
    void signalsAnalysis();

    /*!
     * \brief Переход к окну сводок
     */
    void summaries();

    /*!
     * \brief Добавление показателей теста в сводку
     */
    void summaryAddTest();

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
    Driver* getDriverByProtocols(const QStringList &protocols, const int index = 0) const;

    /*!
     * \brief Возвращает экземпляр драйвера, соответствующего перечню форматов каналов
     * \param formats - перечень форматов каналов
     * \param index - номер в списке
     */
    Driver* getDriverByFormats(const QStringList &formats, const int index = 0) const;
    
    /*!
     * \brief Возвращает список допустимых портов для драйвера по его uid
     * \param drvUid - uid драйвера
     */
    QList<DeviceProtocols::Ports> getDriverPorts(const QString &drvUid) const;


    void registerVisual(VisualDescriptor* visual);

    int visualCount(const BaseDefines::TestLevel level);
    VisualDescriptor* getVisual(const BaseDefines::TestLevel level, const int idx);

    /*!
     * \brief Читает файл списка дневных программ и заполняет модель данных
     * \param model - заполняемая модель данных
     */
    void readDailyProgramList(QStandardItemModel& model, QStringList uids = QStringList());

    /*!
     * \brief Сохраняет список дневных программ из модели данных
     * \param model - модель данных с дневными программами
     */
    void saveDailyProgramList(const QStandardItemModel& model);

    /*!
     * \brief Читает файл списка индивидуальных программ и заполняет модель данных
     * \param model - заполняемая модель данных
     */
    void readPersonalProgramList(QStandardItemModel &model);

    /*!
     * \brief Сохраняет список индивидуальных программ из модели данных
     * \param model - модель данных с индивидуальными программами
     */
    void savePersonalProgramList(const QStandardItemModel& model);

    /*!
     * \brief Связывает дневные программы с индивидуальной
     * \param uidPP - индивидуальная программа
     * \param uidDPs - список дневных программ
     */
    void assignDailyProgramsForPersonal(QString& uidPP, QStringList& uidDPs);

    /*!
     * \brief Изменяет связку дневных программ с индивидуальной
     * Старые дневные программы удаляются
     * \param uidPP - индивидуальная программа
     * \param uidDPs - новый список дневных программ
     */
    void editDailyProgramsForPersonal(QString& uidPP, QStringList& uidDPs);

    /*!
     * \brief Очищает список дневных программ для индивидуальной
     * \param uidPP - uid индивидуальной программы
     */
    void clearListDailyProgramsForPersonal(QString& uidPP);

    /*!
     * \brief Вовзращает список дневных программ для индивидуальной
     * \param uidPP - uid индивидуальной программы
     */
    QStringList getListDailyProgramsForPersonal(QString& uidPP);

    /*!
     * \brief Назначает индивидуальную программу для пациента, вызывая диалог выбора программы
     * \return true, если назначили, false, если отменили
     */
    bool assignPPForPatient();

    /*!
     * \brief Отменяет индивидуальную программу для пациента
     * \return true, если отменили, false, если отменили отмену
     */
    bool cancelPPForPatient();

    /*!
     * \brief Изменение данных канала в БД
     * Вызывается тем, кто изменяет данные в БД
     * \param probeUid - uid пробы
     * \param channelId - id канала
     */
    void doChannelChanged(const QString &probeUid, const QString &channelId);

    /*!
     * \brief Возвращает указатель на модель списка пациентов
     */
    PatientsModel* patientsModel() const {return m_mdlPatients;}

    /*!
     * \brief Возвращает указатель на прокси-модель списка пациентов
     */
    PatientsProxyModel* patientsProxyModel() const {return m_pmdlPatients;}

    /*!
     * \brief Возвращает указатель на прокси-модель списка пациентов с актвной индивидуальной программой
     */
    PatientsProxyModel* patientsPPProxyModel() const {return m_pmdlPatientsPP;}

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

    /*!
     * \brief Регистрирует группу показателей
     * \param group - указатель на группу показателей
     */
    void registerGroup(MultiFactorDescriptor* group);

    /*!
     * \brief Возвращает кол-во групп показателей для заданного уровня теста
     * \param level - уровень теста
     */
    int multiFactorCount(const BaseDefines::TestLevel level);

    /*!
     * \brief Возвращает указатель на описатель групп показателей
     * \param level - уровень теста
     * \param idx - номер в списке
     */
    MultiFactorDescriptor* getMultiFactor(const BaseDefines::TestLevel level, const int idx);

    ///<-----------------------------------------------------------------------------
    ///< Преобразование сигналов

    /*!
     * \brief Регистрирует преобразователь сигнала
     * \param st - указатель на экземпляр класса преобразователя сигнала
     */
    void registerSignalTransformer(SignalTransformer* st);

    /*!
     * \brief Возвращает кол-во преобразователей
     */
    int signalTransformersCount();

    /*!
     * \brief Возвращает uid преобразователя по индексу в списке
     * \param idx - индекс в списке
     */
    QString signalTransformerUid(const int idx) const;

    /*!
     * \brief Возвращает название преобразователя по индексу в списке
     * \param idx - индекс в списке
     */
    QString signalTransformerName(const int idx) const;

    /*!
     * \brief Преобразование сигнала
     * \param idx - индекс в списке
     * \param buffer - преобразуемый сигнал
     */
    void transformSignal(const int idx, QVector<double> &buffer, const QJsonObject& params) const;

    /*!
     * \brief Преобразование сигнала
     * \param uid - идентификатор преобразователя
     * \param buffer - преобразуемый сигнал
     */
    void transformSignal(const QString uid, QVector<double> &buffer, const QJsonObject& params) const;

    /*!
     * \brief Создает виджет редактирования параметров преобразователя и возвращает указатель на него
     * \param idx - индекс в списке
     */
    SignalTransformerParamsWidget* getSignalTransformParamsWidget(const int idx) const;

    /*!
     * \brief Создает виджет редактирования параметров преобразователя и возвращает указатель на него
     * \param uid - идентификатор преобразователя
     */
    SignalTransformerParamsWidget* getSignalTransformParamsWidget(const QString uid) const;

    /*!
     * \brief Запомнить схему преобразований
     * \param sheme - Схема преобразований
     */
    void rememberTransformerScheme(const QJsonArray& sheme);

    /*!
     * \brief Возвращает запомненную схему преобразований
     */
    QJsonArray getTransformerScheme();

    /*!
     * \brief Очищает запомненную схему преобразований
     */
    void clearTransformerSheme();

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

    /*!
     * \brief Получение информации об изменении параметра приложения
     * \param group - группа параметров
     * \param param - имя параметра
     * \param value - новое значение
     */
    void changeApplicationParam(const QString &group, const QString &param, const QVariant &value);

    /*!
     * \brief Устанавливает активную сводку
     * \param uidMethodic - uid методики
     * \param kind - тип сводки
     */
    void setActiveSummary(const QString &uidMethodic, const SummaryDefines::Kind &kind);

    /*!
     * \brief Возвращает данные об активной сводке
     * \param asi - данные
     */
    void getActiveSummary(SummaryDefines::ActiveSummaryInfo &asi);

    ///<-----------------------------------------------------------------------------
    /// Управление прогресс-баром

    /*!
     * \brief Инициализирует полосу процесса, показывает ее
     * \param title - заголовок прогресса
     * \param begin - начальная точка
     * \param end - конечная точка
     * \param stage - название этапа
     */
    void initProgress(const QString &title, const int begin = 0, const int end = 100, const QString &stage = "");

    /*!
     * \brief Устанавливает позицию процесса
     * \param pos - значение позиции
     * \param stage - название этапа
     */
    void setProgressPosition(const int pos, const QString &stage = "");

    /*!
     * \brief Прячет полосу процесса
     */
    void doneProgress();

    ///<-----------------------------------------------------------------------------
    /// Управление ссылками на визуализаторы открытых тестов

    /*!
     * \brief Добавить открываемый тест в список
     * \param testUid - uid теста
     * \param visual - указатель на визуализатор
     */
    void addOpenedTest(const QString& testUid, QWidget* visual);

    /*!
     * \brief Удаляет открытый тест из списка
     * \param testUid - uid теста
     */
    void delOpenedTestFromList(const QString& testUid);

    /*!
     * \brief Очистить список открытых тестов
     */
    void clearOpenedTestList();

    /*!
     * \brief Возвращает указатель на виджет визуала открытого теста
     * \param testUid - uid теста
     */
    QWidget* getOpenedTest(const QString& testUid);

signals:
    void dbConnected();

    void dbDisconnected();

    void newTest(const QString &testUid);
    void removeTest(const QString &testUid);

    /*!
     * \brief Сообщает миру о создании секции
     * \param channelUid - uid канала
     * \param name - название секции
     * \param number - номер секции в строковом виде
     */
    void sectionCreated(QString &channelUid, const QString &name, const QString number);

    void selectPatient(const QString &patientUid);
    void selectMetodic(const QString &metodicUid);
    void selectTest(const QString &testUid);

    void methodicCount(const int count);

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

    /*!
     * \brief Сигнал о том, что у индивидуальной нормы изменено кол-во нормообразующих обследований
     * \param patientUid - uid пациента
     * \param methodUid - uid методики
     * \param conditionUid - uid условий проведения
     */
    void personalNormContainedChange(const QString &patientUid, const QString &methodUid, const QString &conditionUid);

    /*!
     * \brief Извещает мир об изменении условий проведения для теста
     * \param testUid - uid теста
     */
    void changeTestCondition(const QString &testUid);

    /*!
     * \brief Изменение параметра приложения
     * \param group - группа параметров
     * \param param - имя параметра
     * \param value - новое значение
     */
    void applicationParamChanged(const QString &group, const QString &param, const QVariant &value);

    /*!
     * \brief Сигнал начала добавления тестов в сводку
     */
    void addTestToSummaryBegin();
    /*!
     * \brief Сигнал окончания добавления тестов в сводку
     */
    void addTestToSummaryEnd();

    /*!
     * \brief Сигнал добавления теста в сводку
     * \param testUid - uid теста
     * \param mode - режим: в существующую, в новую, в активную
     * \param summary - имя сводки для новой или имя файла для существующей
     * \param kind - тип сводки: первичные / все показатели
     */
    void addTestToSummary(const QString testUid,
                          const SummaryDefines::AddTestMode mode,
                          const QString summary,
                          const SummaryDefines::Kind kind);

    /*!
     * \brief Сигнал изменения данных канала в БД
     * \param probeUid - uid пробы
     * \param channelId - id канала
     */
    void channelChanged(const QString &probeUid, const QString &channelId);

    /*!
     * \brief Сигнал о назначении индивидуальной программы для пациента
     * \param patientUid - uid пациента
     * \param ppUid - uid индивидуальной программы
     */
    void assignedPPForPatient(const QString& patientUid, const QString& ppUid);

    /*!
     * \brief Сигнал о снятии индивидуальной программы для пациента
     * \param patientUid - uid пациента
     */
    void canceledPPForPatient(const QString& patientUid);

protected:
    //! Обработка исключений основного цикла программы
    bool notify(QObject *, QEvent *) override;

private slots:
    void on_dbConnected();

private:
    void on_AddTestToSummaryAccepted();

    bool isOneMethodicOnAddTests();

    QMainWindow *m_mw;
    QString m_languargeCode = DataDefines::LANG_CODE_RUS;

    DataBase *m_database {nullptr};
    MetodicsFactory *m_metodics {nullptr};
    DriversFactory *m_drivers {nullptr};
    FactorsFactory *m_factors {nullptr};
    SignalTransformFactory *m_transformers {nullptr};
    NormsManager *m_normsManager {nullptr};
    VisualsFactory *m_visualsFactory {nullptr};
    PersonalProgramManager *m_ppManager {nullptr};

    PatientsModel *m_mdlPatients {nullptr};
    PatientsProxyModel *m_pmdlPatients {nullptr};   ///< Пациенты без индивидуальной программы
    PatientsProxyModel *m_pmdlPatientsPP {nullptr}; ///< Пациенты с индивидуальной программой

    QString m_patientUid = "";    ///< uid выбранного пациента
    QString m_metodicUid = "";    ///< uid выбранной методики
    QString m_testUid = "";       ///< uid выбранного теста
    QStringList m_selectedTests;  ///< Список uid выделенных тестов

    AddTestToSummaryDialog* m_addTSDlg {nullptr};
    SummaryDefines::ActiveSummaryInfo m_asi;

    QMap<QString, QWidget*> m_openedTests;   ///< Указатели на виджет визуализатора открытого теста
};

#endif // AANALYSERAPPLICATION_H
