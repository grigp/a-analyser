#ifndef BUBBLESIRRIANT_H
#define BUBBLESIRRIANT_H

#include <QObject>
#include <QTime>

#include "videoirritant.h"
#include "baseutils.h"

class BubblesIrriant : public Irriant
{
public:
    BubblesIrriant();

    /*!
     * \brief Возвращает уникальный идентификатор раздражителя
     */
    QString uid() override {return tr("bubbles");}
    /*!
     * \brief Возвращает показываемое название раздражителя
     */
    QString name() override {return tr("Пузырьки");}

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
     * \brief Доступ к m_bubblesCount
     */
    int bubblesCount() const {return m_bubblesCount;}
    void setBubblesCount(const int cnt);

private:
    void initBubbles();

    int m_bubblesCount {50};

};

#endif // BUBBLESIRRIANT_H
