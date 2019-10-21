#ifndef STABTESTPARAMSDIALOG_H
#define STABTESTPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QStandardItemModel>
#include <QItemSelectionModel>

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
    explicit StabTestParamsDialog(QWidget *parent = 0);
    ~StabTestParamsDialog();

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

private:
    Ui::StabTestParamsDialog *ui;

    void showProbeParam();

    void initUi();

    /*!
     * \brief Настройка режима доступа
     */
    void assignAccessMode();

    void fillProbeKinds();

    int m_curProbe = -1;
    QStandardItemModel *m_mdlProbes {nullptr};
};

#endif // STABTESTPARAMSDIALOG_H
