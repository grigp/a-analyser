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

    /*!
     * \brief Удаляет артефакты
     */
    void on_deleteArtifacts();

    /*!
     * \brief Перезаписывает сигнал
     */
    void on_rewriteSignal();

    /*!
     * \brief Действия по изменению данных канала в БД
     * \param probeUid - uid пробы
     * \param channelId - id канала
     */
    void on_channelChanged(const QString &probeUid, const QString &channelId);

private:
    Ui::RitmogramVisualWidget *ui;

    void showGraph();
    void showResume();
    QString getStyleByValue(const int value) const;

    /*!
     * \brief Удаление артефактов на ритмограмме
     * \param lower - нижний порог удаления
     * \param upper - верхний порог удаления
     */
    void deleteArtifacts(const double lower, const double upper);

    PulseFactors *m_factors {nullptr};
    Ritmogram *m_signal {nullptr};
    int m_selBeg {-1};

    ///< Удаление артефактов
    double m_abLower {-1};  ///< Нижняя граница
    double m_abUpper {-1};  ///< Верхняя граница
};

#endif // RITMOGRAMVISUALWIDGET_H
