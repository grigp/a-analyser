#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <QWidget>

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
    explicit ResultsWidget(QWidget *parent = 0);
    ~ResultsWidget();

    void onDbConnect();

    static QString widgetName() {return QString("result");}

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:

    /*!
     * \brief Выбор теста для просмотра результатов
     * \param index - uid теста
     */
    void selectTest(const QModelIndex &index);

    void closeTest();

    void splitterMoved(int pos,int index);

    void onNewTests(const QModelIndex &parent, int first, int last);

    void onRemoveTests(const QModelIndex &parent, int first, int last);

private:
    Ui::ResultsWidget *ui;

    void saveSplitterPosition();
    void restoreSplitterPosition();

    TestsModel *m_mdlTest;
    TestProxyModel *m_pmdlTest;

    //ScaledPixmap *m_pmp;

    QWidget *m_wgtResult {nullptr};  ///< Текущий виджет для показа результатов теста
    int m_selectedRow {-1};          ///< Номер выбранной строки в списке

};

#endif // RESULTSWIDGET_H
