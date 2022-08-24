#ifndef STABILOGRAMVISUALWIDGET_H
#define STABILOGRAMVISUALWIDGET_H

#include <QWidget>
#include <QMenu>

#include "visuals.h"

class Stabilogram;

namespace Ui {
class StabilogramVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора стабилограммы The StabilogramVisualWidget class
 */
class StabilogramVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit StabilogramVisualWidget(VisualDescriptor* visual,
                                     const QString& testUid, const QString& probeUid, const QString& channelId,
                                     QWidget *parent = nullptr);
    ~StabilogramVisualWidget() override;

    bool isValid() override;
    void calculate() override;


private slots:
    void scaleChange(int idx);

    void btnFulSignalClicked(bool isFullSignal);

    void btnZeriongClicked(bool isZeroing);

    void btnPlusClicked();
    void btnMinusClicked();

    void signalScroll(int pos);

    void on_popupMenuRequested(QPoint pos);

    void on_createSection();

    void on_moveCursor();
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
    Ui::StabilogramVisualWidget *ui;

    void showGraph();

    Stabilogram* m_stab {nullptr};
    int m_selBeg {-1};
    QMenu *m_menu {nullptr};
};

#endif // STABILOGRAMVISUALWIDGET_H
