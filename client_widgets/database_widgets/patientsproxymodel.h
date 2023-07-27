#ifndef PATIENTSPROXYMODEL_H
#define PATIENTSPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

/*!
 * \brief Класс прокси модели для списка пациентов PatientsProxyModel class
 */
class PatientsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit PatientsProxyModel(QObject *parent = nullptr);

    /*!
     * \brief Режим отображения пациентов по индивидуальным программам The ShowByPersonalProgram enum
     */
    enum ShowByPersonalProgram
    {
          sppActive = 0 ///< Показывать только пациентов с назначенными программами
        , sppFree = 1   ///< Показывать только пациентов без назначенных программ
    };

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

    void setFilterValue(const QString &value);

    void setShowPPMode(ShowByPersonalProgram sppMode);

private:
    QString m_filerValue {""};
    ShowByPersonalProgram m_sppMode {sppFree};
};

#endif // PATIENTSPROXYMODEL_H
