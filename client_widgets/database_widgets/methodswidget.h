#ifndef METHODSWIDGET_H
#define METHODSWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include <QPushButton>
#include <QItemSelectionModel>

namespace Ui {
class MethodsWidget;
}

class MetodicsModel;
class MetodicsKindModel;
class MethodicProxyModel;

class MethodsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MethodsWidget(QWidget *parent = 0);
    ~MethodsWidget();

    void onDbConnect();

//protected:
//    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_selectMetodicChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void selectMetodic(const QModelIndex index);
    void editMetodParams();
    void unselectMetodic();

    void on_btnKindPressed();

private:
    Ui::MethodsWidget *ui;

    void setMethodicKindsButtons();

    MetodicsModel *m_mdlMethodics;
    MethodicProxyModel *m_pmdlMethodics;
    MetodicsKindModel *m_mdlKinds;

    QMap<QPushButton*, QString> m_btnToKindUid;
};

#endif // METHODSWIDGET_H
