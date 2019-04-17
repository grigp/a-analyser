#ifndef AANALYSERAPPLICATION_H
#define AANALYSERAPPLICATION_H

#include <QMainWindow>
#include <QObject>
#include <QApplication>

class AAnalyserApplication : public QApplication
{
public:
    AAnalyserApplication(int &argc, char **argv, int flags = ApplicationFlags);

    void setMainWindow(QMainWindow *mw);

    /*!
     * \brief Показывает страницу с заданным uid
     * \param uidPage - uid страницы
     *     static_cast<AAnalyserApplication*>(QApplication::instance())->showClientPage(ClientWidgets::uidExecuteWidgetUid);
     */
    void showClientPage(const QString &uidPage);

private:
    QMainWindow *m_mw;
};

#endif // AANALYSERAPPLICATION_H
