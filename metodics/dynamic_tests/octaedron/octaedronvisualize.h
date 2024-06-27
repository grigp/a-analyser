#ifndef OCTAEDRONVISUALIZE_H
#define OCTAEDRONVISUALIZE_H

#include <QWidget>
#include <QPrinter>
#include <QStandardItemModel>

namespace Ui {
class OctaedronVisualize;
}

class OctaedronCalculator;
class DiagOctaedron;

/*!
 * \brief Класс виджета показа результатов тренажера "Октаэдр" OctaedronVisualize class
 */
class OctaedronVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit OctaedronVisualize(QWidget *parent = nullptr);
    ~OctaedronVisualize() override;

    void setTest(const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

private:
    Ui::OctaedronVisualize *ui;

    void showTableFactors();

    OctaedronCalculator* m_calculator {nullptr};
    DiagOctaedron *m_wgtDiag {nullptr};
    QString m_sAverageQuality {""};
    QStandardItemModel* m_model {nullptr};
};

#endif // OCTAEDRONVISUALIZE_H
