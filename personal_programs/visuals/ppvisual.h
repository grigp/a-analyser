#ifndef PPVISUAL_H
#define PPVISUAL_H

#include <QWidget>
#include <QJsonObject>

class PPVisualDescriptor;

/*!
 * \brief Базовый класс визуализатора индивидуальных программ The PPVisual class
 */
class PPVisual : public QWidget
{
    Q_OBJECT

public:
    explicit PPVisual(PPVisualDescriptor* visual, QWidget *parent = nullptr);
    ~PPVisual() override;

    virtual bool isValid() = 0;
    virtual void calculate() = 0;

private:
    PPVisualDescriptor* m_visual {nullptr};
};

#endif // PPVISUAL_H
