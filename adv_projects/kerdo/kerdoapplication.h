#ifndef KERDOAPPLICATION_H
#define KERDOAPPLICATION_H

#include <QMainWindow>
#include <QObject>
#include <QApplication>

#include "datadefines.h"

class DataBase;

class KerdoApplication : public QApplication
{
    Q_OBJECT
public:
    KerdoApplication(int &argc, char **argv);
    ~KerdoApplication() override;

    QMainWindow* mainWindow() const;
    void setMainWindow(QMainWindow *mw);

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
     * \brief Получение данных о выбранной записи о пациенте
     * \param uid - uid выбранного пациента
     */
    void doSelectPatient(const QString &uid);

signals:
    void dbConnected();
    void dbDisconnected();
    void newTest(const QString &testUid);
    void selectPatient(const QString &patientUid);

private:
    QMainWindow *m_mw;
    DataBase *m_database {nullptr};

    QString m_patientUid = "";  ///< uid выбранного пациента
};

#endif // KERDOAPPLICATION_H
