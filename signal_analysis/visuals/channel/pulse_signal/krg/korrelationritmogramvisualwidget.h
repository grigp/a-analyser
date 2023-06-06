#ifndef KORRELATIONRITMOGRAMVISUALWIDGET_H
#define KORRELATIONRITMOGRAMVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class KorrelationRitmogramVisualWidget;
}

class Ritmogram;

/*!
 * \brief Класс виджета визуализатора сигнала корреляционной ритмограммы The KorrelationRitmogramVisualWidget class
 */
class KorrelationRitmogramVisualWidget : public ChannelVisual
{
    Q_OBJECT

public:
    explicit KorrelationRitmogramVisualWidget(VisualDescriptor* visual,
                                              const QString& testUid, const QString& probeUid, const QString& channelId,
                                              QWidget *parent = nullptr);
    ~KorrelationRitmogramVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private slots:
    void on_splitterMoved(int,int);

    /*!
     * \brief Действия по изменению данных канала в БД
     * \param probeUid - uid пробы
     * \param channelId - id канала
     */
    void on_channelChanged(const QString &probeUid, const QString &channelId);


private:
    Ui::KorrelationRitmogramVisualWidget *ui;

    void saveSplitterPosition();
    void restoreSplitterPosition();

    void showKRG();

    Ritmogram *m_signal {nullptr};
};

#endif // KORRELATIONRITMOGRAMVISUALWIDGET_H
