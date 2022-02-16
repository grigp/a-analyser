#ifndef VECTORANALYSISVISUALWIDGET_H
#define VECTORANALYSISVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class VectorAnalysisVisualWidget;
}

class VectorAnalysisVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit VectorAnalysisVisualWidget(VisualDescriptor* visual,
                                        const QString& testUid, const QString& probeUid, const QString& channelUid,
                                        QWidget *parent = nullptr);
    ~VectorAnalysisVisualWidget();

    bool isValid() override;
    void calculate() override;


private:
    Ui::VectorAnalysisVisualWidget *ui;
};

#endif // VECTORANALYSISVISUALWIDGET_H
