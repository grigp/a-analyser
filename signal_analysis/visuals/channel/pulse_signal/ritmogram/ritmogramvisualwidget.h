#ifndef RITMOGRAMVISUALWIDGET_H
#define RITMOGRAMVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class RitmogramVisualWidget;
}

class PulseFactors;
class Ritmogram;

/*!
 * \brief Класс виджета визуализатора сигнала ритмограммы The RitmogramVisualWidget class
 */
class RitmogramVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit RitmogramVisualWidget(VisualDescriptor* visual,
                                   const QString& testUid, const QString& probeUid, const QString& channelId,
                                   QWidget *parent = nullptr);
    ~RitmogramVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    /*!
     * \brief Сигнал нажатия мышки на теле виджета
     * \param x, y - координаты нажатия
    * \param buttons - нажатые кнопки
     */
    void on_press(const int x, const int y, const Qt::MouseButtons buttons);
    /*!
     * \brief Сигнал отпускания мышки на теле виджета
     * \param x, y - координаты отпускания
    * \param buttons - нажатые кнопки
     */
    void on_release(const int x, const int y, const Qt::MouseButtons buttons);
    /*!
     * \brief Сигнал переноса мышки по телу виджета
     * \param x, y - координаты положения
    * \param buttons - нажатые кнопки
     */
    void on_move(const int x, const int y, const Qt::MouseButtons buttons);

private:
    Ui::RitmogramVisualWidget *ui;

    void showGraph();
    void showResume();
    QString getStyleByValue(const int value) const;

    PulseFactors *m_factors {nullptr};
    Ritmogram *m_signal {nullptr};
    int m_selBeg {-1};
};

#endif // RITMOGRAMVISUALWIDGET_H
