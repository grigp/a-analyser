#ifndef IDSWIDGET_H
#define IDSWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class IDSWidget;
}

class IDSCalculator;

/*!
 * \brief Класс виджета визуализатора динамической стабилизации в рамках стабилографического теста IDSWidget class
 */
class IDSWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IDSWidget(QWidget *parent = 0);
    ~IDSWidget();

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(IDSCalculator *calculator, const QString &testUid);

private slots:
    void splitterMoved(int pos,int index);


private:
    Ui::IDSWidget *ui;

    void addFactorsFromMultifactor(IDSCalculator *calculator);

    void saveSplitterPosition();
    void restoreSplitterPosition();

    QStandardItemModel m_mdlTable;
};

#endif // IDSWIDGET_H
