#ifndef EXECUTEWIDGET_H
#define EXECUTEWIDGET_H

#include <QWidget>
#include "clientwidget.h"

namespace Ui {
class ExecuteWidget;
}

namespace ClientWidgets
{
    static const QString uidExecuteWidgetUid = "CW_Execute";
}

class ExecuteWidget : public ClientWidget
{
    Q_OBJECT

public:
    explicit ExecuteWidget(QWidget *parent = 0);
    ~ExecuteWidget();

    /*!
     * \brief Виртуальный метод, возвращающий уникальный идентификатор виджета
     */
    QString uid() override;

    /*!
     * \brief Виртуальный метод, возвращающий название виджета
     */
    QString name() override;

public slots:
    void showDB();


private:
    Ui::ExecuteWidget *ui;
};

#endif // EXECUTEWIDGET_H
