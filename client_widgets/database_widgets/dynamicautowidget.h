#ifndef DYNAMICAUTOWIDGET_H
#define DYNAMICAUTOWIDGET_H

#include <QWidget>
#include <QModelIndex>

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

    static QString widgetName() {return QString("dynamic");}

protected:
    bool eventFilter(QObject *obj, QEvent *event);


private slots:
    void on_selectPatient(const QString &patientUid);
    void on_selectMetodic(const QString &metodicUid);

    void selectFactor(const QModelIndex index);

private:
    Ui::DynamicAutoWidget *ui;

    void buildDynamic();
    void hideDynamic();

    void fillTable();
    void showDynamic();

    void showGraph(const int row);

    DynamicDataModel *m_mdlDynamic {nullptr};
    QString m_selectedPatientUid {""};
    QString m_selectedMetodicUid {""};
    int m_selectRow {-1};
};

#endif // DYNAMICAUTOWIDGET_H
