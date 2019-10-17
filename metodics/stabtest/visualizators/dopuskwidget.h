#ifndef DOPUSKWIDGET_H
#define DOPUSKWIDGET_H

#include <QWidget>

#include "datadefines.h"
#include "circlenormindicator.h"

namespace Ui {
class DopuskWidget;
}

class DopuskCalculator;

/*!
 * \brief Запись о текущей групповой норме GroupNorm struct
 */
struct GroupNorm
{
    double bound;
    double conditionBound;
    GroupNorm(){}
    GroupNorm(double bnd, double cndBnd) : bound(bnd), conditionBound(cndBnd) {}
};

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
    void on_changePersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid);


private:
    Ui::DopuskWidget *ui;

    void getPersonalNorms();

    void showConslution();

    DataDefines::NormValue getGroupNormValue(const int numProbe);
    DataDefines::NormValue getPersonalNormValue(const int numProbe, const NormBounds &pn);

    double m_values[3];

    QString m_testUid {""};
    QList<DataDefines::PersonalNormInfo> m_pnil;

    QMap<int, GroupNorm> m_groupNorms;      ///< Групповые нормы int - номер пробы
};

#endif // DOPUSKWIDGET_H
