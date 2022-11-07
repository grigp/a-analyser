#ifndef MYOGRAMSIGNALVISUALWIDGET_H
#define MYOGRAMSIGNALVISUALWIDGET_H

#include <QWidget>
#include <QMenu>

#include "visuals.h"

class Myogram;

namespace Ui {
class MyogramSignalVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора сигнала миограммы The MyogramSignalVisualWidget class
 */
class MyogramSignalVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit MyogramSignalVisualWidget(VisualDescriptor* visual,
                                       const QString& testUid, const QString& probeUid, const QString& channelId,
                                       QWidget *parent = nullptr);
    ~MyogramSignalVisualWidget() override;

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

    /*!
     * \brief Выбор канала
     * \param chanIdx - номер канала (0 - все, 1, n - номера каналов)
     */
    void on_cbChannelsChanged(int idx);

private:
    Ui::MyogramSignalVisualWidget *ui;

    void getSignal();
    void showGraph(const int chanIdx = -1);

    Myogram* m_signal {nullptr};
    int m_selBeg {-1};
    QMenu *m_menu {nullptr};
};

#endif // MYOGRAMSIGNALVISUALWIDGET_H
