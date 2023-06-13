#ifndef SPECTRPULSEVISUALWIDGET_H
#define SPECTRPULSEVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

class PulseSpectrFactors;

namespace Ui {
class SpectrPulseVisualWidget;
}

class SpectrPulseVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit SpectrPulseVisualWidget(VisualDescriptor* visual,
                                     const QString& testUid, const QString& probeUid, const QString& channelId,
                                     QWidget *parent = nullptr);
    ~SpectrPulseVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    void splitterMoved(int pos, int index);

    /*!
     * \brief Действия по изменению данных канала в БД
     * \param probeUid - uid пробы
     * \param channelId - id канала
     */
    void on_channelChanged(const QString &probeUid, const QString &channelId);

private:
    Ui::SpectrPulseVisualWidget *ui;

    void showSpectr();
    void showFactors();
    void saveSplitterPosition();
    void restoreSplitterPosition();

    PulseSpectrFactors *m_factors {nullptr};
};

#endif // SPECTRPULSEVISUALWIDGET_H
