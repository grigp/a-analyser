#ifndef DYNAMICAUTOWIDGET_H
#define DYNAMICAUTOWIDGET_H

#include <QWidget>

namespace Ui {
class DynamicAutoWidget;
}

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

private:
    Ui::DynamicAutoWidget *ui;
};

#endif // DYNAMICAUTOWIDGET_H
