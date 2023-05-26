#ifndef KRGWIDGET_H
#define KRGWIDGET_H

#include <QWidget>

#include "krgpainter.h"

namespace Ui {
class KRGWidget;
}

/*!
 * \brief Класс виджета скаттерограммы The KRGWidget class
 */
class KRGWidget : public QWidget, public KRGPainter
{
    Q_OBJECT

public:
    explicit KRGWidget(QWidget *parent = nullptr);
    ~KRGWidget() override;

protected:
    void paintEvent(QPaintEvent *event) override;

    void doUpdate() override;

private:
    Ui::KRGWidget *ui;

    QColor m_backgroundColor {Qt::white};
};

#endif // KRGWIDGET_H
