#ifndef SUMMARIESWIDGET_H
#define SUMMARIESWIDGET_H

#include <QWidget>

#include "clientwidget.h"
#include "summarydefines.h"

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

private slots:
    /*!
     * \brief Сигнал добавления теста в сводку
     * \param testUid - uid теста
     * \param mode - режим: в существующую, в новую, в активную
     * \param summaryName - имя сводки для новой или имя файла для существующей
     * \param kind - тип сводки: первичные / все показатели
     */
    void addTestToSummary(const QString testUid,
                          const SummaryDefines::AddTestMode mode,
                          const QString summaryName,
                          const SummaryDefines::Kind kind);


private:
    Ui::SummariesWidget *ui;

    void testTable();
};

#endif // SUMMARIESWIDGET_H
