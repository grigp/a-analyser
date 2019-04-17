#ifndef DATABASERESULTWIDGET_H
#define DATABASERESULTWIDGET_H

#include <QWidget>
#include "clientwidget.h"

namespace Ui {
class DataBaseResultWidget;
}

namespace ClientWidgets
{
    static const QString uidDatabaseResultWidgetUid = "CW_DatabaseResult";
}

class DataBaseResultWidget : public ClientWidget
{
    Q_OBJECT

public:
    explicit DataBaseResultWidget(QWidget *parent = 0);
    ~DataBaseResultWidget();

    /*!
     * \brief Виртуальный метод, возвращающий уникальный идентификатор виджета
     */
    QString uid() override;

    /*!
     * \brief Виртуальный метод, возвращающий название виджета
     */
    QString name() override;

private slots:
    void executeProbe();

private:
    Ui::DataBaseResultWidget *ui;
};

#endif // DATABASERESULTWIDGET_H
