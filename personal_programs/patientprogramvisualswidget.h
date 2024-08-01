#ifndef PATIENTPROGRAMVISUALSWIDGET_H
#define PATIENTPROGRAMVISUALSWIDGET_H

#include <QWidget>
#include <QList>
#include <QTabWidget>

namespace Ui {
class PatientProgramVisualsWidget;
}

class PatientProgramWidget;
class PPVisual;

/*!
 * \brief Класс виджета для отображения визуализаторов индивидуальной программы для пациента The PatientProgramVisualsWidget class
 */
class PatientProgramVisualsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PatientProgramVisualsWidget(PatientProgramWidget* wgtPP,
                                         QList<PPVisual*> visuals,
                                         QWidget *parent = nullptr);
    ~PatientProgramVisualsWidget();

    /*!
     * \brief Загружает индивидуальную программу по ее uid
     * \param patientUid - uid индивидуальной программы
     */
    void assignPersonalProgram(const QString& uidPP);

    /*!
     * \brief Возвращает индекс выбранного итема
     */
    QModelIndex selectedIndex() const;

    /*!
     * \brief Возвращает название индивидуальной программы
     */
    QString namePP() const;

    /*!
     * \brief Возвращает указатель на виджет или nullptr, если он не активен.
     */
    PatientProgramWidget* getCurrentPPW();

    /*!
     * \brief Возвращает указатель на визуал или nullptr, если визуалов нет или активен PPW
     */
    PPVisual* getCurrentVisual();

signals:
    /*!
     * \brief Сигнал выбора нового табулятора
     * \param index - индекс нового табулятора
     */
    void tabChanged(int index);

private slots:
    void on_currentChanged(int index);

private:
    Ui::PatientProgramVisualsWidget *ui;

    /*!
     * \brief Располагает виджеты на панели виджета
     */
    void assign();

    PatientProgramWidget* m_wgtPP {nullptr};
    QList<PPVisual*> m_visuals;
    QTabWidget* m_tabs {nullptr};
};

#endif // PATIENTPROGRAMVISUALSWIDGET_H
