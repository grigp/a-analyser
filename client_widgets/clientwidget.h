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
    explicit ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget() override;

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

    /*!
     * \brief Виртуальный метод, возвращающий true, если виджет разрешает закрыться приложению
     * Чтобы запретить закрытие, нужно вернуть false
     */
    virtual bool isCloseProgramQuery() = 0;

private:
    Ui::ClientWidget *ui;
};

#endif // CLIENTWIDGET_H
