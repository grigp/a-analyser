#ifndef TRANSIENTS_H
#define TRANSIENTS_H

#include <QWidget>
#include <QVector>

#include "stepoffsetdefines.h"
#include "transientspainter.h"

namespace Ui {
class Transients;
}

/*!
 * \brief Класс диаграммы переходных процессов Transients class
 */
class Transients : public QWidget, public TransientsPainter
{
    Q_OBJECT

public:
    explicit Transients(QWidget *parent = nullptr);
    ~Transients() override;

//    /*!
//     * \brief Временные параметры переходного процесса Params struct
//     */
//    struct Params
//    {
//        double latent;       //! Латентный период
//        double swing;        //! Время размаха
//        double spurt;        //! Время броска
//        double reaction;     //! Время реакции
//        double statism;      //! Статизм
//        double deviation;    //! Разброс на этапе удержания
//    };

//    QColor axisColor() const {return m_axisColor;}
//    void setAxisColor(const QColor color) {m_axisColor = color;}

//    int freq() const {return m_freq;}
//    void setFreq(const int freq) {m_freq = freq;}

//    /*!
//     * \brief Возвращает параметры для переходного процесса
//     * \param stage - компенсация или возврат
//     * \param latent - латентный период
//     * \param swing - время размаха
//     * \param spurt - время броска
//     * \param reaction - время реакции
//     */
//    void params(const StepOffsetDefines::Stage stage,
//                double &latent, double &swing, double &spurt, double &reaction, double &statism, double &deviation) const;

//    /*!
//     * \brief Устанавливает параметры для переходного процесса
//     * \param stage - компенсация или возврат
//     * \param latent - латентный период
//     * \param swing - время размаха
//     * \param spurt - время броска
//     * \param reaction - время реакции
//     */
//    void setParams(const StepOffsetDefines::Stage stage,
//                   const double latent, const double swing, const double spurt, const
//                   double reaction, const double statism, const double deviation);

//    /*!
//     * \brief Запускает режим добавления значений
//     */
//    void beginAddValues();
//    /*!
//     * \brief Завершает режим добавления значений
//     */
//    void endAddValues();
//    /*!
//     * \brief Добавляет значение в данные компенсации
//     */
//    void setCompensationValue(const double value);
//    /*!
//     * \brief Добавляет значение в данные возврата
//     */
//    void setReturnValue(const double value);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Transients *ui;

//    /*!
//     * \brief Возвращает цвет переходного процесса для данной точки в соответствии с параметрами
//     * \param cnt - точка процесса
//     * \param params - параметры процесса
//     */
//    QColor getColorTransient(const int cnt, const Params &params) const;

//    QColor m_axisColor {Qt::black};

//    QColor m_latentColor {Qt::darkYellow};
//    QColor m_swingColor {Qt::darkMagenta};
//    QColor m_spurtColor {Qt::darkRed};
//    QColor m_stabilisationColor {Qt::darkCyan};
//    QColor m_retentionColor {Qt::green};

//    QVector<double> m_compensation;
//    QVector<double> m_return;

//    int m_freq {50};

//    Params m_parComp;
//    Params m_parRet;

};

#endif // TRANSIENTS_H
