#ifndef LINESKG_H
#define LINESKG_H

#include <QObject>
#include <QGraphicsItem>

class SignalResultInfo;

/*!
 * \brief Класс линии СКГ LineSKG class
 * Грузится один раз и полностью
 */
class LineSKG : public QGraphicsItem
{
public:
    LineSKG(int diap, QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget);

    // Устанавливает диапазон коодинат
    void setDiap(int diap);

    /*!
     * \brief Задает сигнал для отображения в виджете при использовании в визуале
     * \param signal
     */
    void setSignal(SignalResultInfo *signal);

private:
    void updateItem();

    int m_diap;
    double m_prop = 1;
    int m_width = 0;
    int m_height = 0;

    SignalResultInfo *m_signal {nullptr};
};

#endif // LINESKG_H
