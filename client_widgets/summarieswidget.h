#ifndef SUMMARIESWIDGET_H
#define SUMMARIESWIDGET_H

#include <QWidget>

#include "clientwidget.h"

namespace Ui {
class SummariesWidget;
}

namespace ClientWidgets
{
    static const QString uidSummariesWidgetUid = "CW_Summaries";
}


class SummariesWidget : public ClientWidget
{
    Q_OBJECT

public:
    explicit SummariesWidget(QWidget *parent = nullptr);
    ~SummariesWidget() override;

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
    bool isExternalControl() override {return false;}

    /*!
     * \brief Вызывается после показа виджета
     */
    void onShow() override;
    /*!
     * \brief Вызывается перед прятанием виджета
     */
    void onHide() override;

private:
    Ui::SummariesWidget *ui;

    void testTable();
};

#endif // SUMMARIESWIDGET_H
