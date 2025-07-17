#ifndef STABTESTPARAMSDIALOG_H
#define STABTESTPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QMap>

#include "stabtestdefines.h"

namespace Ui {
class StabTestParamsDialog;
}

/*!
 * \brief Класс диалога редактирования параметров методики
 * "Стабилографический тест" StabTestParamsDialog class
 */
class StabTestParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StabTestParamsDialog(QWidget *parent = nullptr);
    ~StabTestParamsDialog() override;

    /*!
     * \brief Получает параметры для редактирования
     * \param params - параметры
     */
    void setParams(const QJsonObject &params);

    /*!
     * \brief Возвращает отредактированные параметры
     */
    QJsonObject getParams();

private slots:
    void on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void selectProbe(const int numProbe);
    void selectProbe(const QModelIndex index);
    void addProbe();
    void deleteProbe();

    void changeCondition(const int condition);

    void changeProbeKind(const int probeKind);
    void changeAutoEnd(const bool autoEnd);
    void changeTime(const QTime &time);
    void changeLatentTime(const QTime &time);
    void changeStimulIndex(const int stimul);
    void changeZeroing(const bool zeroing);
    void changeScale(const int scale);

    void on_editStimulParams();

    void on_ok();
    void on_cancel();
    void on_default();

private:
    Ui::StabTestParamsDialog *ui;


    /*!
     * \brief Названия окон пациента The PatientWindowsName
     */
    QMap<StabTestDefines::PatientWindows, QString> PatientWindowsName {
          std::pair<StabTestDefines::PatientWindows, QString> (StabTestDefines::pwNone, QCoreApplication::tr("нет"))
        , std::pair<StabTestDefines::PatientWindows, QString> (StabTestDefines::pwColors, QCoreApplication::tr("Цветные круги"))
        , std::pair<StabTestDefines::PatientWindows, QString> (StabTestDefines::pwSound, QCoreApplication::tr("Звуковые сигналы"))
        , std::pair<StabTestDefines::PatientWindows, QString> (StabTestDefines::pwTarget, QCoreApplication::tr("Мишень"))
        , std::pair<StabTestDefines::PatientWindows, QString> (StabTestDefines::pwZones, QCoreApplication::tr("С выделенной зоной"))
        , std::pair<StabTestDefines::PatientWindows, QString> (StabTestDefines::pwLines, QCoreApplication::tr("Движение полос"))
    };


    void showProbeParam();

    void initUi();

    /*!
     * \brief Настройка режима доступа
     */
    void assignAccessMode();

    void fillProbeKinds();

    int m_curProbe = -1;
    QStandardItemModel *m_mdlProbes {nullptr};
    QMap<int, QDialog*> m_stimulParamsEditors;
};

#endif // STABTESTPARAMSDIALOG_H
