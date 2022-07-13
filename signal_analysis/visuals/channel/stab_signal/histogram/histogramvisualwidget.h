#ifndef HISTOGRAMVISUALWIDGET_H
#define HISTOGRAMVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class HistogramVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора гистограммы The HistogramVisualWidget class
 */
class HistogramVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit HistogramVisualWidget(VisualDescriptor* visual,
                                   const QString& testUid, const QString& probeUid, const QString& channelUid,
                                   QWidget *parent = nullptr);
    ~HistogramVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private:
    Ui::HistogramVisualWidget *ui;
};

#endif // HISTOGRAMVISUALWIDGET_H
