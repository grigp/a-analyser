#ifndef AANALYSERAPPLICATION_H
#define AANALYSERAPPLICATION_H

#include "datadefines.h"
#include "metodicdefines.h"

#include <QMainWindow>
#include <QObject>
#include <QApplication>

class DataBase;
class MetodicsFactory;

class AAnalyserApplication : public QApplication
{
    Q_OBJECT
public:
    AAnalyserApplication(int &argc, char **argv);
    ~AAnalyserApplication();

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
    void selectPatient(const QString &uid);

    /*!
     * \brief Получение информации о выбранной методике
     * \param uid - uid выбранной методики
     */
    void selectMetodic(const QString &uid);

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

signals:
    void dbConnected();

    void dbDisconnected();

private:
    QMainWindow *m_mw;
    DataBase *m_database {nullptr};
    MetodicsFactory *m_metodics {nullptr};

    QString m_patientUid = "";  ///< uid выбранного пациента
    QString m_metodicUid = "";  ///< uid выбранной методики
};

#endif // AANALYSERAPPLICATION_H
