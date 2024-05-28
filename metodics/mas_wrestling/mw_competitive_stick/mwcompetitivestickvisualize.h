#ifndef MWCOMPETITIVESTICKVISUALIZE_H
#define MWCOMPETITIVESTICKVISUALIZE_H

#include <QWidget>
#include <QPrinter>

namespace Ui {
class MWCompetitiveStickVisualize;
}

class MWCompetitiveStickCalculator;

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


private:
    Ui::MWCompetitiveStickVisualize *ui;

    MWCompetitiveStickCalculator* m_calculator {nullptr};
};

#endif // MWCOMPETITIVESTICKVISUALIZE_H
