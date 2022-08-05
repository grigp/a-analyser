#ifndef DIAGSPECTR_H
#define DIAGSPECTR_H

#include <QWidget>

namespace Ui {
class DiagSpectr;
}

/*!
 * \brief Класс диаграммы отображения спектра The DiagSpectr class
 */
class DiagSpectr : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor line1_color MEMBER m_line1Color DESIGNABLE true)

public:
    explicit DiagSpectr(QWidget *parent = nullptr);
    ~DiagSpectr() override;

    QColor line1Color() const {return m_line1Color;}
    void setLine1Color(const QColor color) {m_line1Color = color;}

    QColor colorAxis() const {return m_colorAxis;}
    void setColorAxis(const QColor &color);

    QColor colorGrid() const {return m_colorGrid;}
    void setColorGrid(const QColor &color);

    QString title() const {return m_title;}
    void setTitle(const QString title) {m_title = title;}

    QString nameAxisX() const {return m_nameAxisX;}
    void setNameAxisX(const QString name) {m_nameAxisX = name;}

    QString nameAxisY() const {return m_nameAxisY;}
    void setNameAxisY(const QString name) {m_nameAxisY = name;}

    QRect bounds() const {return m_bounds;}

    /*!
     * \brief Добавляет значение в список
     * \param value - значение
     */
    void addValue(const double value);

    /*!
     * \brief задает данные для форматирования:
     * \param frequency - частота дискретизации
     * \param points - кол-во точек
     * \param maxFrequency - максимальная отображаемая частота
     */
    void setFormatData(const int frequency, const int points, const double maxFrequency);

    /*!
     * \brief Задает размеры отображаемой области
     * \param minFreq, maxFreq - диапазон частот
     * \param minValue, maxValue - диапазон значений
     */
    void setVisualArea(const double minFreq, const double maxFreq, const double minValue, const double maxValue);

    /*!
     * \brief Сбрасывает размеры отображаемой области
     */
    void resetVisualArea();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::DiagSpectr *ui;

    QColor m_line1Color {Qt::darkGreen};
    QColor m_colorAxis {Qt::black};
    QColor m_colorGrid {Qt::gray};

    QString m_title {""};
    QString m_nameAxisX {"F,Гц"};
    QString m_nameAxisY {"A,мм"};

    QRect m_bounds = QRect(QPoint(30, 30), QPoint(10, 30));

    QVector<double> m_data;
    int m_frequency {50};      ///< Частота дискретизации
    int m_points {1024};       ///< Кол-во точек
    double m_maxFrequency {6}; ///< Максимальная отображаемая частота

    double m_minFreq {0};      ///< Минимальная частота
    double m_maxFreq {6};      ///< Максимальная частота
    double m_minValue {-1};    ///< Минимальное значение
    double m_maxValue {-1};    ///< Максимальное значение
};

#endif // DIAGSPECTR_H
