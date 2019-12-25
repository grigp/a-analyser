#ifndef SIGNALWIDGET_H
#define SIGNALWIDGET_H

#include <QObject>
#include <QWidget>

/*!
 * \brief Базовый класс виджета сигналов SignalWidget class
 */
class SignalWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SignalWidget(const QString channelId, QWidget *parent = nullptr);

//    virtual QString signalType() = 0;

    QString channelId() {return m_channelId;}

signals:

public slots:


private:
    QString m_channelId {""};
};

#endif // SIGNALWIDGET_H
