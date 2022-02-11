#ifndef SIGNALANALYSISWIDGET_H
#define SIGNALANALYSISWIDGET_H

#include <QWidget>
#include "clientwidget.h"

namespace Ui {
class SignalAnalysisWidget;
}

namespace ClientWidgets
{
    static const QString uidSignalAnalysisWidgetUid = "CW_SignalAnalysis";
}


class SignalAnalysisWidget : public ClientWidget
{
    Q_OBJECT

public:
    explicit SignalAnalysisWidget(QWidget *parent = nullptr);
    ~SignalAnalysisWidget() override;

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
    void splitterMoved(int pos,int index);

private:
    void saveSplitterPosition();
    void restoreSplitterPosition();

    /*!
     * \brief Открывает тест с заданным uid
     * \param uid - uid теста
     */
    void openTest(const QString uid);

    /*!
     * \brief Закрывает тест с заданным uid
     * \param uid - uid теста
     */
    void closeTest(const QString uid);

private:
    Ui::SignalAnalysisWidget *ui;
};

#endif // SIGNALANALYSISWIDGET_H
