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

    Q_PROPERTY(QColor diag_color READ diagColor WRITE setDiagColor DESIGNABLE true)
    Q_PROPERTY(QColor selectItem_color READ selectItemColor WRITE setSelectItemColor DESIGNABLE true)

public:
    explicit DynamicDiagram(QWidget *parent = nullptr);
    ~DynamicDiagram() override;

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

    QString bottomText() const;
    void setBottomText(const QString &text);

    QColor diagColor() const;
    void setDiagColor(const QColor &color);

    QColor selectItemColor() const;
    void setSelectItemColor(const QColor &color);

    QColor titleColor() const;
    void setTitleColor(const QColor &color);

    int titleHeight() const {return m_titleHeight;}
    void setTitleHeight(const int height);

    int axisSpaceLeft() const {return m_axisSpaceLeft;}
    void setAxisSpaceLeft(const int left);

    int axisSpaceBottom() const {return m_axisSpaceBottom;}
    void setAxisSpaceBottom(const int bottom);

    void appendItem(DiagItem* item);
    void insertItem(const int idx, DiagItem* item);

    int size() const {return m_items.size();}

    double value(const int idx) const;
    QString itemName(const int idx) const;

    /*!
     * \brief Устанавливает границы диапазона отображения
     * \param min - минимум
     * \param max - максимум
     */
    void setDiap(const double min, const double max);

    void clear();

    /*!
     * \brief Команда выделения итема снаружи
     * \param idx - индекс выделяемого элемента
     */
    void doSelectItem(const int idx);

    void setSizeDivider(const int sd) {m_sizeDivider = sd;}

signals:
    /*!
     * \brief Возникает при выборе итема мышкой
     * \param idx - индекс выбранного итема
     */
    void selectItem(const int idx);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::DynamicDiagram *ui;

    /*!
     * \brief Рассчитывает минимум и максимум значений на диаграмме
     * \param min, max - возвращаемые значения минимума и максимума
     */
    void computeDiap(double &min, double &max);

    /*!
     * \brief Возвращает цвет темнее базового
     * Раскладывает его на составляющие rgb
     * Затем по каждой составляющей делает темнее вполовину
     * \param colBase - базовый цвет
     */
    QColor getDarkColor(const QColor &colBase) const;

    /*!
     * \brief Рисует сетку по оси значений
     * \param painter - рисователь
     * \param min, max - мин и макс значения
     * \param prop - пропорция
     */
    void showValuesGrid(QPainter &painter,
                        const double min, const double max,
                        const double prop);

    int m_titleHeight {25};
    int m_axisSpaceLeft {50};
    int m_axisSpaceBottom {50};

    Kind m_kind {KindGraph};
    Volume m_volume {Volume2D};
    QString m_title {""};
    QString m_bottomText {""};

    QColor m_diagColor {Qt::darkCyan};
    QColor m_selectItemColor {QColor(0, 194, 194)};
    QColor m_titleColor {Qt::black};

    double m_sizeH {0};
    int m_sizeDivider {5};   ///< Ширина столбика "скважность" sizeOne / m_sizeDivider
    int m_selectItem {-1};   ///< Выбранный элемент

    double m_minimum {INT_MAX};
    double m_maximum {INT_MAX};

    QList<DiagItem*> m_items;
};

#endif // DYNAMICDIAGRAM_H
