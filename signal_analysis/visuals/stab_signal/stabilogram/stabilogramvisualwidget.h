#ifndef STABILOGRAMVISUALWIDGET_H
#define STABILOGRAMVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

class Stabilogram;

namespace Ui {
class StabilogramVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора стабилограммы The StabilogramVisualWidget class
 */
class StabilogramVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit StabilogramVisualWidget(VisualDescriptor* visual,
                                     const QString& testUid, const QString& probeUid, const QString& channelUid,
                                     QWidget *parent = nullptr);
    ~StabilogramVisualWidget() override;

    bool isValid() override;
    void calculate() override;


private:
    Ui::StabilogramVisualWidget *ui;

    void showGraph();

    Stabilogram* m_stab {nullptr};
};

#endif // STABILOGRAMVISUALWIDGET_H
