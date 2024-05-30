#ifndef MWCOMPETITIVESTICKVISUALIZE_H
#define MWCOMPETITIVESTICKVISUALIZE_H

#include <QWidget>
#include <QPrinter>


namespace Ui {
class MWCompetitiveStickVisualize;
}

class MWCompetitiveStickCalculator;
class TestResultData;
class SignalResultInfo;

/*!
 * \brief Класс виджета показа результатов теста с соревновательной палкой The MWCompetitiveStickVisualize class
 */
class MWCompetitiveStickVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit MWCompetitiveStickVisualize(QWidget *parent = nullptr);
    ~MWCompetitiveStickVisualize();

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
    static void paintPreview(QPainter* painter, QRect& rect, const QString& testUid, MWCompetitiveStickCalculator* calculator);


private slots:
    void on_scaleChange(int idx);

    void btnFulSignalClicked(bool isFullSignal);

    void btnPlusClicked();
    void btnMinusClicked();

    void on_signalScroll(int pos);

    void on_moveCursor();

private:
    Ui::MWCompetitiveStickVisualize *ui;

    MWCompetitiveStickCalculator* m_calculator {nullptr};
    TestResultData* m_trd {nullptr};
    SignalResultInfo* m_signal {nullptr};
    double m_absMax {0};
};

#endif // MWCOMPETITIVESTICKVISUALIZE_H
