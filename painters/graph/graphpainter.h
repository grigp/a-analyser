#ifndef GRAPHPAINTER_H
#define GRAPHPAINTER_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

/*!
 * \brief Структура, содержащая основные цвета GraphEnvColors struct
 */
struct GraphEnvColors
{
    QColor colorAxis;
    QColor colorGrid;
    QColor colorLabels;
    QColor colorCursor;
    QColor colorMarkers;
    QColor colorFillBetweenSubchans;

    GraphEnvColors() {}
};

class SignalAccess;
class GraphPainter;

/*!
 * \brief Параметры маркера MarkerInfo struct
 */
struct MarkerInfo
{
    int position;      ///< Положение
    QString comment;   ///< Комментарий
};

/*!
 * \brief Класс, содержащий данные для зоны построения графика GraphArea class
 */
class GraphArea
{
public:
    explicit GraphArea(GraphPainter* parent,
                       SignalAccess* signal,
                       const int numSubChan,
                       const QString &name);

    ~GraphArea();

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

    /*!
     * \brief Доступ к маркерам в зоне
     */
    int markerCount() const {return m_markers.size();}
    MarkerInfo marker(const int idx) const;
    void addMarker(const int pos, const QString comment);

    bool isFillBetweenSubchans() const {return m_isFillBetweenSubchans;}
    void setIsFillBetweenSubchans(const bool isFill) {m_isFillBetweenSubchans = isFill;}

    void setLegend(const QStringList &legend);
    QString legend(const int idx);
    int legendSize() const {return m_legend.size();}

    double step() const {return  m_step;}
    void setStep(const double step) {m_step = step;}

private:
    /*!
     * \brief Расчет среднего значения по каждому отведению сигнала
     * Рассчитанные значния записываются в массив m_average
     * Его размер равен кол-ву подканалов
     */
    void computeAverageValue();

    GraphPainter* m_parent {nullptr};
    SignalAccess* m_signal {nullptr};
    int m_numSubChan {-1};   ///< Номер отображаемого подканала. Если он равен -1, то отображаются все подканалы сигнала в одной зоне
    QString m_name;
    QVector<QColor> m_palette;
    double m_minVal {0};
    double m_maxVal {0};
    QList<double> m_average;         ///< Среднее значение по всему сигналу
    int m_cursorPos {-1};   ///< Номер отсчета, на котором стоит курсор
    QList<MarkerInfo> m_markers;
    bool m_isFillBetweenSubchans {false};
    QStringList m_legend;   ///< Описание подканалов

    double m_step {1};   ///< Шаг по графику в зоне
};


/*!
 * \brief Класс рисователя графика The GraphPainter class
 */
class GraphPainter
{
public:
    explicit GraphPainter(QPainter* painter, QRect geometry);
    explicit GraphPainter();
    virtual ~GraphPainter();

    /*!
     * \brief Тип горизонтальных меток The HorizontalLabelsKind enum
     */
    enum HorizontalLabelsKind
    {
          hlkSeconds = 0  ///< Секундные
        , hlkNumbers       ///< Номера точек
    };

    /*!
     * \brief Задает канву прорисовки. Будет испльзоваться в режиме виджета для задания
     * \param painter - рисователь
     * \param geometry - размер области прорисовки
     */
    void setCanvas(QPainter* painter, QRect geometry, QWidget *wgt = nullptr);

    /*!
     * \brief Добавляет зону построения графика с (блэкджеком и шлюхами) сигналом и номером отведения
     * \param signal - сигнал
     * \param name - название зоны
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
     * \brief Устанавливает описание сигналов для зоны
     * \param areaNum - номер зоны
     * \param legend - список строк с описанием сигналов по каждому подканалу
     */
    void setLegend(const int areaNum, const QStringList &legend);

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

    QColor colorMarker() const {return m_envColors.colorMarkers;}
    void setColorMarker(const QColor &color);

    QColor colorFillBetweenSubchans() const {return m_envColors.colorFillBetweenSubchans;}
    void setColorFillBetweenSubchans(const QColor &color);

    QColor line1Color() const {return m_line1Color;}
    void setLine1Color(const QColor color) {m_line1Color = color;}
    QColor line2Color() const {return m_line2Color;}
    void setLine2Color(const QColor color) {m_line2Color = color;}
    QColor line3Color() const {return m_line3Color;}
    void setLine3Color(const QColor color) {m_line3Color = color;}
    QColor line4Color() const {return m_line4Color;}
    void setLine4Color(const QColor color) {m_line4Color = color;}
    QColor line5Color() const {return m_line5Color;}
    void setLine5Color(const QColor color) {m_line5Color = color;}
    QColor line6Color() const {return m_line6Color;}
    void setLine6Color(const QColor color) {m_line6Color = color;}

    HorizontalLabelsKind horizontalLabelsKind() const {return m_horizontalLabelsKind;}
    void setHorizontalLabelsKind(const HorizontalLabelsKind kind);

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

    double hScale() const {return m_hScale;}
    void setHScale(const double hScale);

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
     * \brief Доступ к маркерам
     */
    int markerCount(const int numArea) const;
    MarkerInfo marker(const int numArea, const int idx) const;
    void addMarker(const int numArea, const int pos, const QString comment);

    /*!
     * \brief Устанавливает курсор в зоне zoneNum на позицию pos
     * \param zoneNum - номер зоны
     * \param pos - позиция (номер отсчета)
     */
    void setCursorOnPosition(const int numArea, const int pos);

    /*!
     * \brief Доступ к свойству isFillBetweenSubchans
     */
    bool isFillBetweenSubchans(const int numArea) const;
    void setIsFillBetweenSubchans(const int numArea, const bool isFill);

    bool isShowCursor() const {return m_isShowCursor;}
    void setIsShowCursor(const bool isShow) {m_isShowCursor = isShow;}

    /*!
     * \brief Возвращает координату курсора по зоне 0
     */
    int cursorPos() const;

    QList<double> cursorValues() const;

    /*!
     * \brief Возвращает время в секундах позиции по координате на теле виджета
     * Если x < левой оси, возвращает 0
     * Если x > правой границы, возвращает максимум
     * \param x - значение координаты
     */
    double getTime(const int x) const;

    /*!
     * \brief Возращает координату на теле виджета временной позиции
     * Если x < 0, возвращает положение левой оси
     * Если x > максимума, возвращает значение правой границы
     * \param time - время в секундах
     */
    int getXPosition(const double time) const;

    int leftSpace() const;
    int rightSpace() const;
    int topSpace() const;
    int bottomSpace() const;
    /*!
     * \brief Возвращает ширину зоны построения
     */
    int areaWidth() const;

    /*!
     * \brief Выделение зоны по времени на графике
     * \param x1, x2 - границы участка в координатах тела виджета.
     * Допустимо x1 < x2 и  x1 > x2
     */
    void selectArea(const int x1, const int x2);

    void clearSelectArea();

    /*!
     * \brief Возвращает выделенную зону по времени
     * \param begin - начало зоны в отсчетах
     * \param end - конец зоны в отсчетах
     */
    void selectedArea(int &begin, int &end);

signals:
    void moveCursor();

    /*!
     * \brief Сигнал нажатия мышки на теле виджета
     * \param x, y - координаты нажатия
     * \param buttons - нажатые кнопки
     */
    void press(const int x, const int y, const Qt::MouseButtons buttons);
    /*!
     * \brief Сигнал отпускания мышки на теле виджета
     * \param x, y - координаты отпускания
     * \param buttons - нажатые кнопки
     */
    void release(const int x, const int y, const Qt::MouseButtons buttons);
    /*!
     * \brief Сигнал переноса мышки по телу виджета
     * \param x, y - координаты положения
     * \param buttons - нажатые кнопки
     */
    void move(const int x, const int y, const Qt::MouseButtons buttons);

    /*!
     * \brief Процедура прорисовки на рисователе m_painter  в рамках m_geometry
     */
    void doPaint(const double ratio);


protected:
    virtual void doUpdate();
    virtual void onMousePressEvent(QMouseEvent *event);
    virtual void onMouseReleaseEvent(QMouseEvent *event);
    virtual void onMouseMoveEvent(QMouseEvent *event);

    virtual void doMoveCursor();


    QColor m_line1Color {Qt::darkGreen};
    QColor m_line2Color {Qt::blue};
    QColor m_line3Color {Qt::darkCyan};
    QColor m_line4Color {Qt::red};
    QColor m_line5Color {Qt::darkYellow};
    QColor m_line6Color {Qt::darkMagenta};

private:
    void init();

    QPainter* m_painter {nullptr};
    QRect m_geometry {QRect(0, 0, 0, 0)};
    QWidget* m_widget {nullptr};

    QList<GraphArea*> m_areases;
    GraphEnvColors m_envColors;
//    QColor m_line1Color {Qt::darkGreen};
//    QColor m_line2Color {Qt::blue};
//    QColor m_line3Color {Qt::darkCyan};
//    QColor m_line4Color {Qt::red};
//    QColor m_line5Color {Qt::darkYellow};
//    QColor m_line6Color {Qt::darkMagenta};

    XCoordSignalMode m_xcsm {xsm_fullSignal}; ///< Режим отображения сигнала по горизонтали
    int m_startPoint {0};                     ///< Начальная точка прорисовки в режиме скроллинга
    double m_hScale {1};                      ///< Множитель горизонтального масштабирования (увеличения)
    bool m_isZeroing {false};                 ///< Центровка

    bool m_isShowCursor {false};
    int m_cursorX {0};
    int m_cursorY {0};
    int m_selectAreaBegin {-1};
    int m_selectAreaEnd {-1};

    HorizontalLabelsKind m_horizontalLabelsKind {hlkSeconds};
};

#endif // GRAPHPAINTER_H
