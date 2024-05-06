#ifndef REHABREPORTPPVISUALWIDGET_H
#define REHABREPORTPPVISUALWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

#include "ppvisual.h"

namespace Ui {
class RehabReportPPVisualWidget;
}

class MultiFactor;

/*!
 * \brief Класс виджета визуализатора ИП - отчет по курсу реабилитации The RehabReportPPVisualWidget class
 */
class RehabReportPPVisualWidget : public PPVisual
{
    Q_OBJECT

public:
    explicit RehabReportPPVisualWidget(PPVisualDescriptor* visual, QJsonObject &objPP, QWidget *parent = nullptr);
    ~RehabReportPPVisualWidget() override;

    bool isValid() override;
    void calculate() override;

private:
    Ui::RehabReportPPVisualWidget *ui;

    struct DPSuccessInfo
    {
        QString dt {""};
        double val {-1};
    };

    QString m_uidRomb1 {""};    ///< uid теста Ромберга перед началом процедур
    QString m_uidRomb2 {""};    ///< uid теста Ромберга перед окончанием процедур
    QString m_uidCross1 {""};   ///< uid теста на устойчивость перед началом процедур
    QString m_uidCross2 {""};   ///< uid теста на устойчивость перед окончанием процедур
    QVector<DPSuccessInfo> m_success;  ///< Динамика результативности по ДП

    QStandardItemModel m_mdlRomb;
    QStandardItemModel m_mdlCross;

    /*!
     * \brief Расчет вперед по ДП
     */
    void calculateStraight();
    /*!
     * \brief Расчет назад по ДП
     */
    void calculateReverse();

    void showRombergResults();
    void showCrossResults();
    void showSuccess();

    QString getTestTitle(const QString& uidTest) const;
    void addFactorColumnToModel(const QString& uidFactor,
                                MultiFactor* fg1,
                                MultiFactor* fg2,
                                QStandardItemModel* model,
                                QStringList& header,
                                const QString& fDetail,
                                const QMap<QString, QString>* factorNames);
    bool getProbesRomberg(const QString& uidTest, QString& uidProbe1, QString& uidProbe2) const;
    bool getProbeCross(const QString& uidTest, QString& uidProbe) const;
};

#endif // REHABREPORTPPVISUALWIDGET_H
