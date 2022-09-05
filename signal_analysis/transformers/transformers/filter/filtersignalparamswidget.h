#ifndef FILTERSIGNALPARAMSWIDGET_H
#define FILTERSIGNALPARAMSWIDGET_H

#include <QWidget>
#include <QJsonObject>

#include "signaltransformerparamswidget.h"

namespace Ui {
class FilterSignalParamsWidget;
}

class FilterSignalParamsWidget : public SignalTransformerParamsWidget
{
    Q_OBJECT

public:
    explicit FilterSignalParamsWidget(QWidget *parent = nullptr);
    ~FilterSignalParamsWidget();

    /*!
     * \brief Задает параметры преобразователя в виде json объекта извне
     * \param params - параметры в виде json объекта
     */
    void setParams(const QJsonObject &params) override;

    /*!
     * \brief Возвращает параметры преобразователя в виде json объекта
     */
    QJsonObject params() const override;

private:
    Ui::FilterSignalParamsWidget *ui;
};

#endif // FILTERSIGNALPARAMSWIDGET_H
