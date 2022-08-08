#ifndef SPECTRSTABVISUALWIDGET_H
#define SPECTRSTABVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class SpectrStabVisualWidget;
}

class SpectrStabFactors;
class DiagSpectr;

class SpectrStabVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit SpectrStabVisualWidget(VisualDescriptor* visual,
                                    const QString& testUid, const QString& probeUid, const QString& channelUid,
                                    QWidget *parent = nullptr);
    ~SpectrStabVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    void splitterMoved(int pos, int index);

    /*!
     * \brief Нажатие мышки на теле виджета
     * \param x, y - координаты нажатия
     */
    void on_press(const int x, const int y);
    /*!
     * \brief Отпускание мышки на теле виджета
     * \param x, y - координаты отпускания
     */
    void on_release(const int x, const int y);

    /*!
     * \brief Перенос мышки по телу виджета
     * \param x, y - координаты положения
     */
    void on_move(const int x, const int y);

private:
    Ui::SpectrStabVisualWidget *ui;

    void showTable();
    void showSpectrs();

    void saveSplitterPosition();
    void restoreSplitterPosition();

    void selectionReset();

    SpectrStabFactors* m_factors {nullptr};

    ///< Выделение зоны на спектре
    DiagSpectr* m_selectAreaWidget {nullptr};  ///< Указатель на виджет
    int m_selAreaX {-1};                       ///< Начальная координата X
    int m_selAreaY {-1};                       ///< Начальная координата Y

    bool m_selectionProcess {false};
    QPoint m_selFrom = QPoint(-1, -1);
    QPoint m_selTo = QPoint(-1, -1);
};

#endif // SPECTRSTABVISUALWIDGET_H
