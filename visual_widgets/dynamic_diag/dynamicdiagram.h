#ifndef DYNAMICDIAGRAM_H
#define DYNAMICDIAGRAM_H

#include <QWidget>

#include "dynamicdiagrampainter.h"

namespace Ui {
class DynamicDiagram;
}

/*!
 * \brief Класс диаграммы построения динамики DynamicDiagram class
 * Построение динамики для одного набора данных
 * Варианты представления:
 * - график
 * - столбиковая диаграмма
 * Форма: 2D и 3D
 */
class DynamicDiagram : public QWidget, public DynamicDiagramPainter
{
    Q_OBJECT

    Q_PROPERTY(QColor diag_color READ diagColor WRITE setDiagColor DESIGNABLE true)
    Q_PROPERTY(QColor selectItem_color READ selectItemColor WRITE setSelectItemColor DESIGNABLE true)

public:
    explicit DynamicDiagram(QWidget *parent = nullptr);
    ~DynamicDiagram() override;

signals:
    /*!
     * \brief Возникает при выборе итема мышкой
     * \param idx - индекс выбранного итема
     */
    void selectItem(const int idx);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    void doUpdate() override;

private:
    Ui::DynamicDiagram *ui;

};

#endif // DYNAMICDIAGRAM_H
