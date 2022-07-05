#ifndef SIGNALANALYSISWIDGET_H
#define SIGNALANALYSISWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QStandardItemModel>

#include "clientwidget.h"
#include "metodicdefines.h"

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

    ///< Номера колонок
    enum Columns
    {
          ColElement = 0
        , ColButtons
    };

    /*!
     * \brief Действия по нажатии кнопок The ButtonActions enum
     */
    enum ButtonActions
    {
          baClose = 0 ///< Закрыть тест
        , baExport    ///< Экспорт сигналов
    };

    ///< Роли для доступа к данным
    enum TestsTreeModelRoles
    {
          UidRole = Qt::UserRole + 1     ///< uid элемента (теста, пробы, канала). QString в итеме элеметнов ColElement
        , ProbeUidRole                   ///< uid пробы
        , ChannelUidRole                 ///< uid канала, именно реальной записи канала, а не channelID
        , ButtonActionRole               ///< действие на кнопке. Столбец ColButtons. Возвращает bsClose или baExport
        , TabWidgetRole                  ///< указатель на табулированный виджет, содержащий линейку визуализаторов
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

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void splitterMoved(int pos,int index);

    void selectElement(QModelIndex index);

private:
    void saveSplitterPosition();
    void restoreSplitterPosition();

    /*!
     * \brief Открывает тест с заданным uid
     * \param testUid - uid теста
     */
    void openTest(const QString testUid);

    /*!
     * \brief Устанавливает курсор на первую линейку визуализаторов открытого теста
     * \param testUid - uid теста
     */
    void selectTest(const QString testUid);

    /*!
     * \brief Поиск итема на дереве теста с линейкой визуализаторов
     * \param index  начиная с какого индекса искать
     * \return индекс итема с линейкой визуализаторов или QModelIndex(), если не найдено
     */
    QModelIndex getFirstVisualLineIndex(QModelIndex &index);

    /*!
     * \brief Закрывает тест
     * \param index - индекс, на котором находится кнопка рядом с тестом, который надо закрыть
     */
    void closeTest(QModelIndex& index);

    void closeVisuals(QModelIndex& index);

    QString getMethodName(const QString& metUid);

    /*!
     * \brief Экспор сигнала
     * \param index - индекс, на котором находится кнопка рядом с каналом, сигнал которого надо экспортировать
     */
    void signalExport(QModelIndex& index);

    /*!
     * \brief Возвращает список uid открытых тестов
     */
    QList<QString> getTests();

    /*!
     * \brief Возвращает true, если тест с заданным uid открыт
     * \param testUid - uid теста
     */
    bool isTestOpened(const QString &testUid);

    /*!
     * \brief Рассчитывает линейку визуализаторов для элемента на дереве теста
     * \param count - кол-во визуализаторов
     * \param testUid - uid теста
     * \param probeUid - uid пробы
     * \param channelId - идентификатор канала
     * \return указатель на виджет табуляции, связанный с элементом на дереве теста и nulptr, если нет визуализаторов
     */
    QTabWidget* calculateVisualsLine(int &count,
                                     const QString &testUid, const QString &probeUid = "", const QString &channelId = "");


    QStandardItemModel* m_mdlTests {nullptr};
    QList<MetodicDefines::MetodicInfo> m_metList;

private:
    Ui::SignalAnalysisWidget *ui;
};

#endif // SIGNALANALYSISWIDGET_H
