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
    explicit PPVisual(PPVisualDescriptor* visual, QJsonObject &objPP, QWidget *parent = nullptr);
    ~PPVisual() override;

    virtual QString name();

    virtual bool isValid() = 0;
    virtual void calculate() = 0;
    virtual void print() = 0;

protected:
    QJsonObject objectPP() {return m_objPP;}

private:
    PPVisualDescriptor* m_visual {nullptr};
    QJsonObject m_objPP {QJsonObject()};
};

#endif // PPVISUAL_H
