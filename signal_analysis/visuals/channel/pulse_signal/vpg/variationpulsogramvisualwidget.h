#ifndef VARIATIONPULSOGRAMVISUALWIDGET_H
#define VARIATIONPULSOGRAMVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class VariationPulsogramVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора вариационной пульсограммы The VariationPulsogramVisualWidget class
 */
class VariationPulsogramVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit VariationPulsogramVisualWidget(VisualDescriptor* visual,
                                            const QString& testUid, const QString& probeUid, const QString& channelId,
                                            QWidget *parent = nullptr);
    ~VariationPulsogramVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private:
    Ui::VariationPulsogramVisualWidget *ui;
};

#endif // VARIATIONPULSOGRAMVISUALWIDGET_H
