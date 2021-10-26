#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <QWidget>

namespace Ui {
class Oscilloscope;
}

/*!
 * \brief Структура, содержащая основные цвета GraphEnvColors struct
 */
struct OscilloscopeEnvColors
{
    QColor colorAxis;
    QColor colorGrid;
    QColor colorLabels;
    QColor colorCursor;

    OscilloscopeEnvColors() {}
};



/*!
 * \brief Класс, содержащий данные для зоны построения графика GraphArea class
 */
class OscilloscopeArea
{
public:
    explicit OscilloscopeArea(QWidget* parent,
                              const QString &name,
                              const int channelCount);

    QColor color(const int colorNum) const;
    void setColor(const int colorNum, const QColor &color);

    QString name() const {return m_name;}
    int channelsCount() const {return m_channelCount;}

    double minValue() const {return m_minVal;}
    double maxValue() const {return m_maxVal;}
    void setDiapazone(const double minVal, const double maxVal);

    void dataResize(const int dataSize);

    void addValue(const QVector<double> value, const int recNum);
    QVector<double> value(const int idx) const;

    double offset(const int numChan) const;
    void setOffset(const int numChan, const double offset);

    void clear() {m_data.clear();}

private:
    QWidget* m_parent {nullptr};
    QString m_name;
    int m_channelCount;
    QVector<QColor> m_palette;
    QVector<QVector<double>> m_data;
    double m_minVal {0};
    double m_maxVal {0};
    QVector<double> m_offsets;
};


/*!
 * \brief Класс виджета - осциллографа (построения графиков online) Oscilloscope class
 */
class Oscilloscope : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor line1_color MEMBER m_line1Color DESIGNABLE true)
    Q_PROPERTY(QColor line2_color MEMBER m_line2Color DESIGNABLE true)
    Q_PROPERTY(QColor line3_color MEMBER m_line3Color DESIGNABLE true)
    Q_PROPERTY(QColor line4_color MEMBER m_line4Color DESIGNABLE true)
    Q_PROPERTY(QColor line5_color MEMBER m_line5Color DESIGNABLE true)
    Q_PROPERTY(QColor line6_color MEMBER m_line6Color DESIGNABLE true)

public:
    explicit Oscilloscope(QWidget *parent = nullptr);
    ~Oscilloscope() override;

    /*!
     * \brief Добавляет зону построения графиков
     * \param areaName - название зоны
     * \param channelCount - кол-во подканалов
     */
    void appendArea(const QString &areaName, const int channelCount);

    /*!
     * \brief Возвращает кол-во зон построения
     */
    int areasesCount() const {return m_areases.size();}

    /*!
     * \brief Возвращает указатель на зону построения по ее номеру
     * \param areaNum - номер зоны построения
     */
    OscilloscopeArea* area(const int areaNum) const;

    /*!
     * \brief Очищает все зоны построения и удаляет их
     */
    void clear();

    QColor colorAxis() const {return m_envColors.colorAxis;}
    void setColorAxis(const QColor &color);

    QColor colorGrid() const {return m_envColors.colorGrid;}
    void setColorGrid(const QColor &color);

    QColor colorLabels() const {return m_envColors.colorLabels;}
    void setColorLabels(const QColor &color);

    QColor colorCursor() const {return m_envColors.colorCursor;}
    void setColorCursor(const QColor &color);

    bool isZeroing() const {return m_isZeroing;}
    void setIsZeroing(const bool zeroing);

    int frequency() const {return m_frequency;}
    void setFrequency(const int frequency) {m_frequency = frequency;}

    bool isSecundLabels() const {return m_isSecundLabels;}
    void setSecundLabels(const bool isSecLabels) {m_isSecundLabels = isSecLabels;}

    /*!
     * \brief Устанавливает диапазоны для одной зоны графиков одинаковыми
     * \param numArea - номер зоны графиков
     * \param minVal, maxVal - минимальная и максимальная границы диапазона
     */
    void setDiapazone(const int numArea, const double minVal, const double maxVal);
    /*!
     * \brief Устанавливает диапазоны для всех зон графиков одинаковыми
     * \param minVal, maxVal - минимальная и максимальная границы диапазона
     */
    void setDiapazone(const double minVal, const double maxVal);

    /*!
     * \brief Возвращает диапазон значений для зоны
     * \param numArea - номер зоны 0 - ...
     * \param minVal, maxVal - возвращаемые минимальное и максимальное значения
     * \return true, если удачно
     */
    bool diapazone(const int numArea, double &minVal, double &maxVal);

    /*!
     * \brief Добавляет значение в данные осциллографа
     * \param value - значение, представляющее собой массив.
     *                Кол-во элементов равно сумме каналов в каждой из зон осциллографа
     */
    void addValue(const QVector<double> value);

    QColor line1Color() const {return m_line1Color;}
    QColor line2Color() const {return m_line2Color;}
    QColor line3Color() const {return m_line3Color;}
    QColor line4Color() const {return m_line4Color;}
    QColor line5Color() const {return m_line5Color;}
    QColor line6Color() const {return m_line6Color;}


protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::Oscilloscope *ui;

    QList<OscilloscopeArea*> m_areases;
    OscilloscopeEnvColors m_envColors;

    int m_graphWidth {0};                     ///< Ширина зоны построения графиков
    bool m_isZeroing {false};                 ///< Центровка
    int m_cursorPos {0};                      ///< Позиция курсора
    int m_dataCounter {0};                    ///< Счетчик переданных данных
    int m_frequency {50};                     ///< Частота дискретизации
    bool m_isSecundLabels {true};             ///< Секундные метки

    QColor m_line1Color {Qt::darkGreen};
    QColor m_line2Color {Qt::blue};
    QColor m_line3Color {Qt::darkCyan};
    QColor m_line4Color {Qt::red};
    QColor m_line5Color {Qt::darkYellow};
    QColor m_line6Color {Qt::darkMagenta};
};

#endif // OSCILLOSCOPE_H
