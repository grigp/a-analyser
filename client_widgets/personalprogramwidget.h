#ifndef PERSONALPROGRAMWIDGET_H
#define PERSONALPROGRAMWIDGET_H

#include <QWidget>

#include "clientwidget.h"
#include "metodicdefines.h"

namespace Ui {
class PersonalProgramWidget;
}

namespace ClientWidgets
{
    static const QString uidPersonalProgramWidgetUid = "CW_PersonalProgram";
}


class PersonalProgramWidget : public ClientWidget
{
    Q_OBJECT

public:
    explicit PersonalProgramWidget(QWidget *parent = nullptr);
    ~PersonalProgramWidget() override;

    /*!
     * \brief Виртуальный метод, возвращающий уникальный идентификатор виджета
     */
    QString uid() override;

    /*!
     * \brief Виртуальный метод, возвращающий название виджета
     */
    QString name() override;

    /*!
     * \brief Виртуальный метод, возвращающий true, если допустимы внешние элементы управления (глобальное меню и т.д.)
     */
    bool isExternalControl() override {return true;}

    /*!
     * \brief Виртуальный метод, возвращающий true, если виджет разрешает закрыться приложению
     * Чтобы запретить закрытие, нужно вернуть false
     */
    bool isCloseProgramQuery() override {return true;}

    /*!
     * \brief Вызывается после показа виджета
     */
    void onShow() override;
    /*!
     * \brief Вызывается перед прятанием виджета
     */
    void onHide() override;

private slots:
    void on_splitterMoved(int,int);

    void on_run();
    void on_delete();
    void on_params();

private:
    Ui::PersonalProgramWidget *ui;

    void saveSplitterPosition();
    void restoreSplitterPosition();
};

#endif // PERSONALPROGRAMWIDGET_H
