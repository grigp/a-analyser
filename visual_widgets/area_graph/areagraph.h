#ifndef AREAGRAPH_H
#define AREAGRAPH_H

#include <QWidget>

namespace Ui {
class AreaGraph;
}

/*!
 * \brief Класс виджета построения графиков AreaGraph class
 */
class AreaGraph : public QWidget
{
    Q_OBJECT

public:
    explicit AreaGraph(QWidget *parent = 0);
    ~AreaGraph();

private:
    Ui::AreaGraph *ui;
};

#endif // AREAGRAPH_H
