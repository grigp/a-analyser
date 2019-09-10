#ifndef METHODSWIDGET_H
#define METHODSWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include <QItemSelectionModel>

namespace Ui {
class MethodsWidget;
}

class MetodicsModel;

class MethodsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MethodsWidget(QWidget *parent = 0);
    ~MethodsWidget();

    void onDbConnect();

private slots:
    void on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void selectMetodic(const QModelIndex index);
    void editMetodParams();

    void unselect();

private:
    Ui::MethodsWidget *ui;
    MetodicsModel *m_model;
};

#endif // METHODSWIDGET_H
