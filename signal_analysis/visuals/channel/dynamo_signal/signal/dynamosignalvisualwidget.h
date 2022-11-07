#ifndef DYNAMOSIGNALVISUALWIDGET_H
#define DYNAMOSIGNALVISUALWIDGET_H

#include <QWidget>
#include <QMenu>

#include "visuals.h"

class DynamoSignal;

namespace Ui {
class DynamoSignalVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора сигнала силомера The DynamoSignalVisualWidget class
 */
class DynamoSignalVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit DynamoSignalVisualWidget(VisualDescriptor* visual,
                                      const QString& testUid, const QString& probeUid, const QString& channelId,
                                      QWidget *parent = nullptr);
    ~DynamoSignalVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    void scaleChange(int idx);

    void btnFulSignalClicked(bool isFullSignal);

    void btnPlusClicked();
    void btnMinusClicked();

    void signalScroll(int pos);

    void on_moveCursor();

    void on_popupMenuRequested(QPoint pos);

    void on_createSection();

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
    Ui::DynamoSignalVisualWidget *ui;

    void showGraph();

    DynamoSignal* m_signal {nullptr};
    int m_selBeg {-1};
    QMenu *m_menu {nullptr};
};

#endif // DYNAMOSIGNALVISUALWIDGET_H
