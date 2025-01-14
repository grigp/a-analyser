#ifndef LINESKG_H
#define LINESKG_H

#include <QObject>
#include <QGraphicsItem>

class SignalAccess;

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
    void setSignal(SignalAccess *signal, const int begin = -1, const int end = -1);

    /*!
     * \brief Задает границы участка сигнала
     * \param begin, end - границы участка
     */
    void setSection(const int begin, const int end);

    /*!
     * \brief Устанавливает признак центровки сигнала
     */
    void setZeroing(const bool zeroing);

    /*!
     * \brief Установка смещения снаружи
     * \param offsetX, offsetY - смещения
     */
    void setOffset(const double offsetX, const double offsetY);

    /*!
     * \brief Устанавливает параметры эллипса
     * \param sizeA, sizeB - длины осей
     * \param angle - угол наклона
     */
    void setEllipse(const double sizeA, const double sizeB, const double angle);

    /*!
     * \brief Устанавливает цвет СКГ
     */
    void setColorSKG(const QColor &color) {m_colorSKG = color;}
    QColor colorSKG() const {return m_colorSKG;}

    /*!
     * \brief Устанавливает цвет эллипса
     */
    void setColorEllipse(const QColor &color) {m_colorEllipse = color;}
    QColor colorEllipse() const {return m_colorEllipse;}

    /*!
     * \brief Устанавливает видимость СКГ
     * \param isVisible - будет ли видна
     */
    void setVisible(const bool isVisible);

private:
    void updateItem();

    int m_diap;
    double m_prop {1};
    int m_width {0};
    int m_height {0};
    double m_offsX {0}, m_offsY {0};
    bool m_isZeroing {false};
    int m_begin = -1;
    int m_end = -1;
    bool m_visible {true};

    SignalAccess *m_signal {nullptr};

    ///< Параметры эллипса
    double m_sizeA {0};
    double m_sizeB {0};
    double m_angle {0};

    QColor m_colorSKG {Qt::darkCyan};
    QColor m_colorEllipse {Qt::darkBlue};

};

#endif // LINESKG_H
