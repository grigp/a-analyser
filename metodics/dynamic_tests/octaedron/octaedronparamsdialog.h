#ifndef OCTAEDRONPARAMSDIALOG_H
#define OCTAEDRONPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class OctaedronParamsDialog;
}

/*!
 * \brief Класс диалога редактирования параметоров тренажера "Октаэдр" OctaedronParamsDialog class
 */
class OctaedronParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OctaedronParamsDialog(QWidget *parent = nullptr);
    ~OctaedronParamsDialog();

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
    void onCirceRoundRuleMode(int idx);
    void onDirectionModeChange(int idx);

private:
    Ui::OctaedronParamsDialog *ui;
};

#endif // OCTAEDRONPARAMSDIALOG_H