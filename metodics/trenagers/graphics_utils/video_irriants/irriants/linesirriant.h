#ifndef LINESIRRIANT_H
#define LINESIRRIANT_H

#include <QObject>

#include "videoirritant.h"

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
};

#endif // LINESIRRIANT_H
