#ifndef SKGVISUALWIDGET_H
#define SKGVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class SKGVisualWidget;
}

class Stabilogram;
class ClassicFactors;

class SKGVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit SKGVisualWidget(VisualDescriptor* visual,
                             const QString& testUid, const QString& probeUid, const QString& channelId,
                             QWidget *parent = nullptr);
    ~SKGVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    void splitterMoved(int pos,int index);

    void on_btnPlusClicked();
    void on_btnMinusClicked();
    void on_btnZeroingClicked(bool isZeroing);

private:
    Ui::SKGVisualWidget *ui;


    void showSKG();
    void showFactors();

    void setSizeSKG();

    Stabilogram* m_stab {nullptr};
    ClassicFactors* m_factors {nullptr};
};

#endif // SKGVISUALWIDGET_H
