#ifndef RITMOGRAMVISUALWIDGET_H
#define RITMOGRAMVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class RitmogramVisualWidget;
}

class PulseFactors;

/*!
 * \brief Класс виджета визуализатора сигнала ритмограммы The RitmogramVisualWidget class
 */
class RitmogramVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit RitmogramVisualWidget(VisualDescriptor* visual,
                                   const QString& testUid, const QString& probeUid, const QString& channelId,
                                   QWidget *parent = nullptr);
    ~RitmogramVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private:
    Ui::RitmogramVisualWidget *ui;

    PulseFactors *m_factors {nullptr};
};

#endif // RITMOGRAMVISUALWIDGET_H
