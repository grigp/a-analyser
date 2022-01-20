#ifndef BROKENLINESSKG_H
#define BROKENLINESSKG_H

#include <QObject>
#include <QGraphicsItem>

#include "areaskgdefines.h"


class BrokenLinesSKG : public QGraphicsItem
{
public:
    BrokenLinesSKG(int diap, QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);

    /*!
     * \brief Добавляет ломанную
     * \param bl - ломанная
     * \return индекс в списке ломаных
     */
    int addBrokenLine(AreaSKGDefines::BrokenLine &bl);

    /*!
     * \brief Удаляет ломаную по индексу
     * \param idx - индекс ломаной
     * \return true, если удачно
     */
    bool deleteBrokenLine(const int idx);

    // Устанавливает диапазон коодинат
    void setDiap(int diap);

private:
    int m_diap;
    double m_prop = 1;
    int m_width {0};
    int m_height {0};

    QList<AreaSKGDefines::BrokenLine> m_brokenLines;   ///< Список ломаных для построения

};

#endif // BROKENLINESSKG_H
