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
    explicit ExecuteWidget(QWidget *parent = nullptr);
    ~ExecuteWidget() override;

    /*!
     * \brief Виртуальный метод, возвращающий уникальный идентификатор виджета
     */
    QString uid() override;

    /*!
     * \brief Виртуальный метод, возвращающий название виджета
     */
    QString name() override;

    /*!
     * \brief Вызывается после показа виджета
     */
    void onShow() override;
    /*!
     * \brief Вызывается перед прятанием виджета
     */
    void onHide() override;

    /*!
     * \brief Виртуальный метод, возвращающий true, если допустимы внешние элементы управления (глобальное меню и т.д.)
     */
    bool isExternalControl() override {return false;}

    /*!
     * \brief Виртуальный метод, возвращающий true, если виджет разрешает закрыться приложению
     * Чтобы запретить закрытие, нужно вернуть false
     */
    bool isCloseProgramQuery() override {return true;}

public slots:
    void showDB();


private:
    Ui::ExecuteWidget *ui;
};

#endif // EXECUTEWIDGET_H
