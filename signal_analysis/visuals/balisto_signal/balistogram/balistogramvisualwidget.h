#ifndef BALISTOGRAMVISUALWIDGET_H
#define BALISTOGRAMVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

class Balistogram;

namespace Ui {
class BalistogramVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора ,балистограммы The BalistogramVisualWidget class
 */
class BalistogramVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit BalistogramVisualWidget(VisualDescriptor* visual,
                                     const QString& testUid, const QString& probeUid, const QString& channelUid,
                                     QWidget *parent = nullptr);
    ~BalistogramVisualWidget() override;

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
    Ui::BalistogramVisualWidget *ui;

    void showGraph();

    Balistogram* m_z {nullptr};
};

#endif // BALISTOGRAMVISUALWIDGET_H
