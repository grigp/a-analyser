#ifndef TESTPROPERTYDIALOG_H
#define TESTPROPERTYDIALOG_H

#include <QDialog>

namespace Ui {
class TestPropertyDialog;
}

class TestPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestPropertyDialog(QWidget *parent = nullptr);
    ~TestPropertyDialog();

    QString comment();
    void setComment(const QString &comment);

    QString condition();
    void setCondition(const QString &condition);

    bool normContained();
    void setNormContained(const bool nc);

    /*!
     * \brief Устанавливает видимость флажка "Использовать как нормообразующее"
     * \param visible - признак видимости
     */
    void setNormVisible(const bool visible);

    /*!
     * \brief Устанавливает видимость закладки условий проведения
     * \param visible - признак видимости
     */
    void setConditionsVisible(const bool visible);

    enum ConditionRoles
    {
          UidRole = Qt::UserRole + 1
        , DescriptionRole
        , NormsEnabledRole
    };

private slots:
    void conditionChanged(int idx);

private:
    Ui::TestPropertyDialog *ui;

    void loadConditions();
};

#endif // TESTPROPERTYDIALOG_H
