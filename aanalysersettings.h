#ifndef AANALYSERSETTINGS_H
#define AANALYSERSETTINGS_H

#include <QDialog>
#include <QStandardItemModel>

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
    explicit AAnalyserSettings(QWidget *parent = 0);
    ~AAnalyserSettings();

public slots:
    int exec() override;
    void accept() override;


private:
    Ui::AAnalyserSettings *ui;

    ///< Роли модели локализации пользователя
    enum SitiesModelRoles
    {
        SMRAccelerationGravityRole = Qt::UserRole + 1       ///< Значение ускорения свободного падения Double
    };

    void load();
    void save();

    void fillSities(const QString& country, const QString& sity);

    QStandardItemModel m_mdlSities;
    QModelIndex m_idxCountry {QModelIndex()};
    QModelIndex m_idxSity {QModelIndex()};
};

#endif // AANALYSERSETTINGS_H
