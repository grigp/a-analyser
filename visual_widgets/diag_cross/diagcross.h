#ifndef DIAGCROSS_H
#define DIAGCROSS_H

#include <QWidget>
//#include <QComboBox>


namespace Ui {
class DiagCross;
}

/*!
 * \brief Класс диаграммы теста на устойчивость DiagCross class
 */
class DiagCross : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor back_color READ backgroundColor WRITE setBackgroundColor DESIGNABLE true)
    Q_PROPERTY(QColor frontal_color READ frontalColor WRITE setFrontalColor DESIGNABLE true)
    Q_PROPERTY(QColor sagittal_color READ sagittalColor WRITE setSagittalColor DESIGNABLE true)

public:
    explicit DiagCross(QWidget *parent = nullptr);
    ~DiagCross() override;

    void setBackgroundColor(const QColor color) {m_backgroundColor = color;}
    QColor backgroundColor() const {return m_backgroundColor;}

    void setFrameColor(const QColor color) {m_frameColor = color;}
    QColor frameColor() const {return m_frameColor;}

    void setFrontalColor(const QColor color) {m_frontalColor = color;}
    void setSagittalColor(const QColor color) {m_sagittalColor = color;}

    void setValueUp(const int value) {m_valueUp = value; repaint();}
    int valueUp() const {return m_valueUp;}

    void setValueDown(const int value) {m_valueDown = value; repaint();}
    int valueDown() const {return m_valueDown;}

    void setValueRight(const int value) {m_valueRight = value; repaint();}
    int valueRight() const {return m_valueRight;}

    void setValueLeft(const int value) {m_valueLeft = value; repaint();}
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

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::DiagCross *ui;

    //QComboBox box;
    QColor m_backgroundColor {Qt::white};
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

#endif // DIAGCROSS_H
