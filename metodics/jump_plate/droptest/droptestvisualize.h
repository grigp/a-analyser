#ifndef DROPTESTVISUALIZE_H
#define DROPTESTVISUALIZE_H

#include <QWidget>
#include <QMap>

namespace Ui {
class DropTestVisualize;
}

class DropTestCalculator;
class DynamicDiagram;

/*!
 * \brief Виджет показа результатов дроп теста DropTestVisualize class
 */
class DropTestVisualize : public QWidget
{
    Q_OBJECT

public:
    explicit DropTestVisualize(QWidget *parent = 0);
    ~DropTestVisualize();

    void setTest(const QString &testUid);

private slots:
    void on_selectGraph();
    void on_selectBar();
    void on_select3D(bool checked);

    void on_selectFactorDiag1(int idx);
    void on_selectFactorDiag2(int idx);
    void on_selectFactorDiag3(int idx);

    void on_selectDiag1Item(const int idx);
    void on_selectDiag2Item(const int idx);
    void on_selectDiag3Item(const int idx);

    void on_selectPage(const int pageIdx);

private:
    Ui::DropTestVisualize *ui;

    void showSelectedValues(const int idx);

    int getPatientMassa(const QString &testUid);

    void setDiagram();

    void showDiagram(const int column, DynamicDiagram* diag);

    void restoreGraphParams();

    enum DropTestModelRoles
    {
        ValueRole = Qt::UserRole + 1
    };


    DropTestCalculator* m_calculator {nullptr};
};

#endif // DROPTESTVISUALIZE_H
