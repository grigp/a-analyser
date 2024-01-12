#ifndef PIEDIAGRAM_H
#define PIEDIAGRAM_H

#include <QWidget>

namespace Ui {
class PieDiagram;
}

/*!
 * \brief Класс виджета круговой диаграммы The PieDiagram class
 */
class PieDiagram : public QWidget
{
    Q_OBJECT

public:
    explicit PieDiagram(QWidget *parent = nullptr);
    ~PieDiagram() override;

    /*!
     * \brief Добавляет значение показателя
     * \param value - значение показателя
     * \param color - цвет куска диаграммы
     */
    void addFactor(const double value, const QColor &color);

    /*!
     * \brief Очищает список показателей
     */
    void clear();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::PieDiagram *ui;

    //! Структура показателя, хранимая внутри
    struct Value
    {
        double value;
        QColor color;
        double percent;
        Value(const double v, const QColor& c) : value(v), color(c) {}
    };

    QList<Value> m_factors;
};

#endif // PIEDIAGRAM_H
