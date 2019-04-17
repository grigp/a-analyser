#ifndef METHODSWIDGET_H
#define METHODSWIDGET_H

#include <QWidget>

namespace Ui {
class MethodsWidget;
}

class MethodsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MethodsWidget(QWidget *parent = 0);
    ~MethodsWidget();

private:
    Ui::MethodsWidget *ui;
};

#endif // METHODSWIDGET_H
