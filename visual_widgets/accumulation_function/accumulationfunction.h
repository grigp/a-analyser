#ifndef ACCUMULATIONFUNCTION_H
#define ACCUMULATIONFUNCTION_H

#include <QWidget>

namespace Ui {
class AccumulationFunction;
}

/*!
 * \brief Класс виджета показа функкции накопления The AccumulationFunction class
 */
class AccumulationFunction : public QWidget
{
    Q_OBJECT

public:
    explicit AccumulationFunction(QWidget *parent = nullptr);
    ~AccumulationFunction() override;

    QColor axizColor() const {return m_axizColor;}
    void setAxizColor(const QColor color) {m_axizColor = color;}

    QColor lineColor() const {return m_lineColor;}
    void setLineColor(const QColor color) {m_lineColor = color;}

    QColor valueColor() const {return m_valueColor;}
    void setValueColor(const QColor color) {m_valueColor = color;}

    QString value() const {return m_value;}
    void setValue(const QString value) {m_value = value;}

    QString title() const {return m_title;}
    void setTitle(const QString value) {m_title = value;}

    QString xAxizName() const {return m_xAxizName;}
    void setXAxizName(const QString value) {m_xAxizName = value;}

    QString yAxizName() const {return m_yAxizName;}
    void setYAxizName(const QString value) {m_yAxizName = value;}

    /*!
     * \brief Устанавливает диапазон вывода функции
     * \param min, max - диапазон
     */
    void setDiap(const double min, const double max);

    /*!
     * \brief Добавляет значение
     * \param v - значение
     */
    void add(const double v);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::AccumulationFunction *ui;

    QColor m_axizColor {Qt::black};
    QColor m_lineColor {Qt::darkCyan};
    QColor m_valueColor {Qt::darkCyan};

    QString m_value {""};
    QString m_title {tr("Функция накопления")};
    QString m_yAxizName {""};
    QString m_xAxizName {""};

    double m_min {0};
    double m_max {100};

    QVector<double> m_function;
};

#endif // ACCUMULATIONFUNCTION_H
