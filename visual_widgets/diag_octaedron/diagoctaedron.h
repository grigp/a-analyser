#ifndef DIAGOCTAEDRON_H
#define DIAGOCTAEDRON_H

#include <QWidget>

namespace Ui {
class DiagOctaedron;
}

/*!
 * \brief Класс диаграммы результатов тренажера "Октаэдр" DiagOctaedron class
 */
class DiagOctaedron : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor line_color READ lineColor WRITE setLineColor DESIGNABLE true)
    Q_PROPERTY(QColor label_color READ labelColor WRITE setLabelColor DESIGNABLE true)
    Q_PROPERTY(QColor labelText_color READ labelTextColor WRITE setLabelTextColor DESIGNABLE true)
    Q_PROPERTY(QColor labelFrame_color READ labelFrameColor WRITE setLabelFrameColor DESIGNABLE true)

public:
    explicit DiagOctaedron(QWidget *parent = nullptr);
    ~DiagOctaedron() override;

    void setLineColor(const QColor color) {m_lineColor = color;}
    QColor lineColor() const {return m_lineColor;}

    void setLabelColor(const QColor color) {m_labelColor = color;}
    QColor labelColor() const {return m_labelColor;}

    void setLabelTextColor(const QColor color) {m_labelTextColor = color;}
    QColor labelTextColor() const {return m_labelTextColor;}

    void setLabelFrameColor(const QColor color) {m_labelFrameColor = color;}
    QColor labelFrameColor() const {return m_labelFrameColor;}

    void setCirceRoundRuleMode(const int mode) {m_mode = mode;}
    int circeRoundRuleMode() const {return m_mode;}

    void setDirection(const int dir) {m_direction = dir;}
    int direction() const {return m_direction;}

    void setData(const int idx, const int value);
    int data(const int idx) const;

    void endUpdate();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::DiagOctaedron *ui;

    QColor m_lineColor {QColor(0, 90, 150)};
    QColor m_labelColor {QColor(0, 90, 150)};
    QColor m_labelTextColor {Qt::white};
    QColor m_labelFrameColor {Qt::black};

    int m_mode {0};      ///< Режим просмотра 0 - радиальный, 1 - кольцевой
    int m_direction {1}; ///< Направление обхода 1 - по часовой, 2 - против часовой (для кольцевого режима)
    int m_data[8] {0, 0, 0, 0, 0, 0, 0, 0};   ///< Данные m_data[0] - верх -> далее против часовой до m_data[7]
};

#endif // DIAGOCTAEDRON_H
