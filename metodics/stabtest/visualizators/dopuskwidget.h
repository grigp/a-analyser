#ifndef DOPUSKWIDGET_H
#define DOPUSKWIDGET_H

#include <QWidget>

#include "datadefines.h"

namespace Ui {
class DopuskWidget;
}

class DopuskCalculator;

/*!
 * \brief Класс виджета визуализатора допускового контроля в рамках стабилографического теста DopuskWidget class
 */
class DopuskWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DopuskWidget(QWidget *parent = 0);
    ~DopuskWidget();

    /*!
     * \brief Метод расчета и отображения данных
     * \param testUid - uid теста
     */
    void calculate(DopuskCalculator *calculator, const QString &testUid);

private slots:
    /*!
     * \brief Произошел перерасчет индивидуальной нормы
     * \param patientUid - uid пациента
     * \param methodUid - uid методики
     * \param conditionUid - uid условий проведения
     */
    void on_recalculatedPersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid);


private:
    Ui::DopuskWidget *ui;

    void getNorms();

    QString m_testUid {""};
    QList<DataDefines::PersonalNormInfo> m_pnil;
};

#endif // DOPUSKWIDGET_H
