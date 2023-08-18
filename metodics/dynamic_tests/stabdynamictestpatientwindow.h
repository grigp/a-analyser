#ifndef STABDYNAMICTESTPATIENTWINDOW_H
#define STABDYNAMICTESTPATIENTWINDOW_H

#include <QDialog>

namespace Ui {
class StabDynamicTestPatientWindow;
}

class StabDynamicTestPatientWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StabDynamicTestPatientWindow(QWidget *parent = nullptr);
    ~StabDynamicTestPatientWindow();

    /*!
     * \brief Действия при начале записи
     */
    virtual void run();

    /*!
     * \brief Действия при завершении / прерывании записи
     */
    virtual void stop();

    /*!
     * \brief Виртуальная функция установки диапазона изменения координат
     * \param diap - диапазон
     */
    virtual void setDiap(const int diap);
    int diap() const {return  m_diap;}

    /*!
     * \brief Установка маркера
     * \param x, y - координаты маркера
     */
    virtual void setMarker(const double x, const double y);
    double x() const {return m_X;}
    double y() const {return m_Y;}

    /*!
     * \brief Управление видимостью маркера
     */
    virtual void setVisibleMarker(const bool isVisible);
    bool isVisibleMarker() const;

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
    void addTarget(const double x, const double y, const QColor color, const int size, const QString label = "");

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

    /*!
     * \brief Возвращает указатель на макет (layout) виджета
     */
    QLayout *getWidgetLayout();

    /*!
     * \brief Виртуальная функция, помещающая комментарий поверх окна
     * \param comment - текст комментария
     */
    virtual void setFrontComment(const QString& comment);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::StabDynamicTestPatientWindow *ui;

    int m_diap {128};
    double m_X {0};
    double m_Y {0};
};

#endif // STABDYNAMICTESTPATIENTWINDOW_H
