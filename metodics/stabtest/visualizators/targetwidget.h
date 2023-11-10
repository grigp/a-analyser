#ifndef TARGETWIDGET_H
#define TARGETWIDGET_H

#include <QWidget>
#include <QPrinter>

namespace Ui {
class TargetWidget;
}

class TargetCalculator;
class DynamicDiagram;
class SKGWidget;
class TestCalculator;

/*!
 * \brief Класс виджета визуализатора мишени в рамках стабилографического теста TargetWidget class
 */
class TargetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TargetWidget(QWidget *parent = nullptr);
    ~TargetWidget() override;

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(TargetCalculator *calculator, const QString &testUid);

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

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    Ui::TargetWidget *ui;

    void showSKG(TargetCalculator *calculator, const QString &testUid);
    void showDiagram(TargetCalculator *calculator, const QString &testUid);

    DynamicDiagram *m_wgtDiagram {nullptr};
    SKGWidget *m_wgtSKG {nullptr};
    QString m_sScore {""};
    QString m_sTotalScore {""};
    TargetCalculator *m_calculator {nullptr};

    int m_tmSetDiap {-1};
    double m_maxValue {0};
};

#endif // TARGETWIDGET_H
