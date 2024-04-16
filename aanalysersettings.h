#ifndef AANALYSERSETTINGS_H
#define AANALYSERSETTINGS_H

#include <QDialog>
#include <QStandardItemModel>

namespace AAnalyserSettingsParams
{
    static const QString pn_patientWindowNumber = "PatientWindowNumber";
    static const QString pn_onePatientMode = "OnePatientMode";
    static const QString pn_onePatientFIO = "OnePatientFIO";
    static const QString pn_country = "Country";
    static const QString pn_sity = "Sity";
    static const QString pn_g = "g";
    static const QString pn_runningMode = "RunningMode";
    static const QString pn_timeCounter = "TimeCounter";
    static const QString pn_timeLatent = "TimeLatent";
    static const QString pn_testDoubleClickMode = "TestDoubleClickMode";
    static const QString pn_isClipCursor = "ClipCursor";

    static const QString pc_userLocalize = "UserLocalize";

}

namespace Ui {
class AAnalyserSettings;
}

/*!
 * \brief Класс диалога настройки общих параметров приложения AAnalyserSettings class
 * Пока простой
 */
class AAnalyserSettings : public QDialog
{
    Q_OBJECT

public:
    explicit AAnalyserSettings(QWidget *parent = nullptr);
    ~AAnalyserSettings() override;

public slots:
    int exec() override;
    void accept() override;

    void cbOnePatientClicked(bool checked);

private slots:
    void on_runningModeChange(int idx);

private:
    Ui::AAnalyserSettings *ui;

    ///< Роли модели локализации пользователя
    enum SitiesModelRoles
    {
        SMRAccelerationGravityRole = Qt::UserRole + 1       ///< Значение ускорения свободного падения Double
    };

    void load();
    void save();

    void fillListMonitors();
    void fillRunningMode();
    void fillTestDoubleClickMode();
    void fillSities(const QString& country, const QString& sity);

    QStandardItemModel m_mdlSities;
    QModelIndex m_idxCountry {QModelIndex()};
    QModelIndex m_idxSity {QModelIndex()};

    QString m_country {""};
    QString m_sity {""};
    int m_winPatientNumber {1};
    bool m_isOnePatient {false};
    QString m_onePatientFIO {""};
};

#endif // AANALYSERSETTINGS_H
