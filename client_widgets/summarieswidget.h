#ifndef SUMMARIESWIDGET_H
#define SUMMARIESWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

#include "clientwidget.h"
#include "summarydefines.h"


namespace Ui {
class SummariesWidget;
}

namespace ClientWidgets
{
    static const QString uidSummariesWidgetUid = "CW_Summaries";
}

class Summary;

class SummariesWidget : public ClientWidget
{
    Q_OBJECT

public:
    explicit SummariesWidget(QWidget *parent = nullptr);
    ~SummariesWidget() override;

    /*!
     * \brief Роли для доступа к данным в модели списка сводок The ListSummariesRoles enum
     */
    enum ListSummariesRoles
    {
        lsWidgetRole = Qt::UserRole + 1   ///< Содержит указатель на виджет со сводкой
    };

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
     * \brief Добавление теста в сводку
     * \param testUid - uid теста
     * \param mode - режим: в существующую, в новую, в активную
     * \param summaryName - имя сводки для новой или имя файла для существующей
     * \param kind - тип сводки: первичные / все показатели
     */
    void addTestToSummary(const QString testUid,
                          const SummaryDefines::AddTestMode mode,
                          const QString summaryName,
                          const SummaryDefines::Kind kind);

    void on_selectIndex(QModelIndex index);

    void on_openSummary();
    void on_saveSummary();
    void on_closeSummary();

    void splitterMoved(int pos,int index);

private:
    Ui::SummariesWidget *ui;

    /*!
     * \brief Открытие сводки. Первый этап - создание
     * \return Указатель на созданную сводку
     */
    Summary* openSummaryFirst();

    /*!
     * \brief Открытие сводки. Второй этап - расположение на виджете и т.д.
     * \param summary - указатель на созданную сводку в методе openSummaryFirst
     */
    void openSummarySecond(Summary* summary);

    /*!
     * \brief Добавление теста в новую сводку
     * \param testUid - uid теста
     * \param summaryName - имя сводки для новой или имя файла для существующей
     * \param kind - тип сводки: первичные / все показатели
     * \return true, если строку удалось добавить успешно
     */
    bool addTestToNewSummary(const QString testUid,
                             const QString summaryName,
                             const SummaryDefines::Kind kind);

    /*!
     * \brief Добавление теста в активную сводку
     * \param testUid - uid теста
     * \param kind - тип сводки: первичные / все показатели
     * \return true, если строку удалось добавить успешно
     */
    bool addTestToActiveSummary(const QString testUid,
                                const SummaryDefines::Kind kind);

    void hideAllWidgets();

    void saveSplitterPosition();
    void restoreSplitterPosition();

    QStandardItemModel* m_mdlLS {nullptr};
};

#endif // SUMMARIESWIDGET_H
