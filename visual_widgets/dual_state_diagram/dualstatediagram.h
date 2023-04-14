#ifndef DUALSTATEDIAGRAM_H
#define DUALSTATEDIAGRAM_H

#include <QWidget>

#include "dualstatediagrampainter.h"

namespace Ui {
class DualStateDiagram;
}

/*!
 * \brief Класс визуальной диаграммы двунаправленного отображения состояния относительно нуля DualStateDiagram class
 */
class DualStateDiagram : public QWidget, public DualStateDiagramPainter
{
    Q_OBJECT

public:
    explicit DualStateDiagram(QWidget *parent = nullptr);
    ~DualStateDiagram() override;

protected:
    void paintEvent(QPaintEvent *event) override;

    void doUpdate() override;

private:
    Ui::DualStateDiagram *ui;

};

#endif // DUALSTATEDIAGRAM_H
