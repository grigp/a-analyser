#ifndef LINESIRRIANT_H
#define LINESIRRIANT_H

#include <QObject>
#include <QTime>

#include "videoirritant.h"
#include "baseutils.h"

class LinesIrriant : public Irriant
{
public:
    LinesIrriant();

    /*!
     * \brief Возвращает уникальный идентификатор раздражителя
     */
    QString uid() override {return tr("lines");}
    /*!
     * \brief Возвращает показываемое название раздражителя
     */
    QString name() override {return tr("Полосы");}

    /*!
     * \brief Выполняет прорисовку раздражителя
     * \param painter - рисователь
     * \param option - опции
     * \param widget - виджет
     * \param boundingRect - границы зоны рисования
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, QRectF boundingRect) override;

    /*!
     * \brief Возвращает указатель на виджет редактирования параметров раздражителя
     */
    QWidget* getSettingsWidget() override;


    /*!
     * \brief Установка параметров рисования
     */
    void setDirection(const BaseUtils::Directions direction) {m_direction = direction;}
    void setWidth(const int width) {m_width = width;}
    void setSpeed(const int speed) {m_speed = speed;}
    void setDutyCycle(const int dutyCycle) {m_dutyCycle = dutyCycle;}
    void setColor(const QColor color) {m_color = color;}

private:
    BaseUtils::Directions m_direction {BaseUtils::dirRight};
    int m_width {120};
    int m_speed {200};
    int m_dutyCycle {1};
    QColor m_color {Qt::black};
    QTime m_time {QTime()};
    qreal m_position = 0.0;

};

#endif // LINESIRRIANT_H
