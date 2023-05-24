#ifndef VARIATIONPULSOGRAMVISUALWIDGET_H
#define VARIATIONPULSOGRAMVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class VariationPulsogramVisualWidget;
}

class PulseFactors;

/*!
 * \brief Класс виджета визуализатора вариационной пульсограммы The VariationPulsogramVisualWidget class
 */
class VariationPulsogramVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit VariationPulsogramVisualWidget(VisualDescriptor* visual,
                                            const QString& testUid, const QString& probeUid, const QString& channelId,
                                            QWidget *parent = nullptr);
    ~VariationPulsogramVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    /*!
     * \brief Действия по изменению данных канала в БД
     * \param probeUid - uid пробы
     * \param channelId - id канала
     */
    void on_channelChanged(const QString &probeUid, const QString &channelId);

private:
    Ui::VariationPulsogramVisualWidget *ui;

    void showVPG();
    void showFactors();

    PulseFactors *m_factors {nullptr};
};

#endif // VARIATIONPULSOGRAMVISUALWIDGET_H
