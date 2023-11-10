#ifndef STRESSSTRATEGYWIDGET_H
#define STRESSSTRATEGYWIDGET_H

#include <QWidget>
#include <QPrinter>

namespace Ui {
class StressStrategyWidget;
}

class StressStrategyCalculator;
class DynamicDiagram;
class TestCalculator;

/*!
 * \brief Класс виджета визуализатора оценки стрессовой стратегии в рамках стабилографического теста StressStrategyWidget class
 */
class StressStrategyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StressStrategyWidget(QWidget *parent = nullptr);
    ~StressStrategyWidget() override;

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(StressStrategyCalculator *calculator, const QString &testUid);

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
    Ui::StressStrategyWidget *ui;

    DynamicDiagram *m_wgtDiag {nullptr};

    double m_valTarget {0};
    double m_valZones {0};
    QString m_sTarget {""};
    QString m_sFiveZones {""};
    QString m_sStressStrategyResume {""};
};

#endif // STRESSSTRATEGYWIDGET_H
