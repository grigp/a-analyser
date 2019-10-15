#ifndef CIRCLENORMINDICATOR_H
#define CIRCLENORMINDICATOR_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QGradient>
#include <QMap>


namespace Ui {
class CircleNormIndicator;
}

/*!
 * \brief Структура, содержащая границы нормы NormBounds
 */
struct NormBounds
{
    int condNormLo;   ///< Нижняя граница условной нормы
    int normValLo;    ///< Нижняя граница нормы
    int normValHi;    ///< Верхняя граница нормы
    int condNormHi;   ///< Верхняя граница условной нормы
    ///< [  не норма  ]cNL[  усл. норма  ]nVL[ норма ]nVH[ усл. норма ]cNH[  не норма  ]

    NormBounds(int cNL, int nVL, int nVH, int cNH) :
        condNormLo(cNL)
      , normValLo(nVL)
      , normValHi(nVH)
      , condNormHi(cNH)
    {}
    NormBounds() {}
};


/*!
 * \brief Класс виджета индикатора соответствия значения норме CircleNormIndicator class
 */
class CircleNormIndicator : public QWidget
{
    Q_OBJECT

public:
    explicit CircleNormIndicator(QWidget *parent = 0);
    ~CircleNormIndicator();

    //! Тип нормы
    enum NormKind
    {
          nkGroup = 0
        , nkPersonal = 1
    };

    /*!
     * \brief Устанавливает новое значение
     * \param value - значение
     */
    void setValue(int value);
    /*!
     * \brief Возвращает значение
     */
    int value();

    /*!
     * \brief Устанавливает минимальное значение на диаграмме
     */
    void setMin(const int min);

    /*!
     * \brief Устанавливает максимальное значение на диаграмме
     */
    void setMax(const int max);

    /*!
     * \brief Устанавливает границы значения групповой нормы
     * \param norm - границы значений нормы
     */
    void setGroupNorm(NormBounds &norm);

    /*!
     * \brief Устанавливает границы значения индивидуальной нормы
     * \param norm - границы значений нормы
     */
    void setPersonalNorm(NormBounds &norm);

protected:
    void resizeEvent(QResizeEvent *event);

    void paintEvent(QPaintEvent *event);

private:
    void draw();

    /*!
     * \brief Рисует диаграмму нормы
     * \param nk - тип нормы
     * \param p - рисователь
     */
    void drawNorm(const NormKind nk, QPainter &p);

    /*!
     * \brief Возвращает нужный угол в градусах по значению
     * \param v - значение
     */
    int valueToDegrees(const int v) const;

    /*!
     * \brief Возвращает расстояние в градусах между углом u и углом d.
     * Отсчет начинается от u
     */
    int getDistanceDegrees(const int u, const int d) const;

    /*!
     * \brief Возвращает точку на окружности групповой нормы по углу
     * \param angle - заданный угол
     */
    QPoint getPointOnCircleByAngle(const int angle) const;

    /*!
     * \brief Выводит числовое значение
     */
    void drawValue(QPainter &p);

    Ui::CircleNormIndicator *ui;

    int m_height;    ///< Высота виджета
    int m_width;     ///< Ширина виджета
    int m_minDim;    ///< Размер виджета (Ширина = Высота)
    int m_half;      ///< Половина размера
    int m_centerX;   ///< Центральная точка X
    int m_centerY;   ///< Центральная точка Y

    int m_topReflexY;
    int m_bottomReflexY;
    int m_topReflexWidth;
    int m_topReflexHeight;
    int m_bottomReflexWidth;
    int m_bottomReflexHeight;

    int m_outerBorderWidth;   ///< Ширина внешней границы
    int m_innerBorderWidth;   ///< Ширина внутренней границы
    int m_outerBorderRadius;  ///< Радиус внешней границы
    int m_innerBorderRadius;  ///< Радиус внутренней границы

    QRect m_drawingRect;     ///< Прорисовываемый прямоугольник

    int m_value = 0;          ///< Значение на индикаторе
    int m_minVal = 0;         ///< Минимальное значение
    int m_maxVal = 100;       ///< Максимальное значение

    QColor m_lightColor;         ///< Цвет светлого освещения
    QColor m_shadowColor;        ///< Цвет тени
    QColor m_panelColor;         ///< Цвет панели
    QColor m_arrowColor;         ///< Цвет стрелки
    QColor m_topReflexUpColor;
    QColor m_topReflexDownColor;

    NormBounds m_groupNorms;      ///< Границы групповой нормы
    NormBounds m_personalNorms;    ///< Границы индивидуальной нормы

    bool m_isGroupNorm = false;
    bool m_isPersonalNorm = false;

    QMap<int, QColor> m_colorNormLight;
    QMap<int, QColor> m_colorNormDark;
};

#endif // CIRCLENORMINDICATOR_H
