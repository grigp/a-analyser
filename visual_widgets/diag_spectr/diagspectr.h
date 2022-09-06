#ifndef DIAGSPECTR_H
#define DIAGSPECTR_H

#include <QWidget>
#include <QMouseEvent>

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
    /*!
     * \brief Зона по частоте на спектре The FreqArea struct
     */
    struct FreqArea
    {
        double begin {0};                    ///< Начало зоны
        double end {0};                      ///< Конец зоны
        QString name {""};                   ///< Название
        QColor colorBackground {Qt::cyan};   ///< Цвет фона
        QColor colorText {Qt::darkCyan};     ///< Цвет текста
        FreqArea(const double b, const double e, const QString n, const QColor cb, const QColor ct)
            : begin(b), end(e), name(n), colorBackground(cb), colorText(ct) {}
        FreqArea() {}
    };

    /*!
     * \brief Метка по частоте на спектре The FreqLabel struct
     */
    struct FreqLabel
    {
        double freq {0};
        QString name {""};
        FreqLabel(const double f, const QString n)
            : freq(f), name(n) {}
        FreqLabel() {}
    };

    explicit DiagSpectr(QWidget *parent = nullptr);
    ~DiagSpectr() override;

    QColor line1Color() const {return m_line1Color;}
    void setLine1Color(const QColor color) {m_line1Color = color;}

    QColor colorAxis() const {return m_colorAxis;}
    void setColorAxis(const QColor &color) {m_colorAxis = color;}

    QColor colorGrid() const {return m_colorGrid;}
    void setColorGrid(const QColor &color) {m_colorGrid = color;}

    QColor colorFreqLabel() const {return m_colorFreqLabel;}
    void setColorFreqLabel(const QColor &color) {m_colorFreqLabel = color;}

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
     * \brief Очищает спектр
     */
    void clear();

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

    /*!
     * \brief Выделение зоны на диаграмме
     * \param rect - прямоугольник выделения
     */
    void selectArea(QRect rect);

    /*!
     * \brief Возвращает значения (частоту и амплитуду) спектра по точке.
     * retval.x() - частота, retval.y() - амплитуда
     * Если точка за пределами, возвращает QPointF()
     * \param point - точка x и y
     */
    QPointF getValues(const QPoint point) const;

    /*!
     * \brief Возвращает точку на диаграмме по значениям (частоте и амплитуде) спектра.
     * Если точка за пределами, возвращает QPoint()
     * \param values - значения
     */
    QPoint getPoint(const QPointF values) const;

    /*!
     * \brief Добавляет зону по частоте на спектре
     */
    void addFreqArea(const double begin, const double end, const QString name, const QColor colorBackground, const QColor colorText);

    /*!
     * \brief Добавляет метку по частоте на спектре
     */
    void addFreqLabel(const double freq, const QString name);

signals:
    /*!
     * \brief Сигнал нажатия мышки на теле виджета
     * \param x, y - координаты нажатия
     */
    void press(const int x, const int y);
    /*!
     * \brief Сигнал отпускания мышки на теле виджета
     * \param x, y - координаты отпускания
     */
    void release(const int x, const int y);
    /*!
     * \brief Сигнал переноса мышки по телу виджета
     * \param x, y - координаты положения
     */
    void move(const int x, const int y);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::DiagSpectr *ui;

    QColor m_line1Color {Qt::darkGreen};
    QColor m_colorAxis {Qt::black};
    QColor m_colorGrid {Qt::lightGray};
    QColor m_colorFreqLabel {Qt::darkRed};

    QString m_title {""};
    QString m_nameAxisX {"F,Гц"};
    QString m_nameAxisY {"A,мм"};

    QRect m_bounds = QRect(QPoint(30, 30), QPoint(10, 30));

    QVector<double> m_data;
    int m_frequency {50};        ///< Частота дискретизации
    int m_points {1024};         ///< Кол-во точек
    double m_maxFrequency {6.1}; ///< Максимальная отображаемая частота

    double m_minFreq {0};      ///< Минимальная частота
    double m_maxFreq {6.1};    ///< Максимальная частота
    double m_minValue {-1};    ///< Минимальное значение
    double m_maxValue {-1};    ///< Максимальное значение

    double m_propX {0};
    double m_propY {0};

    QRect m_selectRect = QRect();

    QList<FreqArea> m_freqAeases;
    QList<FreqLabel> m_freqLabels;
};

#endif // DIAGSPECTR_H
