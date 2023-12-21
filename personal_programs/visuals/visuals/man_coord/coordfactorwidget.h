#ifndef COORDFACTORWIDGET_H
#define COORDFACTORWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class CoordFactorWidget;
}

/*!
 * \brief Класс виджета показа и анализа значения одного из факоров координированности The CoordFactorWidget class
 */
class CoordFactorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CoordFactorWidget(QWidget *parent = nullptr);
    ~CoordFactorWidget();

    /*!
     * \brief Устанавливает название показателя
     * \param number - номер показателя
     * \param name - название показателя
     * \param value - значение показателя
     */
    void setFactor(const int number, const QString &name, const double value);

    /*!
     * \brief Устанавливает значение показателя
     */
    void setValue(const double value);

    /*!
     * \brief Устанавливает составляющую показателя
     * \param name - название показателя составляющей
     * \param value - значение
     * \param percent - процент
     * \param min - мин. граница
     * \param max - макс. граница
     */
    void setComponent(const QString& name, const QString& value, const QString& percent, const QString& min, const QString& max);

private slots:
    void on_expand_collapse();

private:
    Ui::CoordFactorWidget *ui;

    QStandardItemModel m_mdlComponents;
};

#endif // COORDFACTORWIDGET_H
