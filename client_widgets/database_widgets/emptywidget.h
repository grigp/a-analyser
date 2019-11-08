#ifndef EMPTYWIDGET_H
#define EMPTYWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPixmap>

namespace Ui {
class EmptyWidget;
}

/*!
 * \brief Класс виждета, заполняющего зону открытого теста, когда не открыт ни один тест EmptyWidget class
 */
class EmptyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EmptyWidget(QWidget *parent = 0);
    ~EmptyWidget();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::EmptyWidget *ui;

    QPixmap m_picture;
};

#endif // EMPTYWIDGET_H
