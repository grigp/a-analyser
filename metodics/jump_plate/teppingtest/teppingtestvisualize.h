#ifndef TEPPINGTESTVISUALIZE_H
#define TEPPINGTESTVISUALIZE_H

#include <QWidget>

namespace Ui {
class TeppingTestVisualize;
}

class TeppingTestCalculator;

/*!
 * \brief Виджет показа результатов теппинг - теста TeppingTestVisualize class
 */
class TeppingTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit TeppingTestVisualize(QWidget *parent = 0);
    ~TeppingTestVisualize();

    void setTest(const QString &testUid);

private slots:
    void on_selectGraph();
    void on_selectBar();
    void on_select3D(bool checked);

    void on_selectPage(const int pageIdx);
    void on_exportToMSExcel();

    void on_selectDiagItemLeft(const int idx);
    void on_selectDiagItemRight(const int idx);

private:
    Ui::TeppingTestVisualize *ui;

    void setTable();
    void setDiags();

    void restoreGraphParams();

    void showCurrentValues(const int idx);

    TeppingTestCalculator* m_calculator {nullptr};
};

#endif // TEPPINGTESTVISUALIZE_H
