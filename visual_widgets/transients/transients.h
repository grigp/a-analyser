#ifndef TRANSIENTS_H
#define TRANSIENTS_H

#include <QWidget>
#include <QVector>

namespace Ui {
class Transients;
}

/*!
 * \brief Класс диаграммы переходных процессов Transients class
 */
class Transients : public QWidget
{
    Q_OBJECT

public:
    explicit Transients(QWidget *parent = nullptr);
    ~Transients() override;

    QColor backgroundColor() const {return m_backgroundColor;}
    void setBackgroundColor(const QColor color) {m_backgroundColor = color;}

    QColor axisColor() const {return m_axisColor;}
    void setAxisColor(const QColor color) {m_axisColor = color;}

    int freq() const {return m_freq;}
    void setFreq(const int freq) {m_freq = freq;}

    double latentComp() const {return m_latentComp;}
    void setLatentComp(const double latent) {m_latentComp = latent;}

    double latentRet() const {return m_latentRet;}
    void setLatentRet(const double latent) {m_latentRet = latent;}

    double swingComp() const {return m_swingComp;}
    void setSwingComp(const double swing) {m_swingComp = swing;}

    double swingRet() const {return m_swingRet;}
    void setSwingRet(const double ret) {m_swingRet = ret;}

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

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Transients *ui;

    QColor m_backgroundColor {Qt::white};
    QColor m_axisColor {Qt::black};

    QColor m_latentColor {Qt::darkRed};
    QColor m_swingColor {Qt::darkYellow};

    QVector<double> m_compensation;
    QVector<double> m_return;

    int m_freq {50};

    double m_latentComp {0};
    double m_latentRet {0};

    double m_swingComp {0};
    double m_swingRet {0};
};

#endif // TRANSIENTS_H
