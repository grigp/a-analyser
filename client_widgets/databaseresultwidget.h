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
    explicit DataBaseResultWidget(QWidget *parent = nullptr);
    ~DataBaseResultWidget() override;

    /*!
     * \brief Виртуальный метод, возвращающий уникальный идентификатор виджета
     */
    QString uid() override;

    /*!
     * \brief Виртуальный метод, возвращающий название виджета
     */
    QString name() override;

    void onDBConnect() override;

    void onDBDisconnect() override;

    /*!
     * \brief Виртуальный метод, возвращающий true, если допустимы внешние элементы управления (глобальное меню и т.д.)
     */
    bool isExternalControl() override {return true;}

private slots:
    void onSelectTest(const QString &testUid);

    void splitterVMoved(int pos,int index);
    void splitterHMoved(int pos,int index);

    void on_applicationParamChanged(const QString &group, const QString &param, const QVariant &value);
    void on_methodicCount(const int count);


private:
    Ui::DataBaseResultWidget *ui;

    void saveSplittersPosition();
    void restoreSplittersPosition();

    QWidget *m_wgtResult {nullptr};  ///< Текущий виджет для показа результатов теста
    int m_metCnt {1};
};

#endif // DATABASERESULTWIDGET_H
