#ifndef TRENTETRISPARAMSDIALOG_H
#define TRENTETRISPARAMSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class TrenTetrisParamsDialog;
}

class TrenTetrisParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrenTetrisParamsDialog(QWidget *parent = nullptr);
    ~TrenTetrisParamsDialog();

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
    void onPhisioChanEnabledChange(bool enabled);

private:
    Ui::TrenTetrisParamsDialog *ui;

    QJsonObject m_params;
};

#endif // TRENTETRISPARAMSDIALOG_H
