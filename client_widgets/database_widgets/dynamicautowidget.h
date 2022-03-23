#ifndef DYNAMICAUTOWIDGET_H
#define DYNAMICAUTOWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include <QSet>
#include <QItemSelectionModel>

namespace Ui {
class DynamicAutoWidget;
}

class DynamicDataModel;

/*!
 * \brief Класс виджета для построения автоматической динамики показателей DynamicAutoWidget class
 */
class DynamicAutoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DynamicAutoWidget(QWidget *parent = 0);
    ~DynamicAutoWidget();

    void onDbConnect();
    void onDbDisconnect();

    static QString widgetName() {return QString("dynamic");}

private slots:
    void on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void on_selectPatient(const QString &patientUid);
    void on_selectMetodic(const QString &metodicUid);
    void on_selectTest(const QString &testUid);
    void on_removeTest(const QString &testUid);

    void selectFactor(const QModelIndex index);

    void dynamicAsGraph();
    void dynamicAsBar();
    void dynamic3D(bool pressed);

    /*!
     * \brief Выбор итема мышкой
     * \param idx - индекс выбранного итема
     */
    void on_selectItem(const int idx);


private:
    Ui::DynamicAutoWidget *ui;

    void buildDynamic();
    void hideDynamic();

    void fillTable();
    void showDynamic();

    void showGraph(const int row);

    void saveDynamicKind(const int kindCode) const;
    void restoreDynamicKind();

    void saveDynamicVolume(const int volumeCode) const;
    void restoreDynamicVolume();

    DynamicDataModel *m_mdlDynamic {nullptr};
    QString m_selectedPatientUid {""};
    QString m_selectedMetodicUid {""};
    int m_selectRow {-1};
    QList<QString> m_tests; ///< uid-ы тестов, участвующих в построении динамики
};

#endif // DYNAMICAUTOWIDGET_H
