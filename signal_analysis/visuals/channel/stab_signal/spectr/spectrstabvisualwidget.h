#ifndef SPECTRSTABVISUALWIDGET_H
#define SPECTRSTABVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class SpectrStabVisualWidget;
}

class SpectrStabFactors;

class SpectrStabVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit SpectrStabVisualWidget(VisualDescriptor* visual,
                                    const QString& testUid, const QString& probeUid, const QString& channelUid,
                                    QWidget *parent = nullptr);
    ~SpectrStabVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    void splitterMoved(int pos, int index);

private:
    Ui::SpectrStabVisualWidget *ui;

    void showTable();

    void saveSplitterPosition();
    void restoreSplitterPosition();

    SpectrStabFactors* m_factors {nullptr};
};

#endif // SPECTRSTABVISUALWIDGET_H
