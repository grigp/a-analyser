#ifndef METHODSWIDGET_H
#define METHODSWIDGET_H

#include <QWidget>
#include <QModelIndex>

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

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void selectMetodic(const QModelIndex index);
    void editMetodParams();

private:
    Ui::MethodsWidget *ui;
    MetodicsModel *m_model;
};

#endif // METHODSWIDGET_H
