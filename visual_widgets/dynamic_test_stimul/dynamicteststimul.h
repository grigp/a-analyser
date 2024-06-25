#ifndef DYNAMICTESTSTIMUL_H
#define DYNAMICTESTSTIMUL_H

#include <QWidget>

namespace Ui {
class DynamicTestStimul;
}

/*!
 * \brief Структура данных о цели TargetInfo struct
 */
struct TargetInfo
{
    double x;
    double y;
    QColor color;
    QString label;
    bool visible;
    int size;
};

/*!
 * \brief Класс виджета стимуляции пациента динамического теста DynamicTestStimul class
 */
class DynamicTestStimul : public QWidget
{
    Q_OBJECT

public:
    explicit DynamicTestStimul(QWidget *parent = nullptr);
    ~DynamicTestStimul() override;

    void setDiap(const int diap) {m_diap = diap;}
    int diap() const {return m_diap;}

    void setMarker(const double x, const double y);

    void setBackgroundColor(const QColor color) {m_backgroundColor = color;}
    QColor backgroundColor() const {return m_backgroundColor;}

    void setMarkerColor(const QColor color) {m_markerColor = color;}
    QColor markerColor() const {return m_markerColor;}

    void setVisibleMarker(const bool visible) {m_visibleMarker = visible;}
    bool visibleMarker() const {return m_visibleMarker;}

    void setMarkerSize(const int size) {m_markerSize = size;}
    int markerSize() const {return m_markerSize;}

    /*!
     * \brief Инициализирует count целей по центру с заданным цветом и размером
     * \param count - кол-во целей
     * \param color - цвет целей
     * \param size - размер целей
     */
    void initTargets(const int count, const QColor color, const int size);

    /*!
     * \brief Добавляет цель
     * \param x, y - координаты цели в мм
     * \param color - цвет цели
     * \param size - размер цели
     * \param label - подпись цели
     */
    void addTarget(const double x, const double y, const QColor color, const int size, const bool visible = true, const QString label = "");

    /*!
     * \brief Устанавливает параметры цели с заданным номером
     * \param idx - номер цели
     * \param color - цвет цели
     * \param size - размер цели
     * \param label - подпись цели
     */
    void setTargetInfo(const int idx, const QColor color, const int size, const QString label = "");

    /*!
     * \brief Устанавливает цель с номером на позицию в миллиметрах
     * \param idx - номер цели
     * \param x, y - координаты цели
     */
    void setTarget(const double x, const double y, const int idx = 0);

    /*!
     * \brief Очищает список целей
     */
    void clearTargets();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::DynamicTestStimul *ui;

    int m_diap {128};
    double m_x {0};
    double m_y {0};
    int m_markerSize {20};
    bool m_visibleMarker {true};

    QColor m_backgroundColor {Qt::darkGray};
    QColor m_markerColor {Qt::red};

    QList<TargetInfo> m_targets;
};

#endif // DYNAMICTESTSTIMUL_H
