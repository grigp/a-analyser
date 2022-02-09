#ifndef VISUAL_H
#define VISUAL_H

#include <QWidget>

class BaseVisual;

/*!
 * \brief Базовый класс виджета визуализатора The Visual class
 */
class Visual : public QWidget
{
    Q_OBJECT

public:
    explicit Visual(BaseVisual* visual, QWidget *parent = nullptr);
    ~Visual() override;

    virtual void calculate() = 0;

private:

    BaseVisual* m_visual {nullptr};
};

#endif // VISUAL_H
