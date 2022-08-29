#ifndef FILTERLOWFREQUENCYPARAMSWIDGET_H
#define FILTERLOWFREQUENCYPARAMSWIDGET_H

#include <QWidget>

namespace Ui {
class FilterLowFrequencyParamsWidget;
}

#include "signaltransformerparamswidget.h"

class FilterLowFrequencyParamsWidget : public SignalTransformerParamsWidget
{
    Q_OBJECT

public:
    explicit FilterLowFrequencyParamsWidget(QWidget *parent = nullptr);
    ~FilterLowFrequencyParamsWidget() override;

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
    Ui::FilterLowFrequencyParamsWidget *ui;
};

#endif // FILTERLOWFREQUENCYPARAMSWIDGET_H
