#ifndef STABTESTPARAMSDIALOG_H
#define STABTESTPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QStandardItemModel>

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

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void selectProbe(const int numProbe);
    void selectProbe(const QModelIndex index);
    void addProbe();
    void deleteProbe();

    void changeTime(const QTime &time);
    void changeLatentTime(const QTime &time);
    void changeStimulIndex(const int stimul);
    void changeZeroing(const bool zeroing);
    void changeScale(const int scale);

private:
    Ui::StabTestParamsDialog *ui;

    void showProbeParam();

    int m_curProbe = -1;
    QStandardItemModel *m_mdlProbes {nullptr};
};

#endif // STABTESTPARAMSDIALOG_H
