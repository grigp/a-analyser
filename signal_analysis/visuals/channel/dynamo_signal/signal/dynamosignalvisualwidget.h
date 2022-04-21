#ifndef DYNAMOSIGNALVISUALWIDGET_H
#define DYNAMOSIGNALVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

class DynamoSignal;

namespace Ui {
class DynamoSignalVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора сигнала силомера The DynamoSignalVisualWidget class
 */
class DynamoSignalVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit DynamoSignalVisualWidget(VisualDescriptor* visual,
                                      const QString& testUid, const QString& probeUid, const QString& channelUid,
                                      QWidget *parent = nullptr);
    ~DynamoSignalVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    void scaleChange(int idx);

    void btnFulSignalClicked(bool isFullSignal);

    void btnPlusClicked();
    void btnMinusClicked();

    void signalScroll(int pos);

    void on_moveCursor();

private:
    Ui::DynamoSignalVisualWidget *ui;

    void showGraph();

    DynamoSignal* m_signal {nullptr};
};

#endif // DYNAMOSIGNALVISUALWIDGET_H
