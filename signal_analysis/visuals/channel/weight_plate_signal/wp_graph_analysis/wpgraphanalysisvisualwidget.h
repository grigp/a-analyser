#ifndef WPGRAPHANALYSISVISUALWIDGET_H
#define WPGRAPHANALYSISVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class WPGraphAnalysisVisualWidget;
}

class WeightPlateSignal;
class DecartCoordinatesSignal;
class AreaGraph;

class WPGraphAnalysisVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit WPGraphAnalysisVisualWidget(VisualDescriptor* visual,
                                         const QString& testUid, const QString& probeUid, const QString& channelUid,
                                         QWidget *parent = nullptr);
    ~WPGraphAnalysisVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    void scaleChange(int idx);

    void btnFulSignalClicked(bool isFullSignal);

    void btnPlusClicked();
    void btnMinusClicked();

    void signalScroll(int pos);

    void on_signalExport();
    void on_apnoeFactorsCalculate();

private:
    Ui::WPGraphAnalysisVisualWidget *ui;

    void getSignal();
    void filtration();
    void filtrationY(QVector<double> &arr);
    void showGraph();

    int scaleMultiplier(const int scaleIdx) const;
    void setDiapazones(DecartCoordinatesSignal* signal, AreaGraph* area);

    bool m_isCalculate {false};
    WeightPlateSignal* m_signal {nullptr};
    DecartCoordinatesSignal* m_signalZ {nullptr};
    DecartCoordinatesSignal* m_signalY {nullptr};
    QVector<double> m_fltZ;
    QVector<double> m_fltY;
    QList<QVector<double>> m_fragments; ///< Фрагменты сигнала в покое для анализа дыхания

    int m_apnoeFactsCount {0};          ///< Кол-во приступов апноэ
    double m_apnoeFactTimeAverage {0};  ///< Средняя продолжительность приступа апноэ
    double m_apnoeFactTimeMax {0};      ///< Максимальная продолжительность приступа апноэ
};

#endif // WPGRAPHANALYSISVISUALWIDGET_H
