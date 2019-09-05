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

    GraphEnvColors() {}
};

class SignalAccess;

/*!
 * \brief Класс, содержащий данные для зоны построения графика GraphArea class
 */
class GraphArea
{
public:
    explicit GraphArea(SignalAccess* signal, const int numSubChan);

    SignalAccess* signal() const {return m_signal;}

    int numSubChan() {return m_numSubChan;}

    QColor color(const int colorNum) const;

    void setColor(const int colorNum, const QColor &color);

private:
    SignalAccess* m_signal {nullptr};
    int m_numSubChan {-1};   ///< Номер отображаемого подканала. Если он равен -1, то отображаются все подканалы сигнала в одной зоне
    QVector<QColor> m_palette;
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
    void appendSignal(SignalAccess* signal, const int numSubChan = -1);

    /*!
     * \brief Возвращает кол-во зон построения
     */
    int areasesCount() const {return m_areases.size();}

    QColor colorBackground() const {return m_envColors.colorBackground;}
    void setColorBackground(const QColor &color) {m_envColors.colorBackground = color;}

    QColor colorAxis() const {return m_envColors.colorAxis;}
    void setColorAxis(const QColor &color) {m_envColors.colorAxis = color;}

    QColor colorGrid() const {return m_envColors.colorGrid;}
    void setColorGrid(const QColor &color) {m_envColors.colorGrid = color;}

    QColor colorLabels() const {return m_envColors.colorLabels;}
    void setColorLabels(const QColor &color) {m_envColors.colorLabels = color;}

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::AreaGraph *ui;

    QList<GraphArea*> m_areases;
    GraphEnvColors m_envColors;
};

#endif // AREAGRAPH_H
