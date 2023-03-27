#ifndef CROSSPAINTER_H
#define CROSSPAINTER_H

#include <QObject>
#include <QWidget>
#include <QPainter>

/*!
 * \brief Класс рисователя диаграммы теста на устойчивость The CrossPainter class
 */
class CrossPainter
{
public:
    explicit CrossPainter(QPainter* painter, QRect geometry);
    explicit CrossPainter();
    virtual ~CrossPainter();

    /*!
     * \brief Задает канву прорисовки. Будет испльзоваться в режиме виджета для задания
     * \param painter - рисователь
     * \param geometry - размер области прорисовки
     */
    void setCanvas(QPainter* painter, QRect geometry, QWidget *wgt = nullptr);

    void setFrameColor(const QColor color) {m_frameColor = color;}
    QColor frameColor() const {return m_frameColor;}

    void setFrontalColor(const QColor color) {m_frontalColor = color;}
    void setSagittalColor(const QColor color) {m_sagittalColor = color;}

    void setValueUp(const int value);
    int valueUp() const {return m_valueUp;}

    void setValueDown(const int value);
    int valueDown() const {return m_valueDown;}

    void setValueRight(const int value);
    int valueRight() const {return m_valueRight;}

    void setValueLeft(const int value);
    int valueLeft() const {return m_valueLeft;}

    void setDiap(const int value) {m_diap = value;}
    int diap() const {return m_diap;}

    void setIsShowValueUp(const bool value) {m_isShowValueUp = value;}
    bool isShowValueUp() const {return m_isShowValueUp;}

    void setIsShowValueDown(const bool value) {m_isShowValueDown = value;}
    bool isShowValueDown() const {return m_isShowValueDown;}

    void setIsShowValueRight(const bool value) {m_isShowValueRight = value;}
    bool isShowValueRight() const {return m_isShowValueRight;}

    void setIsShowValueLeft(const bool value) {m_isShowValueLeft = value;}
    bool isShowValueLeft() const {return m_isShowValueLeft;}

public slots:
    QColor frontalColor() const {return m_frontalColor;}
    QColor sagittalColor() const {return m_sagittalColor;}

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
    QColor m_frontalColor {QColor(0, 118, 163)};
    QColor m_sagittalColor {QColor(0, 166, 80)};

    int m_valueUp {0};
    int m_valueDown {0};
    int m_valueRight {0};
    int m_valueLeft {0};
    int m_diap {1};

    bool m_isShowValueUp {true};
    bool m_isShowValueDown {true};
    bool m_isShowValueRight {true};
    bool m_isShowValueLeft {true};

};

#endif // CROSSPAINTER_H
