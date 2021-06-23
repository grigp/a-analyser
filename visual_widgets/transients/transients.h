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

    void setBackgroundColor(const QColor color) {m_backgroundColor = color;}
    QColor backgroundColor() const {return m_backgroundColor;}

    void setAxisColor(const QColor color) {m_axisColor = color;}
    QColor axisColor() const {return m_axisColor;}

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

    QVector<double> m_compensation;
    QVector<double> m_return;
};

#endif // TRANSIENTS_H
