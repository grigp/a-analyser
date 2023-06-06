#ifndef AUTOCORRELATIONANALYSISVISUALWIDGET_H
#define AUTOCORRELATIONANALYSISVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class AutoCorrelationAnalysisVisualWidget;
}

class PulseFactors;
class AnySignal;

/*!
 * \brief Класс виджета визуализатора автокорреляционного анализа The AutoCorrelationAnalysisVisualWidget class
 */
class AutoCorrelationAnalysisVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit AutoCorrelationAnalysisVisualWidget(VisualDescriptor* visual,
                                                 const QString& testUid, const QString& probeUid, const QString& channelId,
                                                 QWidget *parent = nullptr);
    ~AutoCorrelationAnalysisVisualWidget() override;

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
    Ui::AutoCorrelationAnalysisVisualWidget *ui;

    void showDiag();
    void showFactors();

    PulseFactors *m_factors {nullptr};
    AnySignal *m_diagData {nullptr};
};

#endif // AUTOCORRELATIONANALYSISVISUALWIDGET_H
