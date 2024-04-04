#ifndef BABYPOSTUREWIDGET_H
#define BABYPOSTUREWIDGET_H

#include <QWidget>
#include <QPrinter>
#include <QFileDialog>

namespace Ui {
class BabyPostureWidget;
}

class BabyPostureCalculator;
class TestCalculator;

/*!
 * \brief Класс виджета визуализатора постурального контроля детей первого года жизни в рамках стабилографического теста The BabyPostureWidget class
 */
class BabyPostureWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BabyPostureWidget(QWidget *parent = nullptr);
    ~BabyPostureWidget();

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(BabyPostureCalculator *calculator, const QString &testUid);

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


private:
    Ui::BabyPostureWidget *ui;
};

#endif // BABYPOSTUREWIDGET_H
