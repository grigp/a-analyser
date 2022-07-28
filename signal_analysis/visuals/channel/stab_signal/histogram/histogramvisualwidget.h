#ifndef HISTOGRAMVISUALWIDGET_H
#define HISTOGRAMVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class HistogramVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора гистограммы The HistogramVisualWidget class
 */
class HistogramVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    static const int HistBarsCount = 19;

    explicit HistogramVisualWidget(VisualDescriptor* visual,
                                   const QString& testUid, const QString& probeUid, const QString& channelUid,
                                   QWidget *parent = nullptr);
    ~HistogramVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    /*!
     * \brief Возникает при выборе итема мышкой
     * \param idx - индекс выбранного итема
     */
    void on_selectItem(const int idx);


private:
    Ui::HistogramVisualWidget *ui;

    double m_dataX[HistBarsCount];
    double m_dataY[HistBarsCount];

    double m_minX {INT_MAX};
    double m_maxX {-INT_MAX};
    double m_minY {INT_MAX};
    double m_maxY {-INT_MAX};
    double m_sizeOneX = {1};
    double m_sizeOneY = {1};

    void computeHist();
    void showHist();

    void spectrTest();
};

#endif // HISTOGRAMVISUALWIDGET_H
