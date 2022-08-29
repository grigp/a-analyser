#ifndef SIGNALTRANSFORMERPARAMSWIDGET_H
#define SIGNALTRANSFORMERPARAMSWIDGET_H

#include <QWidget>
#include "QJsonObject"

namespace Ui {
class SignalTransformerParamsWidget;
}

/*!
 * \brief Суперкласс виджета параметров преобразователя сигнала The SignalTransformerParamsWidget class
 */
class SignalTransformerParamsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SignalTransformerParamsWidget(QWidget *parent = nullptr);
    ~SignalTransformerParamsWidget();

    /*!
     * \brief Задает параметры преобразователя в виде json объекта извне
     * \param params - параметры в виде json объекта
     */
    virtual void setParams(const QJsonObject &params) = 0;

    /*!
     * \brief Возвращает параметры преобразователя в виде json объекта
     */
    virtual QJsonObject params() const = 0;

private:
    Ui::SignalTransformerParamsWidget *ui;
};

#endif // SIGNALTRANSFORMERPARAMSWIDGET_H
