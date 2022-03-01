#ifndef VECTORCLOUD_H
#define VECTORCLOUD_H

#include <QWidget>

namespace Ui {
class VectorCloud;
}

/*!
 * \brief Класс виджета диаграммы облака векторов The VectorCloud class
 */
class VectorCloud : public QWidget
{
    Q_OBJECT

public:
    explicit VectorCloud(QWidget *parent = nullptr);
    ~VectorCloud() override;

    QColor axizColor() const {return m_axizColor;}
    void setAxizColor(const QColor color) {m_axizColor = color;}

    QColor vectorColor() const {return m_vectorColor;}
    void setVectorColor(const QColor color) {m_vectorColor = color;}

    QColor zonesColor() const {return m_zonesColor;}
    void setZonesColor(const QColor color) {m_zonesColor = color;}

    QString title() const {return m_title;}
    void setTitle(const QString title) {m_title = title;}

    /*!
     * \brief Добавляет вектор в список
     * \param vector - вектор
     */
    void add(const QPointF vector);

    /*!
     * \brief Добавляет значение диапазона в список
     * Добавлять надо от малого к большому!!!!
     * \param diap - значение диапазона
     */
    void addDiapLimit(const double diap);

    double diap() const {return m_diap;}
    void setDiapazone(const double diap);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::VectorCloud *ui;

    QColor m_axizColor {Qt::black};
    QColor m_vectorColor {Qt::darkCyan};
    QColor m_zonesColor {Qt::lightGray};

    QString m_title {tr("Облако векторов")};

    double m_diap {1};
    QVector<QPointF> m_vectors;
    QVector<double> m_diaps;
};

#endif // VECTORCLOUD_H
