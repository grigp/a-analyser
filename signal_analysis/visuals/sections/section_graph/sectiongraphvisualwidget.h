#ifndef SECTIONGRAPHVISUALWIDGET_H
#define SECTIONGRAPHVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class SectionGraphVisualWidget;
}

class AnySignal;

/*!
 * \brief Виджет визуализатора графика секции The SectionGraphVisualWidget class
 */
class SectionGraphVisualWidget : public SectionVisual
{
    Q_OBJECT

public:
    explicit SectionGraphVisualWidget(VisualDescriptor* visual,
                                      const QString& testUid, const QString& probeUid, const QString& channelId, const QString& sectionNumber,
                                      QWidget *parent = nullptr);
    ~SectionGraphVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    void scaleChange(int idx);

    void btnFulSignalClicked(bool isFullSignal);

    void btnPlusClicked();
    void btnMinusClicked();

    void signalScroll(int pos);

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

    void on_transform();

    void on_revert();

private:
    Ui::SectionGraphVisualWidget *ui;

    void updateSectionData();

    AnySignal *m_signal {nullptr};
    double m_absMax {0};
};

#endif // SECTIONGRAPHVISUALWIDGET_H
