#ifndef TRENVISUALIZE_H
#define TRENVISUALIZE_H

#include <QWidget>
#include <QPrinter>
#include <QStandardItemModel>

namespace Ui {
class TrenVisualize;
}

class TrenTestCalculator;

/*!
 * \brief Класс виджета показа результатов тренажеров TrenVisualize class
 */
class TrenVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit TrenVisualize(QWidget *parent = nullptr);
    ~TrenVisualize() override;

    void setTest(const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

private:
    Ui::TrenVisualize *ui;

    TrenTestCalculator* m_calculator {nullptr};
    QStandardItemModel *m_mdlFactors;
};

#endif // TRENVISUALIZE_H
