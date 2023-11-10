#ifndef STABTESTVISUALIZE_H
#define STABTESTVISUALIZE_H

#include <QWidget>
#include <QTextEdit>
#include <QPrinter>

namespace Ui {
class StabTestVisualize;
}

class TestResultData;

class TestCalculator;

/*!
 * \brief Виджет показа результатов стабилометрического тестирования StabTestVisualize class
 */
class StabTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit StabTestVisualize(TestCalculator* calculator, QWidget *parent = nullptr);
    ~StabTestVisualize() override;

    void setTest(const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

    /*!
     * \brief Прорисовывает предварительные результаты с помощью заданного рисователя
     * \param painter - рисователь
     * \param rect - зона прорисовки
     * \param testUid - uid теста
     */
    static void paintPreview(QPainter* painter, QRect& rect, const QString& testUid, TestCalculator* calculator);

    /*!
     * \brief Возвращает указатель на виджет визуализации для конкретного теста
     */
    QWidget* widget() {return m_widget;}

private:
    Ui::StabTestVisualize *ui;

    TestCalculator* m_calculator;
    QWidget *m_widget {nullptr};

};

#endif // STABTESTVISUALIZE_H
