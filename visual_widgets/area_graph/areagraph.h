#ifndef AREAGRAPH_H
#define AREAGRAPH_H

#include <QWidget>

namespace Ui {
class AreaGraph;
}

/*!
 * \brief Структура, содержащая основные цвета GraphEnvColors struct
 */
struct GraphEnvColors
{
    QColor colorBackground;
    QColor colorAxis;
    QColor colorGrid;
    QColor colorLabels;
    QColor colorCursor;

    GraphEnvColors() {}
};

class SignalAccess;

/*!
 * \brief Класс, содержащий данные для зоны построения графика GraphArea class
 */
class GraphArea
{
public:
    explicit GraphArea(SignalAccess* signal,
                       const int numSubChan,
                       const QString &name);

    SignalAccess* signal() const {return m_signal;}

    int numSubChan() {return m_numSubChan;}

    QColor color(const int colorNum) const;
    void setColor(const int colorNum, const QColor &color);

    QString name() const {return m_name;}

    double minValue() const {return m_minVal;}
    double maxValue() const {return m_maxVal;}
    double average(const int numSubChan);
    void setDiapazone(const double minVal, const double maxVal);

    int cursorPos() {return m_cursorPos;}
    void setCursor(const int pos);

private:
    /*!
     * \brief Расчет среднего значения по каждому отведению сигнала
     * Рассчитанные значния записываются в массив m_average
     * Его размер равен кол-ву подканалов
     */
    void computeAverageValue();

    SignalAccess* m_signal {nullptr};
    int m_numSubChan {-1};   ///< Номер отображаемого подканала. Если он равен -1, то отображаются все подканалы сигнала в одной зоне
    QString m_name;
    QVector<QColor> m_palette;
    double m_minVal {0};
    double m_maxVal {0};
    QList<double> m_average;
    int m_cursorPos {-1};   ///< Номер отсчета, на котором стоит курсор
};

/*!
 * \brief Класс виджета построения графиков AreaGraph class
 */
class AreaGraph : public QWidget
{
    Q_OBJECT

public:
    explicit AreaGraph(QWidget *parent = 0);
    ~AreaGraph();

    /*!
     * \brief Добавляет зону построения графика с (блэкджеком и шлюхами) сигналом и номером отведения
     * \param signal - сигнал
     * \param numSubChan - номер отведения
     */
    void appendSignal(SignalAccess* signal, const QString &name, const int numSubChan = -1);

    /*!
     * \brief Возвращает кол-во зон построения
     */
    int areasesCount() const {return m_areases.size();}

    /*!
     * \brief Возвращает указатель на зону построения по ее номеру
     * \param areaNum - номер зоны построения
     */
    GraphArea* area(const int areaNum) const;

    /*!
     * \brief Очищает все зоны построения и удаляет их
     */
    void clear();

    QColor colorBackground() const {return m_envColors.colorBackground;}
    void setColorBackground(const QColor &color);

    QColor colorAxis() const {return m_envColors.colorAxis;}
    void setColorAxis(const QColor &color);

    QColor colorGrid() const {return m_envColors.colorGrid;}
    void setColorGrid(const QColor &color);

    QColor colorLabels() const {return m_envColors.colorLabels;}
    void setColorLabels(const QColor &color);

    QColor colorCursor() const {return m_envColors.colorCursor;}
    void setColorCursor(const QColor &color);

    /*!
     * \brief Режимы отображения сигнала по горизонтали XCoordSignalMode enum
     */
    enum XCoordSignalMode
    {
          xsm_fullSignal = 0  ///< Режим полного сигнала
        , xsm_scrolling       ///< Режим скроллинга
    };

    XCoordSignalMode xcoordSignalMode() const {return m_xcsm;}
    void setXCoordSignalMode(const XCoordSignalMode mode);

    int startPoint() const {return m_startPoint;}
    void setStartPoint(const int startPoint);

    int hScale() const {return m_hScale;}
    void setHScale(const int hScale);

    bool isZeroing() const {return m_isZeroing;}
    void setIsZeroing(const bool zeroing);

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
     * \brief Устанавливает курсор в зоне zoneNum на позицию pos
     * \param zoneNum - номер зоны
     * \param pos - позиция (номер отсчета)
     */
    void setCursor(const int zoneNum, const int pos);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::AreaGraph *ui;

    QList<GraphArea*> m_areases;
    GraphEnvColors m_envColors;

    XCoordSignalMode m_xcsm {xsm_fullSignal}; ///< Режим отображения сигнала по горизонтали
    int m_startPoint {0};                     ///< Начальная точка прорисовки в режиме скроллинга
    int m_hScale {1};                         ///< Множитель горизонтального масштабирования (увеличения)
    bool m_isZeroing {false};                 ///< Центровка
};

#endif // AREAGRAPH_H
