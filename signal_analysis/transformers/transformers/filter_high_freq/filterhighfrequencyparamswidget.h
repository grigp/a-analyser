#ifndef FILTERHIGHFREQUENCYPARAMSWIDGET_H
#define FILTERHIGHFREQUENCYPARAMSWIDGET_H

#include <QWidget>

#include "signaltransformerparamswidget.h"

namespace Ui {
class FilterHighFrequencyParamsWidget;
}

class FilterHighFrequencyParamsWidget : public SignalTransformerParamsWidget
{
    Q_OBJECT

public:
    explicit FilterHighFrequencyParamsWidget(QWidget *parent = nullptr);
    ~FilterHighFrequencyParamsWidget() override;

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
    Ui::FilterHighFrequencyParamsWidget *ui;
};

#endif // FILTERHIGHFREQUENCYPARAMSWIDGET_H
