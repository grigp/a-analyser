#ifndef BILATERALVISUALWIDGET_H
#define BILATERALVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

class BilateralResultData;
class Stabilogram;

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

private slots:
    void on_btnPlusClick();
    void on_btnMinusClick();

    void on_cbSKGCommonChecked(bool checked);
    void on_cbSKGLeftChecked(bool checked);
    void on_cbSKGRightChecked(bool checked);

    void on_btnZeroingNoClicked();
    void on_btnZeroingDifClicked();
    void on_btnZeroingCommonClicked();

private:
    Ui::BilateralVisualWidget *ui;

    int computeDiap();

    void showPlatforms();
    void showSKG();
    void showDiagram();

    int m_diap {128};
    BilateralResultData* m_bData {nullptr};
    Stabilogram* m_stab {nullptr};
    Stabilogram* m_stabLeft {nullptr};
    Stabilogram* m_stabRight {nullptr};
};

#endif // BILATERALVISUALWIDGET_H
