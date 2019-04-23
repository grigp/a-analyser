#ifndef AANALYSERAPPLICATION_H
#define AANALYSERAPPLICATION_H

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
     * \brief Возвращает указатель на список методик
     * В клиентских модулях не пользоваться!
     * Вызывать продедуры из namespace DataProvider!
     */
    MetodicsFactory* getMetodics();

signals:
    void dbConnected();

    void dbDisconnected();

private:
    QMainWindow *m_mw;
    DataBase *m_database {nullptr};
    MetodicsFactory *m_metodics {nullptr};
};

#endif // AANALYSERAPPLICATION_H
