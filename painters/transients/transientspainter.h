#ifndef TRANSIENTSPAINTER_H
#define TRANSIENTSPAINTER_H

#include <QObject>
#include <QWidget>
#include <QPainter>

#include "stepoffsetdefines.h"

/*!
 * \brief Класс рисователя диаграммы теста "Ступени" The TransientsPainter class
 */
class TransientsPainter
{
public:
    explicit TransientsPainter(QPainter* painter, QRect geometry);
    explicit TransientsPainter();
    virtual ~TransientsPainter();

    /*!
     * \brief Задает канву прорисовки. Будет испльзоваться в режиме виджета для задания
     * \param painter - рисователь
     * \param geometry - размер области прорисовки
     * \param wgt - указатель на виджет, если рисуем на виджете
     */
    void setCanvas(QPainter* painter, QRect geometry, QWidget *wgt = nullptr);

    /*!
     * \brief Временные параметры переходного процесса Params struct
     */
    struct Params
    {
        double latent;       //! Латентный период
        double swing;        //! Время размаха
        double spurt;        //! Время броска
        double reaction;     //! Время реакции
        double statism;      //! Статизм
        double deviation;    //! Разброс на этапе удержания
    };

    QColor axisColor() const {return m_axisColor;}
    void setAxisColor(const QColor color) {m_axisColor = color;}

    int freq() const {return m_freq;}
    void setFreq(const int freq) {m_freq = freq;}

    /*!
     * \brief Возвращает параметры для переходного процесса
     * \param stage - компенсация или возврат
     * \param latent - латентный период
     * \param swing - время размаха
     * \param spurt - время броска
     * \param reaction - время реакции
     */
    void params(const StepOffsetDefines::Stage stage,
                double &latent, double &swing, double &spurt, double &reaction, double &statism, double &deviation) const;

    /*!
     * \brief Устанавливает параметры для переходного процесса
     * \param stage - компенсация или возврат
     * \param latent - латентный период
     * \param swing - время размаха
     * \param spurt - время броска
     * \param reaction - время реакции
     */
    void setParams(const StepOffsetDefines::Stage stage,
                   const double latent, const double swing, const double spurt, const
                   double reaction, const double statism, const double deviation);

    /*!
     * \brief Запускает режим добавления значений
     */
    void beginAddValues();
    /*!
     * \brief Завершает режим добавления значений
     */
    void endAddValues();
    /*!
     * \brief Добавляет значение в данные компенсации
     */
    void setCompensationValue(const double value);
    /*!
     * \brief Добавляет значение в данные возврата
     */
    void setReturnValue(const double value);

    /*!
     * \brief Процедура прорисовки на рисователе m_painter  в рамках m_geometry
     */
    void doPaint(const double ratio);


protected:
    virtual void doUpdate();

private:
    QPainter* m_painter {nullptr};
    QRect m_geometry {QRect(0, 0, 0, 0)};
    QWidget* m_widget {nullptr};

    /*!
     * \brief Возвращает цвет переходного процесса для данной точки в соответствии с параметрами
     * \param cnt - точка процесса
     * \param params - параметры процесса
     */
    QColor getColorTransient(const int cnt, const Params &params) const;

    QColor m_axisColor {Qt::black};

    QColor m_latentColor {Qt::darkYellow};
    QColor m_swingColor {Qt::darkMagenta};
    QColor m_spurtColor {Qt::darkRed};
    QColor m_stabilisationColor {Qt::darkCyan};
    QColor m_retentionColor {Qt::green};

    QVector<double> m_compensation;
    QVector<double> m_return;

    int m_freq {50};

    Params m_parComp;
    Params m_parRet;
};

#endif // TRANSIENTSPAINTER_H
