#include "signalwidget.h"

#include "driver.h"

SignalWidget::SignalWidget(Driver *driver, const QString channelId, QWidget *parent)
    : QWidget(parent)
    , m_driver(driver)
    , m_channelId(channelId)
{

}
