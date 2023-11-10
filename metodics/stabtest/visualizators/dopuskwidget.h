#ifndef DOPUSKWIDGET_H
#define DOPUSKWIDGET_H

#include <QWidget>
#include <QPrinter>

#include "datadefines.h"
#include "circlenormindicator.h"

namespace Ui {
class DopuskWidget;
}

class DopuskCalculator;
class TestCalculator;

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
    static void paintPreview(QPainter* painter, QRect& rect, const QString& testUid, TestCalculator* calculator);

private slots:
    /*!
     * \brief Произошел перерасчет индивидуальной нормы
     * \param patientUid - uid пациента
     * \param methodUid - uid методики
     * \param conditionUid - uid условий проведения
     */
    void on_changePersonalNorm(const QString &patientUid, const QString &methodUid, const QString &conditionUid);

    /*!
     * \brief Произошло изменение условий проведения теста
     * \param testUid - uid теста
     */
    void on_changeTestCondition(const QString &testUid);


private:
    Ui::DopuskWidget *ui;

    void getGroupNorm(const QString &methodUid, const QString &conditionUid);
    void getPersonalNorms();

    void showConslution();

    DataDefines::NormValue getGroupNormValue(const int numProbe);
    DataDefines::NormValue getPersonalNormValue(const int numProbe, const NormBounds &pn);

    double m_values[3];

    QString m_testUid {""};
    QList<DataDefines::PersonalNormInfo> m_pnil;

    QMap<int, GroupNorm> m_groupNorms;      ///< Групповые нормы int - номер пробы

    CircleNormIndicator *m_wgtOpenEyesNorm {nullptr};
    CircleNormIndicator *m_wgtCloseEyesNorm {nullptr};
    CircleNormIndicator *m_wgtTargetNorm {nullptr};
    QString m_sOpenEyesResult {""};
    QString m_sOpenEyesFactor {""};
    QString m_sCloseEyesResult {""};
    QString m_sCloseEyesFactor {""};
    QString m_sTargetResult {""};
    QString m_sTargetFactor {""};

    QString m_sOpenEyesGroupNormResume {""};
    QString m_sCloseEyesGroupNormResume {""};
    QString m_sTargetGroupNormResume {""};
    QString m_sOpenEyesPersonalNormResume {""};
    QString m_sCloseEyesPersonalNormResume {""};
    QString m_sTargetPersonalNormResume {""};
};

#endif // DOPUSKWIDGET_H
