#ifndef KRGPAINTER_H
#define KRGPAINTER_H

#include <QObject>
#include <QWidget>
#include <QPainter>

class Ritmogram;

/*!
 * \brief Класс рисователя скаттерограммы The KRGPainter class
 */
class KRGPainter
{
public:
    explicit KRGPainter(QPainter* painter, QRect geometry);
    explicit KRGPainter();
    virtual ~KRGPainter();

    /*!
     * \brief Задает канву прорисовки. Будет испльзоваться в режиме виджета для задания
     * \param painter - рисователь
     * \param geometry - размер области прорисовки
     */
    void setCanvas(QPainter* painter, QRect geometry, QWidget *wgt = nullptr);

    int leftSpace() const;
    int rightSpace() const;
    int topSpace() const;
    int bottomSpace() const;

    /*!
     * \brief Процедура прорисовки на рисователе m_painter  в рамках m_geometry
     */
    void doPaint(const double ratio);

    /*!
     * \brief Получение указателя на объект сигнала
     */
    void appendSignal(Ritmogram* signal);

    QString nameHorizontal() const {return m_nameHorizontal;}
    void setNameHorizontal(const QString name) {m_nameHorizontal = name;}

    QString nameVertical() const {return m_nameVertical;}
    void setNameVertical(const QString name) {m_nameVertical = name;}

protected:
    virtual void doUpdate();

    QColor m_colorAxis {Qt::black};
    QColor m_colorKRG {Qt::blue};
    QColor m_colorKRGBound {Qt::darkBlue};

private:
    QPainter* m_painter {nullptr};
    QRect m_geometry {QRect(0, 0, 0, 0)};
    QWidget* m_widget {nullptr};

    Ritmogram* m_signal {nullptr};

    QString m_nameHorizontal {""};
    QString m_nameVertical {""};
};

#endif // KRGPAINTER_H
