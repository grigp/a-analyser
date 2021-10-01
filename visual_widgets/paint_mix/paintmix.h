#ifndef PAINTMIX_H
#define PAINTMIX_H

#include <QWidget>
#include <QPixmap>
#include <QMap>

namespace Ui {
class PaintMix;
}


/*!
 * \brief Класс виджета, содержащего картинки PaintMix class
 */
class PaintMix : public QWidget
{
    Q_OBJECT

public:
    explicit PaintMix(QWidget *parent = nullptr);
    ~PaintMix() override;

    /*!
     * \brief Структура данных о картинке PixmapInfo struct
     */
    struct PixmapInfo
    {
        QPixmap pixmap;  ///< Сама картинка
        QRectF rect;     ///< Позиция картинки
        bool isVisible;  ///< Видима ли

    };

    /*!
     * \brief Устанавливает фоновую картинку
     * \param fn - имя файла из ресурсов
     */
    void setBackground(const QString fn);

    /*!
     * \brief Добавляет картинку в список
     * \param code - уникальный код картинки
     * \param fn - имя файла из ресурсов
     * \param rect - прямоугольник
     * \param isVisible - будет ли видима
     */
    void addPicture(const int code, const QString fn, const QRectF rect, const bool isVisible);

    /*!
     * \brief Управляет видимостью картинки
     * \param code - уникальный код картинки
     * \param isVisible - видиммость
     */
    void setIsVisible(const int code, const bool isVisible);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::PaintMix *ui;

    QPixmap m_backgroung;
    QMap<int, PixmapInfo> m_pixmaps;
};

#endif // PAINTMIX_H
