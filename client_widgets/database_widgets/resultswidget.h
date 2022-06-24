#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <QWidget>
#include <QItemSelectionModel>

namespace Ui {
class ResultsWidget;
}

class TestsModel;
class TestProxyModel;

/*!
 * \brief Класс масштабируемого pixmap ScaledPixmap
 */
class ScaledPixmap : public QWidget
{
public:
      ScaledPixmap(QWidget *parent = 0);

      void setScaledPixmap(const QPixmap &pixmap);

protected:
      void paintEvent(QPaintEvent *event);

private:
      QPixmap m_pixmap;
};


/*!
 * \brief Класс виджета для отображения списка проведенных тестов и результатов проведенного теста ResultsWidget class
 */
class ResultsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultsWidget(QWidget *parent = nullptr);
    ~ResultsWidget() override;

    void onDbConnect();
    void onDbDisconnect();

    static QString widgetName() {return QString("result");}

private slots:

    void on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    /*!
     * \brief Выбор теста для просмотра результатов
     * \param index - uid теста
     */
    void selectTest(const QModelIndex &index);

    void closeTest();

    void splitterMoved(int pos,int index);

    void onNewTests(const QModelIndex &parent, int first, int last);

    void onRemoveTests(const QModelIndex &parent, int first, int last);

    void on_selectPatient(const QString &patientUid);
    void on_selectMetodic(const QString &metodicUid);

    void onEntered(QModelIndex index);

    /*!
     * \brief Нажали кнопку повторного открытия сбойного теста
     */
    void onPressButtonOpenTestAgain();
    /*!
     * \brief Нажали кнопку удаления сбойного теста
     */
    void onPressButtonRemoveBadTest();

private:
    Ui::ResultsWidget *ui;

    /*!
     * \brief Функция открытия теста по его uid
     * \param testUid
     */
    void openTest(const QString testUid);

    void closeTestIfNotSelection();

    void saveSplitterPosition();
    void restoreSplitterPosition();

    void calculateSelected();

    TestsModel *m_mdlTest;
    TestProxyModel *m_pmdlTest;

    //ScaledPixmap *m_pmp;

    QWidget *m_wgtResult {nullptr};  ///< Текущий виджет для показа результатов теста
    int m_selectedRow {-1};          ///< Номер выбранной строки в списке
    QString m_uidOpenedTest {""};    ///< uid открытого теста

};

#endif // RESULTSWIDGET_H
