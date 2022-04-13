#ifndef BILATERALVISUALWIDGET_H
#define BILATERALVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

class BilateralResultData;

namespace Ui {
class BilateralVisualWidget;
}

class BilateralVisualWidget : public ProbeVisual
{
    Q_OBJECT

public:
    explicit BilateralVisualWidget(VisualDescriptor* visual,
                                   const QString& testUid, const QString& probeUid,
                                   QWidget *parent = nullptr);
    ~BilateralVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private:
    Ui::BilateralVisualWidget *ui;

    void showPlatforms();

    int computeDiap();

    BilateralResultData* m_bData {nullptr};
};

#endif // BILATERALVISUALWIDGET_H
