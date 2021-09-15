#ifndef VIDEOIRRITANT_H
#define VIDEOIRRITANT_H

#include <QObject>
#include <QGraphicsItem>
#include <QFrame>

/*!
 * \brief Базовый виртуальный класс видео различных раздражений Irriant class
 */
class Irriant : public QObject
{
    Q_OBJECT
public:
    Irriant();
    virtual ~Irriant();

    /*!
     * \brief Возвращает уникальный идентификатор раздражителя
     */
    virtual QString uid() = 0;
    /*!
     * \brief Возвращает показываемое название раздражителя
     */
    virtual QString name() = 0;

    /*!
     * \brief Выполняет прорисовку раздражителя
     * \param painter - рисователь 
     * \param option - опции
     * \param widget - виджет
     * \param boundingRect - границы зоны рисования
     */
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget, QRectF boundingRect) = 0;
    
    /*!
     * \brief Возвращает указатель на виджет редактирования параметров раздражителя
     */
    virtual QWidget* getSettingsWidget() = 0;
};

/*!
 * \brief Класс видеораздражителя для игр (оптокинетическая стимуляция) VidioIrritant class
 */
class VideoIrritant : public QGraphicsItem
{
public:
    explicit VideoIrritant(const QRectF &rect, QGraphicsItem *parent = nullptr);
    ~VideoIrritant() override;

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /*!
     * \brief Доступ к раздражителям
     */
    int irriantsCount() const;
    Irriant *irriant(const int idx) const;

    /*!
     * \brief Устанавливает номер текущего раздражителя
     * \param idx - номер раздражителя в списке
     */
    void setCurrentIrriant(const int idx);

private:
    QRectF m_rect;
    QList<Irriant*> m_irriants; ///< Список допустимых раздражителей
    int m_current {-1};         ///< Индекс текущего раздражителя в списке
};

#endif // VIDEOIRRITANT_H
