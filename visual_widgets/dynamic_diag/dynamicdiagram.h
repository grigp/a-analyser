#ifndef DYNAMICDIAGRAM_H
#define DYNAMICDIAGRAM_H

#include <QWidget>

namespace Ui {
class DynamicDiagram;
}

/*!
 * \brief Класс элемента диаграммы (отсчета) DiagItem class
 */
class DiagItem
{
public:
    DiagItem(double value, QString name)
        : m_value(value), m_name(name)
    {}

    double value() const {return m_value;}
    QString name() const {return m_name;}

private:
    double m_value {0};
    QString m_name {""};
};

/*!
 * \brief Класс диаграммы построения динамики DynamicDiagram class
 * Построение динамики для одного набора данных
 * Варианты представления:
 * - график
 * - столбиковая диаграмма
 * Форма: 2D и 3D
 */
class DynamicDiagram : public QWidget
{
    Q_OBJECT

public:
    explicit DynamicDiagram(QWidget *parent = 0);
    ~DynamicDiagram();

    ///< Вид динамики
    enum Kind
    {
          KindGraph = 0   ///< График
        , KindBar         ///< Столбиковая диаграмма
    };

    ///< Объем
    enum Volume
    {
          Volume2D = 0  ///< Двухмерная диаграмма
        , Volume3D      ///< Трехмерная диаграмма
    };

    Kind kind() const;
    void setKind(const Kind kind);

    Volume volume() const;
    void setVolume(const Volume volume);

    QString title() const;
    void setTitle(const QString &title);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    QColor diagColor() const;
    void setDiagColor(const QColor &color);

    QColor titleColor() const;
    void setTitleColor(const QColor &color);

    void appendItem(DiagItem* item);
    void insertItem(const int idx, DiagItem* item);

    int size() const {return m_items.size();}

    double value(const int idx) const;
    QString itemName(const int idx) const;

    void clear();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::DynamicDiagram *ui;

    /*!
     * \brief Рассчитывает минимум и максимум значений на диаграмме
     * \param min, max - возвращаемые значения минимума и максимума
     */
    void computeDiap(double &min, double &max);

    Kind m_kind {KindBar};
    Volume m_volume {Volume2D};
    QString m_title {""};

    QColor m_backgroundColor {Qt::white};
    QColor m_diagColor {Qt::blue};
    QColor m_titleColor {Qt::black};

    QList<DiagItem*> m_items;
};

#endif // DYNAMICDIAGRAM_H