#ifndef BEDSIDESCALESTESTVISUALIZE_H
#define BEDSIDESCALESTESTVISUALIZE_H

#include <QWidget>
#include <QPrinter>

namespace Ui {
class BedsideScalesTestVisualize;
}

class BedsideScalesTestCalculator;

/*!
 * \brief Класс фрейма визуализации результатов для прикроватных весов The BedsideScalesTestVisualize class
 */
class BedsideScalesTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit BedsideScalesTestVisualize(QWidget *parent = nullptr);
    ~BedsideScalesTestVisualize() override;

    void setTest(const QString &testUid);

    /*!
     * \brief Печать отчета о результатах теста
     * \param printer - принтер
     * \param testUid - uid теста
     */
    static void print(QPrinter *printer, const QString &testUid);

private slots:
    void signalExport();

private:
    Ui::BedsideScalesTestVisualize *ui;

    void saveSignal(const QString &fileName);

    BedsideScalesTestCalculator* m_calculator {nullptr};
};

#endif // BEDSIDESCALESTESTVISUALIZE_H