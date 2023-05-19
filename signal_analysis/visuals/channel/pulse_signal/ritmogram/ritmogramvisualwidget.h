#ifndef RITMOGRAMVISUALWIDGET_H
#define RITMOGRAMVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class RitmogramVisualWidget;
}

class PulseFactors;
class Ritmogram;

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

    void showGraph();
    void showResume();
    QString getStyleByValue(const int value) const;

    PulseFactors *m_factors {nullptr};
    Ritmogram *m_signal {nullptr};
};

#endif // RITMOGRAMVISUALWIDGET_H
