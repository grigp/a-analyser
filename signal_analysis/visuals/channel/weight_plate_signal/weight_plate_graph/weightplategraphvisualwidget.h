#ifndef WEIGHTPLATEGRAPHVISUALWIDGET_H
#define WEIGHTPLATEGRAPHVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class WeightPlateGraphVisualWidget;
}

class WeightPlateSignal;

/*!
 * \brief Класс виджета визуализатора графика динамики веса The WeightPlateGraphVisualWidget class
 */
class WeightPlateGraphVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit WeightPlateGraphVisualWidget(VisualDescriptor* visual,
                                          const QString& testUid, const QString& probeUid, const QString& channelUid,
                                          QWidget *parent = nullptr);
    ~WeightPlateGraphVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    void scaleChange(int idx);

    void btnFulSignalClicked(bool isFullSignal);

    void btnPlusClicked();
    void btnMinusClicked();

    void signalScroll(int pos);

    /*!
     * \brief Выбор канала
     * \param chanIdx - номер канала (0 - все, 1, n - номера каналов)
     */
    void on_selectChannel(int chanIdx);

    //void on_moveCursor();

private:
    Ui::WeightPlateGraphVisualWidget *ui;

    void getSignal();

    void showGraph(const int chanIdx);

    WeightPlateSignal* m_signal {nullptr};
};

#endif // WEIGHTPLATEGRAPHVISUALWIDGET_H
