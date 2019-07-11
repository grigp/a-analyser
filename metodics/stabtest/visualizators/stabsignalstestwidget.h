#ifndef STABSIGNALSTESTWIDGET_H
#define STABSIGNALSTESTWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class StabSignalsTestWidget;
}

/*!
 * \brief Класс виджета визуализатора обработчика сигналов стабилографического теста StabSignalsTestWidget class
 */
class StabSignalsTestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StabSignalsTestWidget(QWidget *parent = 0);
    ~StabSignalsTestWidget();

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(const QString &testUid);

private:
    Ui::StabSignalsTestWidget *ui;
    QStandardItemModel m_mdlTable;
};

#endif // STABSIGNALSTESTWIDGET_H
