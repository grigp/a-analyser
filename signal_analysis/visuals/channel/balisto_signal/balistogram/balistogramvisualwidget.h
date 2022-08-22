#ifndef BALISTOGRAMVISUALWIDGET_H
#define BALISTOGRAMVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

class Balistogram;

namespace Ui {
class BalistogramVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора балистограммы The BalistogramVisualWidget class
 */
class BalistogramVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit BalistogramVisualWidget(VisualDescriptor* visual,
                                     const QString& testUid, const QString& probeUid, const QString& channelUid,
                                     QWidget *parent = nullptr);
    ~BalistogramVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    void scaleChange(int idx);

    void btnFulSignalClicked(bool isFullSignal);

    void btnPlusClicked();
    void btnMinusClicked();

    void signalScroll(int pos);

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
    Ui::BalistogramVisualWidget *ui;

    void showGraph();

    Balistogram* m_z {nullptr};
    int m_selBeg {-1};
};

#endif // BALISTOGRAMVISUALWIDGET_H
