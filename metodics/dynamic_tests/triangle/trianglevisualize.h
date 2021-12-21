#ifndef TRIANGLEVISUALIZE_H
#define TRIANGLEVISUALIZE_H

#include <QWidget>
#include <QPrinter>

namespace Ui {
class TriangleVisualize;
}

class TriangleCalculator;

/*!
 * \brief Класс виджета показа результатов теста Треугольник TriangleVisualize class
 */
class TriangleVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit TriangleVisualize(QWidget *parent = nullptr);
    ~TriangleVisualize();

    void setTest(const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

private slots:
    void curPageChanged(int pageIdx);


private:
    Ui::TriangleVisualize *ui;

    TriangleCalculator* m_calculator {nullptr};
};

#endif // TRIANGLEVISUALIZE_H
