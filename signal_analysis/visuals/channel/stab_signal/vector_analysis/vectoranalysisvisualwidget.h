#ifndef VECTORANALYSISVISUALWIDGET_H
#define VECTORANALYSISVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class VectorAnalysisVisualWidget;
}

class VectorFactors;

class VectorAnalysisVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit VectorAnalysisVisualWidget(VisualDescriptor* visual,
                                        const QString& testUid, const QString& probeUid, const QString& channelId,
                                        QWidget *parent = nullptr);
    ~VectorAnalysisVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    void splitterMoved(int pos,int index);
    void on_plusButtonClicked();
    void on_minusButtonClicked();

private:
    Ui::VectorAnalysisVisualWidget *ui;

    void showFactors();
    void showAccumulationFunction();
    void showVectorCloud();

    void saveSplitterPosition();
    void restoreSplitterPosition();

    VectorFactors *m_factors {nullptr};
};

#endif // VECTORANALYSISVISUALWIDGET_H
