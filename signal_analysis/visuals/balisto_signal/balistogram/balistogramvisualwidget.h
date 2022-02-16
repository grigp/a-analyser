#ifndef BALISTOGRAMVISUALWIDGET_H
#define BALISTOGRAMVISUALWIDGET_H

#include <QWidget>

#include "visuals.h"

namespace Ui {
class BalistogramVisualWidget;
}

/*!
 * \brief Класс виджета визуализатора ,балистограммы The BalistogramVisualWidget class
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


private:
    Ui::BalistogramVisualWidget *ui;
};

#endif // BALISTOGRAMVISUALWIDGET_H
