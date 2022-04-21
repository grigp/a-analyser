#ifndef MYOGRAMSIGNALVISUALWIDGET_H
#define MYOGRAMSIGNALVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

class Myogram;

namespace Ui {
class MyogramSignalVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора сигнала миограммы The MyogramSignalVisualWidget class
 */
class MyogramSignalVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit MyogramSignalVisualWidget(VisualDescriptor* visual,
                                       const QString& testUid, const QString& probeUid, const QString& channelUid,
                                       QWidget *parent = nullptr);
    ~MyogramSignalVisualWidget() override;

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
    Ui::MyogramSignalVisualWidget *ui;

    void showGraph();

    Myogram* m_signal {nullptr};
};

#endif // MYOGRAMSIGNALVISUALWIDGET_H
