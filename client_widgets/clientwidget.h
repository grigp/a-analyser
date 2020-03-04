#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>

namespace Ui {
class ClientWidget;
}

/*!
 * \brief Базовый класс клиентских виджетов ClientWidget class
 */
class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = 0);
    ~ClientWidget();

    /*!
     * \brief Виртуальный метод, возвращающий уникальный идентификатор виджета
     */
    virtual QString uid() = 0;

    /*!
     * \brief Виртуальный метод, возвращающий название виджета
     */
    virtual QString name() = 0;

    virtual void onDBConnect() {}

    virtual void onDBDisconnect() {}

    /*!
     * \brief Вызывается после показа виджета
     */
    virtual void onShow() {}
    /*!
     * \brief Вызывается перед прятанием виджета
     */
    virtual void onHide() {}

    /*!
     * \brief Виртуальный метод, возвращающий true, если допустимы внешние элементы управления (глобальное меню и т.д.)
     */
    virtual bool isExternalControl() = 0;

private:
    Ui::ClientWidget *ui;
};

#endif // CLIENTWIDGET_H
