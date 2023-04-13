#ifndef DUALSTATEDIAGRAMPAINTER_H
#define DUALSTATEDIAGRAMPAINTER_H

#include <QObject>
#include <QWidget>
#include <QPainter>

/*!
 * \brief Класс рисователя диаграммы двунаправленного отображения состояния относительно нуля The DualStateDiagramPainter class
 */
class DualStateDiagramPainter
{
public:
    explicit DualStateDiagramPainter();
    explicit DualStateDiagramPainter(QPainter* painter, QRect geometry);
    virtual ~DualStateDiagramPainter();

    /*!
     * \brief Задает канву прорисовки. Будет испльзоваться в режиме виджета для задания
     * \param painter - рисователь
     * \param geometry - размер области прорисовки
     */
    void setCanvas(QPainter* painter, QRect geometry, QWidget *wgt = nullptr);

    QColor frameColor() const {return m_frameColor;}
    void setFrameColor(const QColor color) {m_frameColor = color;}

    QColor labelColor() const {return m_labelsColor;}
    void setLabelColor(const QColor color) {m_labelsColor = color;}

    QColor insignificantlyColor() const {return m_insignificantlyColor;}
    void setInsignificantlyColor(const QColor color) {m_insignificantlyColor = color;}

    QColor moderateColor() const {return m_moderateColor;}
    void setModerateColor(const QColor color) {m_moderateColor = color;}

    QColor averageColor() const {return m_averageColor;}
    void setAverageColor(const QColor color) {m_averageColor = color;}

    QColor valueColor() const {return m_valuerColor;}
    void setValueColor(const QColor color) {m_valuerColor = color;}

    double diap() const {return m_diap;}
    void setDiap(const double diap) {m_diap = diap;}

    double insignificantlyLo() const {return m_insignificantlyLo;}
    void setInsignificantlyLo(const double val) {m_insignificantlyLo = val;}

    double moderateLo() const {return m_moderateLo;}
    void setModerateLo(const double val) {m_moderateLo = val;}

    double averageLo() const {return m_averageLo;}
    void setAverageLo(const double val) {m_averageLo = val;}

    double value() const {return m_value;}
    void setValue(const double val) {m_value = val;}

    QString descriptionLeft() const {return m_descriptionLeft;}
    void setDescriptionLeft(const QString desc) {m_descriptionLeft = desc;}

    QString descriptionRight() const {return m_descriptionRight;}
    void setDescriptionRight(const QString desc) {m_descriptionRight = desc;}

    /*!
     * \brief Процедура прорисовки на рисователе m_painter  в рамках m_geometry
     */
    void doPaint(const double ratio);


protected:
    virtual void doUpdate();

private:
    QPainter* m_painter {nullptr};
    QRect m_geometry {QRect(0, 0, 0, 0)};
    QWidget* m_widget {nullptr};

    QColor m_frameColor {Qt::darkGray};
    QColor m_labelsColor {Qt::black};
    QColor m_insignificantlyColor {Qt::green};
    QColor m_moderateColor {Qt::yellow};
    QColor m_averageColor {Qt::red};
    QColor m_valuerColor {Qt::darkCyan};

    double m_diap {100};
    double m_insignificantlyLo {10};
    double m_moderateLo {30};
    double m_averageLo {50};
    double m_value {0};

    QString m_descriptionLeft {""};
    QString m_descriptionRight {""};
};

#endif // DUALSTATEDIAGRAMPAINTER_H
